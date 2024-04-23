#include <sys/poll.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <poll.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#define FD_LIMIT 1024

struct server_s {
	int server_fd;
	size_t length;
	int * client_fds;
	char** nicks;
};


typedef struct server_s Server;

int server_init(Server *s, char *host, int port) {
	s->server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (s->server_fd == -1)
		return 1;

	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr(host);
	addr.sin_port = htons(port);

	if (bind(s->server_fd, (struct sockaddr*)&addr, sizeof(addr)) == -1)
		return 1;
	
	if (listen(s->server_fd, 20) == -1)
		return 1;

	s->client_fds = malloc(FD_LIMIT * sizeof(int));
	s->length = 0;
	s->nicks = malloc(FD_LIMIT * sizeof(char*));
	for(int i = 0; i < FD_LIMIT; i++) {
		s->nicks[i] = malloc(1024 * sizeof(char));
		s->nicks[i][0] = 0;
	}

	for (int i = 0; i < FD_LIMIT; i++)
		s->client_fds[i] = -1;

	if (s->client_fds == NULL)
		return 1;

	return 0;
}

void server_add_fd(Server * s, int fd) {
	for (int i = 0; i < FD_LIMIT; i++) {
		if (s->client_fds[i] == -1) {
			s->client_fds[i] = fd;
			s->length++;
			return;
		}
	}
}

void server_del_fd(Server * s, int fd) {
	for(int i = 0; i < FD_LIMIT; i++) {
		if (s->client_fds[i] == fd) {
			s->client_fds[i] = -1;
			s->length--;
			return;
		}
	}
}

struct pollfd * server_new_pollfds(Server * s, nfds_t * nfds_out) {
	nfds_t nfds = (s->length + 1);
	struct pollfd * pollfds = malloc(nfds * sizeof(struct pollfd));
	pollfds[0] = (struct pollfd){
		.fd = s->server_fd,
		.events = POLLIN,
		.revents = POLLIN | POLLHUP | POLLERR,
	};

	int pollfd_i = 1;

	for (int i = 0; i < FD_LIMIT; i++) {
		if (s->client_fds[i] == -1)
			continue;

		pollfds[pollfd_i++] = (struct pollfd){
			.fd = s->client_fds[i],
			.events = POLLIN,
			.revents = POLLIN | POLLHUP | POLLERR,
		};
	}
	
	*nfds_out = nfds;
	return pollfds;
}

void server_free_pollfds(struct pollfd * pollfds) {
	free(pollfds);
}

void server_connect(Server * s) {
	int new_fd = accept(s->server_fd, NULL, NULL);
	server_add_fd(s, new_fd);
	printf("[%d] Connected\n", new_fd);
}

void server_disconnect(Server * s, int fd) {
	close(fd);
	server_del_fd(s, fd);
	printf("[%d] Disconnected\n", fd);
}

int server_process(Server *s) {\
	const size_t buff_size = 1024;
	char buff[buff_size];
	nfds_t nfds;
	struct pollfd * pollfds = server_new_pollfds(s, &nfds);
	
	int r = poll(pollfds, nfds, -1);
	if (r < 0)
		return 1;

	for (int i = 1; i < nfds; i++) {
		if (pollfds[i].revents) {
			int fd = pollfds[i].fd;
			short revents = pollfds[i].revents;
			if (revents & (POLLIN)) {
				
				size_t r = recv(fd, buff, buff_size, 0);
				//detect disconnect
				if (r == 0) {
					server_disconnect(s, fd);
					continue;
				}
				//detect nick change message
				if (r > 0 && buff[0] == '@') {
					for (int i = 1; i < r; i++) {
						s->nicks[fd][i - 1] = buff[i];
						//printf("%c", buff[i]);
					}
					printf("[%d] Nick changed to %s\n", fd, s->nicks[fd]);
					continue;
				}
				for (int i = 0; i < FD_LIMIT; i++) {
					int rfd = s->client_fds[i];
					if (rfd != -1 && rfd != fd) {
						//prepend message with nick
						send(rfd, s->nicks[fd], strlen(s->nicks[fd]), 0);
						send(rfd, ": ", 2, 0);
						send(rfd, buff, r, 0);
					}
				}
			}
			if (revents & (POLLERR | POLLHUP)) {
				server_disconnect(s, fd);
			}
		}
	}

	if (pollfds[0].revents) {
		server_connect(s);
	}

	server_free_pollfds(pollfds);

	return 0;
}


int main() {
	Server s;
	char *host = "0.0.0.0";
	int port = 2137;
	while (server_init(&s, host, port)) {
		fprintf(stderr, "Server init failed.\n");
		sleep(1);
		//return 1;
	}
	printf("Listening on %s:%d\n", host, port);
	while (1)
		server_process(&s);
}

#include <arpa/inet.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>


void chat(int sock_fd)
{
	fd_set read_fds;
	FD_ZERO(&read_fds);
	FD_SET(sock_fd, &read_fds);
	FD_SET(STDIN_FILENO, &read_fds);
	int max_fd = sock_fd;
	while (1)
	{
		fd_set tmp_fds = read_fds;
		if (select(max_fd + 1, &tmp_fds, NULL, NULL, NULL) == -1)
		{
			printf("Select() failed!\n");
			break;
		}
		if (FD_ISSET(sock_fd, &tmp_fds))
		{
			char buf[1024];
			int len = read(sock_fd, buf, sizeof(buf));
			if (len <= 0)
			{
				printf("Server closed connection!\n");
				break;
			}
			buf[len] = 0;
			printf("%s", buf);
		}
		if (FD_ISSET(STDIN_FILENO, &tmp_fds))
		{
			char buf[1024];
			int len = read(STDIN_FILENO, buf, sizeof(buf));
			if (len <= 0)
			{
				printf("Read error!\n");
				break;
			}
			buf[len] = 0;
			write(sock_fd, buf, len);
		}
	}
}

int main()
{
	int sock_fd, conn_fd;
	struct sockaddr_in endpoint;
	sock_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (sock_fd == -1)
		return 1;
	bzero(&endpoint, sizeof(endpoint));
	endpoint.sin_family = AF_INET;
	endpoint.sin_addr.s_addr = inet_addr("172.31.193.77");
	endpoint.sin_port = htons(2137);
	if (connect(sock_fd, (struct sockaddr *)&endpoint, sizeof(endpoint)))
	{
		printf("Failed to connect!\n");
		return 1;
	}
	printf("Connected! Enter your nick: \n");
	char buf[512] = {'@'};
	scanf("%s", buf+1);
	//printf("Your nick is: %s\n", buf);
	write(sock_fd, buf, strlen(buf));
	chat(sock_fd);
	close(sock_fd);
	return 0;
}

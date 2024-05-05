#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include "config.h"

int main(){
	int fd1[2], fd2[2];
	enum {READ, WRITE};
	if (pipe(fd1) == -1 || pipe(fd2) == -1){
		perror("Cannot create pipe(s)!");
		return 1;
	}
	int pid = fork();
	if (pid == -1){
		perror("Cannot fork!");
		return 2;
	}
	if (pid > 0){
		char buf[BUFFER_SIZE];
		while(1){
			fgets(buf, BUFFER_SIZE, stdin);
			write(fd1[WRITE], buf, BUFFER_SIZE);
			printf("ReadKeyboard--fd1-->SwapCase: %s\n", buf);
		}
	}
	else{
		int pid2 = fork();
		if (pid2 == -1){
			perror("Cannot fork!");
			return 3;
		}
		if (pid2 > 0){
			char buf[BUFFER_SIZE];
			while(1){
				read(fd1[READ], buf, BUFFER_SIZE);
				for (int i = 0; i < BUFFER_SIZE; i++){
					if (buf[i] >= 'a' && buf[i] <= 'z'){
						buf[i] = buf[i] - 32;
					}
					else if (buf[i] >= 'A' && buf[i] <= 'Z'){
						buf[i] = buf[i] + 32;
					}
				}
				write(fd2[WRITE], buf, BUFFER_SIZE);
				printf("SwapCase--fd2-->MulNumbers: %s\n", buf);
			}
		}
		else{
			char buf[BUFFER_SIZE];
			int fd3 = open(FIFO_PATH, O_WRONLY);
			if (fd3 == -1){
				perror("Cannot open FIFO for writing - no one is reading!");
				return 4;
			}
			while(1){
				read(fd2[READ], buf, BUFFER_SIZE);
				for (int i = 0; i < BUFFER_SIZE; i++){
					if (buf[i] >= '0' && buf[i] <= '9'){
						buf[i] = ((buf[i]-'0') * 2) % 10 + '0';
					}
				}
				write(fd3, buf, BUFFER_SIZE);
				printf("MulNumbers - sent to FIFO: %s\n", buf);
			}
		}
	}
}
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char* argv[]){
	int depth;
	if(argc<2){
		printf("Missing parameter: N\n");
		return 1;
	}
	else{
		depth = atoi(argv[1])*2;
	}
	for(int i = 1; i<depth; i++){
		int pid = fork();
		if(pid==-1){
			perror("Fork error");
			return 2;
		}
		if((pid==0)==(i%2==1)){
			pause();
			return 0;	
		}
	}
	execlp("ps","-u student","-l","--forest",NULL);
	return 0;
}

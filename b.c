#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(){
	pid_t pid, pid_wait;
	int fd[2];
	char buff[64], *cmd = "exit";
	
	if (pipe(fd)) {
		perror ("Create pipe fail!");
		return 0;
		}
	pid = fork();
	if(-1 == pid){
		perror ("Create process fail!\n");
		return 0;
		}
	else if (0==pid){  //Child Thread
		close(fd[1]); 	//close write
		printf("Wait command from parent!\n");
		while(1){
			read (fd[0],buff,64);
			if (0==strcmp(buff,cmd)){
				printf("recv command ok !\n");
				close (fd[0]);	//close read
				exit(0);
				}
			}
		}
		else{					//Parent Thread
			printf("Parent process! child process id : %d\n",pid);
			close (fd[0]);	//close read
			sleep(2);
			printf("Send command to child process. \n");
			write(fd[1],cmd,strlen(cmd)+1);	//write cmd
			close (fd[1]);	//closr write
			}
	return 0;
	}
	

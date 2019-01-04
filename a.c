#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
	int fd[2];
	pid_t pid;
	char buf[64] = "Piple data transmite!\n";
	char line[64];
	
	if(0!=pipe(fd)){
		fprintf(stderr, "Fail to create pipe!\n");
		return 0;
		}	
		
	pid=fork();
	if(pid<0){
		fprintf(stderr,"Fail to create process!\n");
		return 0;
		}
	else if(0==pid){
		close(fd[0]);	//close read pipe
		write (fd[1],buf,strlen(buf));
		close(fd[1]);
		}
		else{
			close (fd[1]);
			read(fd[0],line,64);
			printf("Data From Pipe: %s", line);
			close(fd[0]);			
			}
			
	return 0;
	
}

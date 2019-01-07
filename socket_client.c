// echo socket_client
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#define ECHO_PORT 8080
#define MAX_COMMAND 5

int main(){
	int sock_fd;
	struct sockaddr_in structServerAddr;

	char *pSendBuf[MAX_COMMAND] ={"abc","def","test","hello","quit"};
	char chrRcvBuf[100+1];
	socklen_t len;
	int n,i;

	/*Create socket*/
	sock_fd=socket(AF_INET, SOCK_STREAM,0);
	if(-1==sock_fd){
		perror("create socket error!");
		return 0;
	}else{
		printf("Success to create socket %d\n",sock_fd);
	}

	/*Set server addr struct*/
	bzero(&structServerAddr,sizeof(structServerAddr));		//init struct
	structServerAddr.sin_family=AF_INET;				//set transport layer type
	structServerAddr.sin_port=htons(ECHO_PORT);			//set Listening Port
	structServerAddr.sin_addr.s_addr=inet_addr("127.0.0.1");	//set server addr
	bzero(&(structServerAddr.sin_zero),8);				//	

	/* connect to server	*/
	if(-1 == connect(sock_fd,(struct sockaddr*)&structServerAddr,sizeof(structServerAddr))){
		perror("connect() error!\n");
		close(sock_fd);
		return 0;
	}else{
		printf("Success connect to server!\n");
	}

	/*Send and Recv data*/
	for(i=0;i<MAX_COMMAND;i++){
		send(sock_fd,pSendBuf[i],100,0);	//send data to server
		n=recv(sock_fd,chrRcvBuf,100,0);	//recv data
		chrRcvBuf[n]='\0';
		printf("data send: %s receive: %s \n",pSendBuf[i],chrRcvBuf);
		if(0==strncmp(chrRcvBuf,"quit",4)) break;
	}
	close(sock_fd);
	return 0;
}


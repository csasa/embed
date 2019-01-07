//echo socket_server.c  
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#define ECHO_PORT 8080
#define MAX_CLIENT_NUM 10

int main(){
	int intListenSockFd,intClientSockFd;
	struct sockaddr_in structServerAddr, structClientAddr;
	char chrRcvBuf[101];
	socklen_t len;
	int reuse = 1;
	int cnt;

	/*Create Socket*/
	intListenSockFd=socket(AF_INET,SOCK_STREAM,0);	//AF_INET:IPV4  SOCK_STREAM:TCP
	if(-1==intListenSockFd){
		perror("create socket error!");
		return 0;
	}else{
		printf("Success to create socket,socket Id=%d\n",intListenSockFd);
	}
	
	/* 打开 SO_REUSEADDR 选项，解决bind时 EADDRINUSE 错误*/
	if (setsockopt(intListenSockFd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0){
		perror("setsockopet error\n");
		return -1;
	}

	/*Set server addr struct*/
	bzero(&structServerAddr,sizeof(structServerAddr));	//init struct
	structServerAddr.sin_family=AF_INET;				//set transport layer type
	structServerAddr.sin_port=htons(ECHO_PORT);			//set Listening Port
	structServerAddr.sin_addr.s_addr=htons(INADDR_ANY);	//set server addr
	bzero(&(structServerAddr.sin_zero),8);				//

	/*bind the addr and socket*/
	if(bind(intListenSockFd,(struct sockaddr*)&structServerAddr,sizeof(structServerAddr))!=0){
		printf("bind address fail! %d\n",errno);
		close(intListenSockFd);
		return 0;
	}else{
		printf("Success to bind address!\n");
	}

	/* Set socket to listening*/
	if(listen(intListenSockFd,MAX_CLIENT_NUM)!=0){
		perror("listen socket error!\n");
		close(intListenSockFd);
		return 0;
	}else{
		printf("Success to listening\n");
	}

	/* create new session socket*/
	len=sizeof(structClientAddr);
	//后续accept，会阻塞进程，直到有客户端连接上来为止
	intClientSockFd=accept(intListenSockFd,(struct sockaddr*)&structClientAddr,&len);
	if(intClientSockFd<=0){
		perror("accept error!\n");
		close(intListenSockFd);
		return 0;
	}else{
		printf("Create Client socket OK!\n");
	}

	/*recv user data*/
	while(1){
		cnt = recv(intClientSockFd,chrRcvBuf,100,0);
		if(cnt>0){
			chrRcvBuf[cnt]='\0';	//add terminator
			printf("number of receive bytes = %d data = %s\n",cnt,chrRcvBuf);
			fflush(stdout);
			send(intClientSockFd,chrRcvBuf,cnt,0);	//Send data to client
			if(strncmp(chrRcvBuf,"quit",4)==0) break;	//judge exit cmd
		}else{
	        if((cnt<0) &&(errno == EAGAIN||errno == EWOULDBLOCK||errno == EINTR))
			{
	            continue;//继续接收数据
	        }
	        break;//跳出接收循环			
		}
	}

	close(intClientSockFd);	//close session
	close(intListenSockFd);		//close listening socket
	return 0;
}


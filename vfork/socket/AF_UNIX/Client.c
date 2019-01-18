//c_unix.c
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#define UNIX_DOMAIN "/tmp/UNIX.domain"

void* recvMsg(void * socket)
{
    int sock = *((int*)socket);
    while (1) {
        char msg[4096];
        memset(msg, 0, sizeof(msg));
 
        ssize_t bytecount = recv(sock, msg, sizeof(msg), 0); //接收消息
        if (bytecount <= 0) {
          usleep(500*1000);
          continue;
        } else {
          printf("recv:%s\n", msg);
        }
 
    }
}


int main(void)
{
  int connect_fd;
  int ret;
  char snd_buf[1024];
  int i;
  static struct sockaddr_un srv_addr;
  pthread_t recv;
//creat unix socket

  connect_fd=socket(PF_UNIX,SOCK_STREAM,0);
  if(connect_fd<0)
  {
    perror("cannot create communication socket");
    return 1;
  }  

  pthread_create(&recv, NULL, recvMsg, &connect_fd);

  srv_addr.sun_family=AF_UNIX;
  strcpy(srv_addr.sun_path,UNIX_DOMAIN);
//connect server
  ret=connect(connect_fd,(struct sockaddr*)&srv_addr,sizeof(srv_addr));
  if(ret==-1)
  {
    perror("cannot connect to the server");
    close(connect_fd);
    return 1;
  }
  memset(snd_buf,0,1024);
  strcpy(snd_buf,"message from client");
//send info server
  for(i=0;i<4;i++)
    write(connect_fd,snd_buf,sizeof(snd_buf));
  
  pthread_join(recv, 0);
  
  close(connect_fd);
  return 0;
}
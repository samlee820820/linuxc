#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <limits.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h> 
 
#define SERVER_W "serverWrite"
#define SERVER_R "serverRead"
 
void main()
{
    printf("程序开始\n");
    //服务端读写文件描述符
    int fd_w,fd_r;
    //服务端读写管道
    int pid_w,pid_r;
    //读写数组
    char msg_w[BUFSIZ],msg_r[BUFSIZ];
    //进程id
    pid_t pid;
 
    //首先要检查文件的存在性
    //access 存在返回0，不存在返回-1
    //写管道
    if(access(SERVER_W,0) < 0){
        //文件不存在，创建管道
        pid_w = mkfifo(SERVER_W,0700);
        if(pid_w < 0){
            perror("服务器创建写管道失败:");
            return;
        }
        printf("管道%s创建成功\n",SERVER_W);
    }
    //存在则打开管道
    fd_w = open(SERVER_W,O_RDWR);
    if(fd_w < 0){
        perror("打开写管道失败");
    }
    printf("打开写管道\n");
 
    //读管道
    if(access(SERVER_R,0) < 0){
        //文件不存在，创建管道
        pid_r = mkfifo(SERVER_R,0700);
        if(pid_r < 0){
            perror("创建读管道失败");
            return;
        }
         printf("管道%s创建成功\n",SERVER_R);
    }
    //存在则打开
    fd_r = open(SERVER_R,O_RDWR);
    if(fd_r < 0){
        perror("打开读管道失败");
    }
    printf("打开读管道\n");
 
    printf("服务端开始通信\n");
 
    pid = fork();
    if(pid < 0){
        perror("子进程创建失败\n");
    }else if(pid == 0){
        //子进程用于父持续读
        printf("服务端子进程开始\n");
        printf("\n---------------------提示：服务端可以读取消息\n");
        while(1){
            if( read(fd_r,msg_r,BUFSIZ) == -1 ){
                perror("服务端读取消息失败");
            }else{
                if( strlen(msg_r) > 0 ){
                    printf("****************服务端读取信息:%s\n",msg_r);
                    if (strcmp("EOF",msg_r) == 0){
                        printf("服务端朋友丢失,终止读取\n");
                        break;   
                    }
                }else{
                    printf("服务端无可读消息\n");
                }
                printf("服务端读取消息完成\n\n");    
            }
        }//while
 
        printf("---------------------提示：服务端子进程结束,终止读取\n");
        _exit(0);
    }else{
        //父进程用于持续写
        printf("父进程开始\n");
        printf("\n---------------------提示：服务端可以写入信息\n");
        while(1){  
            scanf("%s",msg_w);
            
            if ( write(fd_w,msg_w,strlen(msg_w)+1) == -1){
                perror("服务端发送消息失败");
            }else{
                printf("****************服务端发送:%s\n",msg_w);
                printf("服务端写入完成\n\n");
                if (strcmp("EOF",msg_w) == 0){       
                    break;
                }
            }
        }//while
        
        printf("---------------------提示：服务端父进程结束,终止写入\n");
        wait(NULL);
    }
 
    //关闭文件
    close(fd_w);
    printf("关闭写管道\n");
    close(fd_r);
    printf("关闭读管道\n");
    printf("程序结束\n");
    return;
}
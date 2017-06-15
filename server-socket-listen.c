#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/time.h>

#define BUFLEN 1024 
#define PORT 6666
#define LISTNUM 20

int main()
{
    int sockfd, newfd;
    struct sockaddr_in s_addr, c_addr;
    char buf[BUFLEN];
    socklen_t len;
    unsigned int port, listnum;
    fd_set rfds;
    struct timeval tv;
    int retval,maxfd;

    /*创建socket*/
    if((sockfd = socket(PF_INET, SOCK_STREAM, 0)) == -1){
        perror("socket");
        exit(errno);
    }else
        printf("socket create success!\n");
    memset(&s_addr,0,sizeof(s_addr));
    s_addr.sin_family = AF_INET;
    s_addr.sin_port = htons(PORT);
    s_addr.sin_addr.s_addr = inet_addr("182.92.123.13");

    /*绑定*/
    if((bind(sockfd, (struct sockaddr*) &s_addr,sizeof(struct sockaddr))) == -1){
        perror("bind");
        exit(errno);
    }else
        printf("bind success!\n");
    /*监听*/
    if(listen(sockfd,listnum) == -1){
        perror("listen");
        exit(errno);
    }else
        printf("the server is listening!\n");
    while(1){
        printf("*****************聊天开始**************\n");
        len = sizeof(struct sockaddr);
        if((newfd = accept(sockfd,(struct sockaddr*) &c_addr, &len)) == -1){
            perror("accept");
            exit(errno);
        }else
            printf("聊天的对象是：%s: %d\n",inet_ntoa(c_addr.sin_addr),ntohs(c_addr.sin_port));
        while(1){
            FD_ZERO(&rfds);
            FD_SET(0, &rfds);
            maxfd = 0;
            FD_SET(newfd, &rfds);
            if(maxfd < newfd)
                maxfd = newfd;
            tv.tv_sec = 6;
            tv.tv_usec = 0;
            retval = select(maxfd+1, &rfds, NULL, NULL, &tv);
            if(retval == -1){
                printf("select出错\n");
                break;
            }else if(retval == 0){
                printf("waiting...\n");
                continue;
            }else{
                if(FD_ISSET(0, &rfds)){
                    memset(buf,0,sizeof(buf));
                    fgets(buf,BUFLEN,stdin);
                    //fputs(buf,stdout); 
                    if(!strncasecmp(buf,"quit",4)){
                        printf("server 请求终止聊天\n");
                        break;
                    }
                        len = send(newfd,buf,strlen(buf),0);
                    if(len > 0)
                        printf("\t成功发送 ：%s\n",buf);
                    else{
                        printf("发送失败\n");
                        break;
                    }
                }
                if(FD_ISSET(newfd, &rfds)){
                    memset(buf,0,sizeof(buf));
                    len = recv(newfd,buf,BUFLEN,0);
                    if(len > 0)
                        printf("客户端发送的消息：%s\n",buf);
                    else{
                        if(len < 0 )
                            printf("消息接收失败\n");
                        else
                            printf("客户端退出聊天，聊天终止！\n");
                        break;
                    }
                }
            }
        }
        close(newfd);
        printf("服务器是否退出程序：y->是; n->否 ？\n ");
        bzero(buf, BUFLEN);
        fgets(buf,BUFLEN, stdin);
        if(!strncasecmp(buf,"y",1)){
            printf("server 离开 \n");
            break;
        }
    }
    close(sockfd);
    return 0;
}


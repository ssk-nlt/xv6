#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define ONEBETY 16
int main(){
    int fd_fw[2];
    int fd_cw[2];
    char buf[ONEBETY];
    int ret_w=pipe(fd_fw);
    int ret_c=pipe(fd_cw);
    if(ret_w==-1)
    {
        perror("pipe");
    }
    if(ret_c==-1)
    {
        perror("pipe");
    }

    int pid=fork();
    if(pid>0){
        int ret_ww=write(fd_fw[0],"ping",ONEBETY);
        int ret_wr=read(fd_cw[1],buf,ONEBETY);
        close(fd_fw[1]);
        close(fd_cw[0]);
        if(ret_ww==-1)
        {
            perror("write");
        }
        if(ret_wr==-1)
        {
            perror("read");
        }
        printf("<%d>:received %s\n",pid,buf);
    }
    else
    {
        int ret_cr=read(fd_fw[1],buf,ONEBETY);
        printf("<%d>:received %s\n",pid,buf);
        int ret_cw=write(fd_cw[0],"pong",ONEBETY);
        if(ret_cr==-1)
        {
            perror("read");
        }
        if(ret_cw==-1)
        {
            perror("write");
        }
        close(fd_fw[0]);
        close(fd_cw[1]);
        exit(0);
    }
    exit(0);
    return 0;
}
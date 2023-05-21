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
        printf("there a wrong:father");
        exit(1);
    }
    if(ret_c==-1)
    {
        printf("there a wrong:children");
        exit(1);
    }

    int pid=fork();
    if(pid>0){
        int ret_ww=write(fd_fw[0],"ping",ONEBETY);
        int ret_wr=read(fd_cw[1],buf,ONEBETY);
        close(fd_fw[1]);
        close(fd_cw[0]);
        if(-1)
        {
            printf("%d%d",ret_ww,ret_wr);
        }
        printf("<%d>:received %s",getpid(),buf);
    }
    else
    {
        int ret_cr=read(fd_fw[1],buf,ONEBETY);
        printf("<%d>:received %s",getpid(),buf);
        int ret_cw=write(fd_cw[0],"pong",ONEBETY);
        if(-1)
        {
            printf("%d%d",ret_cr,ret_cw);
        }
        close(fd_fw[0]);
        close(fd_cw[1]);
        exit(0);
    }
    exit(0);
    return 0;
}
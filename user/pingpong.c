#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define ONEBETY 1
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
    if(ret_w==-1)
    {
        printf("there a wrong:children");
        exit(1);
    }

    int pid=fork();
    if(pid>0){
        write(fd_fw[0],"ping",ONEBETY);
        read(fd_cw[1],buf,ONEBETY);
        printf("<%d>:received %s",getpid(),buf);
    }
    else
    {
        read(fd_fw[1],buf,ONEBETY);
        printf("<%d>:received %s",getpid(),buf);
        write(fd_cw[0],"pong",ONEBETY);
        exit(0);
    }
    exit(0);
    return 0;
}
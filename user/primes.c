#include "kernel/types.h"
#include "kernel/stat.h"
#include "user.h"

void primes(int p[]){
    int res;
    if(!read(p[0].&res,4)){
        exit(0);
    }
    fprintf(2,"prime %d\n",res);
    int c[2];
    pipe(c);
    if(fork()==0){
        close(p[0]);
        close(c[1]);
        primes(c);
    }else{
        close(c[0]);
        int tmp;
        while(read(p[0],tmp,4)){
            if(tmp%res!=0){
                write(c[1],&tmp,4);
            }
        }
        close(p[0]);
        close(c[1]);
        wait((int *)0);
    }
}




int main(int argc,char *argv[])
{
    int p[2];
    pipe(p);
    for(int i=2;i<=35;i++)
    {
        write(p[1],&i,4);
    }
    close(p[1]);
    primes(p);
    exit(0);
}
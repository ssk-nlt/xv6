/**
* 管道|的缺陷：
	注意：并不是所有的命令都支持管道运算，这是管道|的缺陷，但我们又要需要这样的传递功能时，从而我们就需要另外的xargs命令实现传递参数。
管道实现的是将前面的输出stdout作为后面的输入stdin，但是有些命令不接受管道的传递方式。例如：ls，这是为什么	呢？因为有些命令希望管道传递过来的是参数，但是直接使用管道有时无法传递到命令的参数位。这时候就需要xargs，	xargs实现的是将管道传递过来的stdin进行处理然后传递到命令的参数位置上。

xargs完成了两个行为：
1. 处理管道传输过来的stdin；
2. 将处理后的数据传递到正确的位置；
*/

#include "kernel/types.h"
#include "kernel/param.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

#define MSGSIZE 16

////$echo hello too | xargs echo bye
//int main(int argc,char *argv[]){
//    sleep(30);   //有时候点一个命令没有执行完，所以等待其输出到标准化输出
//
//    //Q1如何获取前一个命令的标准化输出（即此命令的标准化输入）呢？
//    char buf[MSGSIZE];
//    read(0,buf,MSGSIZE);
//
//    //Q2如何获取到自己的命令行参数
//    char *xargv[MAXARG];
//    int xargc=0;
//    for(int i=1;i<argc;++i){  //xargs echo bye
//        xargv[xargc]=argv[i];
//        xargc++;
//    }
//    char *p=buf;
//    for(int i=0;i<MSGSIZE;++i){
//        if(buf[i]=='\n'){
//            int pid=fork();
//            if(pid>0){
//                p=&buf[i+1];
//                wait(0);
//            }else{
//                //Q3如何使用exec取执行命令
//                buf[i]=0;// set '\0' to end of token
//                xargv[xargc]=p;
//                xargc++;
//                xargv[xargc]=0;// set nullptr in the end of array
//                xargc++;
//
//                exec(xargv[0],xargv);  //echo bye hello too
//                exit(0);
//            }
//        }
//    }
//    wait(0);
//    exit(0);
//}
int
main(int argc, char *argv[])
{
    char inputBuf[32]; // record the input from previous command
    char charBuf[320]; // buf for the char of all token
    char* charBufPointer = charBuf;
    int charBufSize = 0;

    char *commandToken[32]; // record the token from input spilt by space(' ')
    int tokenSize = argc - 1;// record token number(initial is argc - 1,because xargs will bot be execute)
    int inputSize = -1;

    // first copy initial argv argument to commandToken
    for(int tokenIdx=0;tokenIdx<tokenSize;tokenIdx++)
        commandToken[tokenIdx]=argv[tokenIdx+1];


    while((inputSize = read(0,inputBuf,sizeof(inputBuf))) > 0){
        for(int i = 0; i < inputSize;i++){
            char curChar = inputBuf[i];
            if(curChar == '\n'){// if read '\n', execute the command
                charBuf[charBufSize] = 0; // set '\0' to end of token
                commandToken[tokenSize++] = charBufPointer;
                commandToken[tokenSize] = 0; // set nullptr in the end of array

                if(fork()==0){ // open child to execute
                    exec(argv[1],commandToken);
                }
                wait(0);
                tokenSize = argc - 1;// initialize
                charBufSize = 0;
                charBufPointer = charBuf;
            }
            else if(curChar == ' '){
                charBuf[charBufSize++] = 0; // mark the end of string
                commandToken[tokenSize++] = charBufPointer;
                charBufPointer = charBuf+charBufSize; // change to the start of new string
            }else{
                charBuf[charBufSize++] = curChar;
            }

        }
    }
    exit(0);
}

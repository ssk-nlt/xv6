#include "kernel/types.h"
#include "user/user.h"
#include "kernel/stat.h"
#include "kernel/fs.h"

//struct dirent用于存储目录中的文件信息
//struct stat用来描述一个系统文件系统中的文件属性的结构。

/**
 * 查找具有特定名称的目录树中的所有文件（所有）。
 * find <path> <file name>
 * @param path:从该目录下查找
 * @param name：要查找的文件名
 * @return file name(without slash)
 */


char*
fmtname(char *path)
{
    static char buf[DIRSIZ+1];
    char *p;

    // Find first character after last slash.
    for(p=path+strlen(path); p >= path && *p != '/'; p--)
        ;
    p++;

    // Return blank-padded name.
    if(strlen(p) >= DIRSIZ)
        return p;
    memmove(buf, p, strlen(p));
    buf[strlen(p)] = 0;
    return buf;
}

void find(char *path,char *fileName){
    //printf("path:%s fileName:%s\n", fmtname(path),fileName);
    char buf[512], *p;
    int fd;
    struct dirent de;
    struct stat st;

    if((fd = open(path, 0)) < 0){
        fprintf(2, "find: cannot open %s\n", path);
        return;
    }

    if(fstat(fd, &st) < 0){
        fprintf(2, "find: cannot stat %s\n", path);
        close(fd);
        return;
    }

    switch(st.type){
        case T_FILE: // if path is a file,then compare their fileName
            if(strcmp(fmtname(path),fileName) == 0)
                printf("%s\n",path);
            break;

        case T_DIR:
            if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
                printf("find: path too long\n");
                break;
            }
            strcpy(buf, path);
            p = buf+strlen(buf);
            *p++ = '/'; // now buf contains a subdirectory
            while(read(fd, &de, sizeof(de)) == sizeof(de)){ //continue to read file from path
                if(de.inum == 0 || strcmp(de.name, ".")==0 || strcmp(de.name, "..")==0)
                    continue;
                memmove(p, de.name, DIRSIZ);
                p[DIRSIZ] = 0;
                if(stat(buf, &st) < 0){
                    printf("find: cannot stat %s\n", buf);
                    continue;
                }
                find(buf,fileName);
            }
            break;
    }
    close(fd);
}
int
main(int argc, char *argv[])
{
    if(argc != 3) {
        printf("usage: find <path> <file name>\n");
        exit(1);
    }
    find(argv[1],argv[2]);
    exit(0);
    return 0;
}
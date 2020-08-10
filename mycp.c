#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <time.h>
#include<fcntl.h>

int findLen(char *str){
    int len = 0;
    int i;

    for (i=0; str[i] != 0; i++)
    {
        len++;
    }
    return(len);
}

int main(int argc,char *argv[]){

    long ret = -1;
    long handle = 1;
    int src,dest,cnt;
    char buf[1024];

    if(argv[1] != NULL && argv[2]!= NULL){
        src=open(argv[1],O_RDONLY);
        if(src!=-1){
            dest=open(argv[2],O_WRONLY);
        } else{
            asm("syscall" : "=a" (ret) : "0"(1), "D"(handle), "S"("Source not valid"), "d"(findLen("Source not valid")) : "cc", "rcx", "r11", "memory");
            asm("syscall" : "=a" (ret) : "0"(1), "D"(handle), "S"("\n"), "d"(1) : "cc", "rcx", "r11", "memory");
            return 1;
        }
        if(dest==-1) {
            asm("syscall" : "=a" (ret) : "0"(1), "D"(handle), "S"("Dest not valid"), "d"(findLen("Dest not valid")) : "cc", "rcx", "r11", "memory");
            asm("syscall" : "=a" (ret) : "0"(1), "D"(handle), "S"("\n"), "d"(1) : "cc", "rcx", "r11", "memory");
            return 1;
        }


        while((cnt=read(src,buf,512))>0) {
            write(dest,buf,cnt);
        }

        close(src);
        close(dest);
    } else {
        asm("syscall" : "=a" (ret) : "0"(1), "D"(handle), "S"("Wrong input format"), "d"(findLen("Wrong input format")) : "cc", "rcx", "r11", "memory");
        asm("syscall" : "=a" (ret) : "0"(1), "D"(handle), "S"("\n"), "d"(1) : "cc", "rcx", "r11", "memory");
        return 1;
    }
    return 0;
}

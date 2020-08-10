#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <time.h>

int findLen(char *str){
    int len = 0;
    int i;

    for (i=0; str[i] != 0; i++)
    {
        len++;
    }
    return(len);
}

char * doCat(char *dest, const char *src){
    int i;
    int j;
    for (i = 0; dest[i] != '\0'; i++);
    for (j = 0; src[j] != '\0'; j++){
        dest[i+j] = src[j];
    }
    dest[i+j] = '\0';
    return dest;
}

void doCpy(char dest[], const char source[]){
    int i = 0;
    while (1)
    {
        dest[i] = source[i];

        if (dest[i] == '\0')
        {
            break;
        }

        i++;
    } 
}

void convertStr(int n, char s[]){

     int k = 0;
     int i, j;
     char c;

     do {
         s[k++] = n % 10 + '0';
     } while ((n /= 10) > 0);

     s[k] = '\0';

     for (i = 0, j = findLen(s)-1; i<j; i++, j--) {
          c = s[i];
          s[i] = s[j];
          s[j] = c;
     }
}

int printFile(int argc, char *argv, char* name){

    long ret = -1;
    long handle = 1;
    struct tm *t;
    char time[26];

    if(argc != 2)
        return 1;

    struct stat fileStat;
    if(stat(argv,&fileStat) < 0)
        return 1;

    asm("syscall" : "=a" (ret) : "0"(1), "D"(handle), "S"((S_ISDIR(fileStat.st_mode)) ? "d" : "-"), "d"(1) : "cc", "rcx", "r11", "memory");
    asm("syscall" : "=a" (ret) : "0"(1), "D"(handle), "S"((fileStat.st_mode & S_IRUSR) ? "r" : "-"), "d"(1) : "cc", "rcx", "r11", "memory");
    asm("syscall" : "=a" (ret) : "0"(1), "D"(handle), "S"((fileStat.st_mode & S_IWUSR) ? "w" : "-"), "d"(1) : "cc", "rcx", "r11", "memory");
    asm("syscall" : "=a" (ret) : "0"(1), "D"(handle), "S"((fileStat.st_mode & S_IXUSR) ? "x" : "-"), "d"(1) : "cc", "rcx", "r11", "memory");
    asm("syscall" : "=a" (ret) : "0"(1), "D"(handle), "S"((fileStat.st_mode & S_IRGRP) ? "r" : "-"), "d"(1) : "cc", "rcx", "r11", "memory");
    asm("syscall" : "=a" (ret) : "0"(1), "D"(handle), "S"((fileStat.st_mode & S_IWGRP) ? "w" : "-"), "d"(1) : "cc", "rcx", "r11", "memory");
    asm("syscall" : "=a" (ret) : "0"(1), "D"(handle), "S"((fileStat.st_mode & S_IXGRP) ? "x" : "-"), "d"(1) : "cc", "rcx", "r11", "memory");
    asm("syscall" : "=a" (ret) : "0"(1), "D"(handle), "S"((fileStat.st_mode & S_IROTH) ? "r" : "-"), "d"(1) : "cc", "rcx", "r11", "memory");
    asm("syscall" : "=a" (ret) : "0"(1), "D"(handle), "S"((fileStat.st_mode & S_IWOTH) ? "w" : "-"), "d"(1) : "cc", "rcx", "r11", "memory");
    asm("syscall" : "=a" (ret) : "0"(1), "D"(handle), "S"((fileStat.st_mode & S_IXOTH) ? "x" : "-"), "d"(1) : "cc", "rcx", "r11", "memory");
    asm("syscall" : "=a" (ret) : "0"(1), "D"(handle), "S"("\t"), "d"(1) : "cc", "rcx", "r11", "memory");

    char links[64];
    convertStr(fileStat.st_nlink,links);
    ret = write(handle, links,findLen(links));
    asm("syscall" : "=a" (ret) : "0"(1), "D"(handle), "S"("\t"), "d"(1) : "cc", "rcx", "r11", "memory");

    char uid[64];
    convertStr(fileStat.st_uid, uid);
    ret = write(handle, uid,findLen(uid));
    asm("syscall" : "=a" (ret) : "0"(1), "D"(handle), "S"("\t"), "d"(1) : "cc", "rcx", "r11", "memory");

    char gid[64];
    convertStr(fileStat.st_gid, gid);
    ret = write(handle, gid,findLen(gid));
    asm("syscall" : "=a" (ret) : "0"(1), "D"(handle), "S"("\t"), "d"(1) : "cc", "rcx", "r11", "memory");

    char size[64];
    convertStr(fileStat.st_size, size);
    ret = write(handle, size,findLen(size));
    asm("syscall" : "=a" (ret) : "0"(1), "D"(handle), "S"("\t"), "d"(1) : "cc", "rcx", "r11", "memory");

    t=localtime(&fileStat.st_mtime);
    strftime(time,sizeof(time),"%b %d %H:%M",t);
    asm("syscall" : "=a" (ret) : "0"(1), "D"(handle), "S"("\t"), "d"(1) : "cc", "rcx", "r11", "memory");

    ret = write(handle, time ,findLen(time));
    asm("syscall" : "=a" (ret) : "0"(1), "D"(handle), "S"("\t"), "d"(1) : "cc", "rcx", "r11", "memory");

    ret = write(handle, name ,findLen(name));
    asm("syscall" : "=a" (ret) : "0"(1), "D"(handle), "S"("\n"), "d"(1) : "cc", "rcx", "r11", "memory");

    return 0;
}

int main(int argc,char **argv){
    long ret = -1;
    long handle = 1;
    DIR *d;
    struct dirent *de;
    int i,j,n;
    if(argv[1] != NULL){
        d=opendir(argv[1]);
            if(d){
                while((de=readdir(d))!=NULL){
                    char base[100];
                    doCpy(base,argv[1]);
                    char* addDash = doCat(base,"/");
                    char* str = doCat(addDash,de->d_name);
                    printFile(2 ,str, de->d_name);
                }
            } else{
//                ret = write(handle, "Directory does not exist or wrong input format\n" ,findLen("Directory does not exist or wrong input format\n"));
                asm("syscall" : "=a" (ret) : "0"(1), "D"(handle), "S"("Directory does not exist or wrong input format\n"), "d"(findLen("Directory does not exist or wrong input format\n")) : "cc", "rcx", "r11", "memory");
            }
    }else{
//        ret = write(handle, "You must input a file or directory\n" ,findLen("You must input a file or directory\n"));
        asm("syscall" : "=a" (ret) : "0"(1), "D"(handle), "S"("You must input a file or directory\n"), "d"(findLen("You must input a file or directory\n")) : "cc", "rcx", "r11", "memory");

    }


}
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
int main(int argc, char const *argv[])
{
    pid_t pid = fork();
    
    if (pid < 0){
        printf("fork error\n");
        exit(1);
    } else if (pid == 0){
        count--;
        printf("in the child process count:%d\n", count);
    } else {
        sleep(1);
        printf ("in the parent process count:%d\n", count);
    }
    return 0;
}

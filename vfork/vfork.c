#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
int g_value = 2;
int main(int argc, char const *argv[])
{
    pid_t pid;
    int var = 5;
    printf("process id:%ld\n",(long)getpid());
    printf("g_value=%d, var=%d\n",g_value,var);
    pid = vfork();
    if (pid < 0){
        printf("vfork error\n");
    } else if (pid == 0){
        g_value--;
        var++;
        printf("the child process id:%ld\ng_value=%d, var=%d\n", (long)getpid(),g_value,var);
        _exit(0);
    } else {
        printf("the parent process id:%ld\ng_value=%d, var=%d\n", (long)getpid(),g_value, var);
        return 0;
    }
    return 0;
}

#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
extern char **__environ;
int main(int argc, char const *argv[])
{
    execve("new", argv, __environ);
    puts("正常情况下无法输出此信息!\n");
    return 0;
}


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
 
#define MAX_COMMAND_LENGTH 100
#define MAX_ARGUMENTS 10
#define MAX_PIPES 10
 
#define RED "\e[0;31m"
#define L_RED "\e[1;31m"
#define GREEN "\e[0;32m"
#define L_GREEN "\e[1;32m"
#define BLUE "\e[0;34m"   //正常设置
#define L_BLUE "\e[1;34m" //蓝色线条粗一些（0为正常，1为粗体）
#define END "\033[0m"
 
void handleInternalCommand(char *command);
void executeExternalCommand(char *command);
void executeCommandWithPipes(char *command);
 
char *inner_commmand[] = {"help", "exit", "cd"}; // 内部命令
char current_directory[MAX_COMMAND_LENGTH];      // 当前工作目录
char command[MAX_COMMAND_LENGTH];    //存放输入命令
 
 
//判断是否为内部命令
int check(char *command)
{
    for (int i = 0; i < 3; i++)
    {
        if (i == 2)
        {
            if (strncmp(command, "cd ", 3) == 0)
                return 1;
        }
 
        if (strcmp(command, inner_commmand[i]) == 0)
            return 1;
    }
    return 0;
}
 
// 输出命令行提示符
void print_prompt()
{
    printf("\e[1;34msztu202200202047\033[0m:\e[1;32m%s\033[0m$ ", current_directory);
}
 
void read_line()
{
    // 读取用户输入的命令
    if (fgets(command, sizeof(command), stdin) == NULL)
    {
        // 读取失败，退出程序
        exit(0);
    }
    // 删除末尾的换行符
    command[strcspn(command, "\n")] = '\0';
}
 
void parse_cmd()
{
    if (strlen(command) > 0)
    {
        if (check(command) == 1)
        {
            // 内部命令
            handleInternalCommand(command);
        }
        else if (strstr(command, "|") != NULL)
        {
            // 带有管道的命令
            executeCommandWithPipes(command);
        }
        else
        {
            // 外部命令
            executeExternalCommand(command);
        }
    }
}
 
// 内部命令的处理函数
void handleInternalCommand(char *command)
{
    if (strcmp(command, "help") == 0)
    {
        printf("这是一个简单的shell程序。\n");
        printf("支持的命令:\n");
        printf("  help - 显示帮助信息\n");
        printf("  exit - 退出myshell\n");
        printf("  cd [目录] - 改变当前工作目录\n");
        printf("  author:黄树坤\n");
    }
    else if (strcmp(command, "exit") == 0)
    {
        exit(0);
    }
    else if (strncmp(command, "cd ", 3) == 0)
    {
        // 获取目标目录
        char *targetDir = &command[3];
 
        // 改变当前工作目录
        if (chdir(targetDir) != 0)
        {
            printf("无法改变目录：%s\n", targetDir);
        }
        else
        {
            getcwd(current_directory, sizeof(current_directory)); // 更新当前工作目录
        }
    }
    else
    {
        printf("无效命令：%s\n", command);
    }
}
 
// 执行外部命令
void executeExternalCommand(char *command)
{
    pid_t pid = fork();
 
    if (pid < 0)
    {
        // fork() 出错
        printf("无法创建子进程。\n");
        return;
    }
    else if (pid == 0)
    {
        // 子进程
        char *args[MAX_ARGUMENTS];
 
        // 将命令分割成参数
        int argIndex = 0;
        char *token = strtok(command, " ");
        while (token != NULL && argIndex < MAX_ARGUMENTS - 1)
        {
            args[argIndex] = token;
            token = strtok(NULL, " ");
            argIndex++;
        }
        args[argIndex] = NULL;
 
        // 执行外部命令
        execvp(args[0], args);
 
        // execvp() 只在出错时返回
        printf("无效命令：%s\n", command);
        exit(0);
    }
    else
    {
        // 等待子进程结束
        int status;
        waitpid(pid, &status, 0);
    }
}
 
// 执行带有管道的命令
void executeCommandWithPipes(char *command)
{
    char *pipes[MAX_PIPES];
    int pipeCount = 0;
 
    // 将命令按照管道符号 "|" 分割成多个子命令
    char *token = strtok(command, "|");
    while (token != NULL && pipeCount < MAX_PIPES)
    {
        pipes[pipeCount] = token;
        token = strtok(NULL, "|");
        pipeCount++;
    }
 
    // 创建管道
    int pipefds[2 * pipeCount];
    for (int i = 0; i < pipeCount; i++)
    {
        if (pipe(pipefds + 2 * i) < 0)
        {
            perror("无法创建管道");
            exit(1);
        }
    }
 
    // 执行子命令
    for (int i = 0; i < pipeCount; i++)
    {
        pid_t pid = fork();
        if (pid < 0)
        {
            // fork() 出错
            perror("无法创建子进程");
            exit(1);
        }
        else if (pid == 0)
        {
            // 子进程
 
            // 如果不是第一个子命令，则将管道的输入重定向到上一个子命令的输出
            if (i > 0)
            {
                if (dup2(pipefds[2 * (i - 1)], STDIN_FILENO) < 0)
                {
                    perror("无法重定向输入");
                    exit(1);
                }
            }
 
            // 如果不是最后一个子命令，则将管道的输出重定向到下一个子命令的输入
            if (i < pipeCount - 1)
            {
                if (dup2(pipefds[2 * i + 1], STDOUT_FILENO) < 0)
                {
                    perror("无法重定向输出");
                    exit(1);
                }
            }
 
            // 关闭所有管道文件描述符
            for (int j = 0; j < 2 * pipeCount; j++)
            {
                close(pipefds[j]);
            }
 
            // 执行子命令
            executeExternalCommand(pipes[i]);
 
            exit(0);
        }
    }
 
    // 关闭所有管道文件描述符
    for (int i = 0; i < 2 * pipeCount; i++)
    {
        close(pipefds[i]);
    }
 
    // 等待所有子进程结束
    for (int i = 0; i < pipeCount; i++)
    {
        int status;
        wait(&status);
    }
}
 
int main()
{
    getcwd(current_directory, sizeof(current_directory)); // 获取当前工作目录
 
    while (1)
    {
        // 显示命令行提示符
        print_prompt();
 
        //读取命令
        read_line();
 
        // 处理命令
        parse_cmd();
    }
 
    return 0;
}
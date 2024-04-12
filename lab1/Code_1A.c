#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    int i;
    pid_t pid;

    for (i = 0; i < 10; i++) {
        pid = fork();
        if (pid < 0) {
            // Fork failed
            perror("fork");
            return 1;
        } else if (pid == 0) {
            // This is the child process
            printf("Child process: %d, parent process: %d\n", getpid(), getppid());
        } else {
            // This is the parent process
            wait(NULL);  // Wait for the child to finish
            sleep(5);
            break;
        }

    }
    
    return 0;
}

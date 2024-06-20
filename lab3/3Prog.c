#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define MB (1024 * 1024)

int main() {
    void* blocks[7];
    printf("pid: %d\n", getpid());
    for (int i = 0; i < 6; i++) {
        blocks[i] = malloc(128 * MB);
    }
    getchar();

    free(blocks[1]);
    free(blocks[2]);
    free(blocks[4]);
    getchar();

    blocks[6] = malloc(1024 * MB);
    blocks[6] = malloc(64 * MB);
    getchar();
    return 0;
}
#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
    int pid;
    int pipe1[2], pipe2[2];
    char buf[] = {'a'};
    pipe(pipe1);
    pipe(pipe2);

    int ret = fork();

    // parent sends in pipe1[1], child receives in pipe1[0]
    // child sends in pipe2[1], parent receives in pipe2[0]
    if (ret == 0) {
        // child
        pid = getpid();
        close(pipe1[1]);
        close(pipe2[0]);
        read(pipe1[0], buf, sizeof(buf));
        printf("%d: received ping\n", pid);
        write(pipe2[1], buf, sizeof(buf));
        exit(0);
    } else {
        // parent
        pid = getpid();
        close(pipe1[0]);
        close(pipe2[1]);
        write(pipe1[1], buf, sizeof(buf));
        read(pipe2[0], buf, sizeof(buf));
        printf("%d: received pong\n", pid);
        exit(0);
    }
}
#include "kernel/types.h"
#include "user/user.h"

void runprocess(int listenfd) {
    int cur_num = 0, left_num = 0, forked = 0;
    int pipes[2];   // read from pipes[0], write by pipes[1]

    // judge the nums one by one
    while (1) {
        int read_bytes = read(listenfd, &left_num, sizeof(left_num));

        // all nums were judged
        if (read_bytes == 0) {
            close(listenfd);
            if (forked) {
                // have a child
                close(pipes[1]);    // close the write side
                int child_pid;
                wait(&child_pid);            // waiting for its child
            }
            exit(0);
        }

        // get the prime num
        if (cur_num == 0) {
            cur_num = left_num;
            printf("prime %d\n", cur_num);
        }

        // we have to pass these nums
        if (left_num % cur_num != 0) {
            if (!forked) {
                // the first num to pass(as a prime)
                pipe(pipes);
                forked = 1;
                int ret = fork();
                if (ret == 0) {
                    // child
                    close(pipes[1]);    // line 39 and 40 are very important
                    close(listenfd);    // without them the program will trap in loop
                    runprocess(pipes[0]);
                } else {
                    // parent
                    close(pipes[0]);    // close the read side
                }
            }
            // pass the nums
            write(pipes[1], &left_num, sizeof(left_num));
        }
    }
}

int main (int argc, char* argv[]) {
    int pipes[2];
    pipe(pipes);
    for (int i = 2; i <= 35; i++) {
        write(pipes[1], &i, sizeof(i));
    }
    close(pipes[1]);
    runprocess(pipes[0]);
    exit(0);
}
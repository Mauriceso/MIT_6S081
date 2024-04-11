#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
    // what is argc
    // argv[1] is the time of sleeping; argv[0] is "sleep"?
    if (argc < 2) { // imitation rm.c
        fprintf(2, "Usage: sleep [ticks num]...\n");   // [ticks num]?
        exit(1);
    }
    // atoi sys call guarantees return an integer
    int ticks = atoi(argv[1]);
    int ret = sleep(ticks);
    exit(ret);
}
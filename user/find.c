#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"

void find(char *path, char* target)
{
    char buf[512], *p;
    int fd;
    struct dirent de;
    struct stat st;

    if ((fd = open(path, O_RDONLY)) < 0)
    {
        fprintf(2, "Usage: cannot open %s\n", path);
        return;
    }

    if (fstat(fd, &st) < 0)
    {
        fprintf(2, "Usage: cannot stat %s\n", path);
        close(fd);
        return;
    }

    // read the name of each file/floder under the folder specified by fd, which is $path, name is de.name
    while (read(fd, &de, sizeof(de)) == sizeof(de)) {
        // why should we come in ?
        strcpy(buf, path);
        p = buf + strlen(buf);  // p direct the end of buf(actually is path)
        *p++ = '/'; // add a '/' to the end of buf
        if (de.inum == 0)
            continue;
        // get the full path name of the current file/dir selected
        memmove(p, de.name, DIRSIZ);    // copy de.name to p
        p[DIRSIZ] = 0;  // set the flag_end of string p

        if (stat(buf, &st) < 0) {
            fprintf(2, "Usage: cannot stat %s\n\n", buf);
        }

        switch (st.type) {
        case T_FILE:
            if (strcmp(target, de.name) == 0) {
                printf("%s\n", buf);
            }
            break;
        case T_DIR:
            if (strcmp(de.name, ".") != 0 && strcmp(de.name, "..") != 0) {
                // de.name is neither "." or ".."
                find(buf, target);
            }
        }
    }
    close(fd);
    return;
}

int main(int argc, char *argv[])
{   // argc is the num of the shell line(eg: find x, argc is 2)
    if (argc != 3)
    {
        fprintf(2, "Usage: find [directory] [target filename]\n");  // what is it in []?
        exit(1);    // why 1?
    }
    find(argv[1], argv[2]);
    exit(0);
}
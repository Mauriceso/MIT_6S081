#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"

char *fmtname(char *path)
{
    static char buf[DIRSIZ + 1];
    char *p;

    // Find first character after last slash.Look backwards.
    for (p = path + strlen(path); p >= path && *p != '/'; p--)
        ;   // the last p is '/'
    p++;    // filename is stored in string p

    // Return blank-padded name.
    if (strlen(p) >= DIRSIZ)
        return p;   // string p is much bigger than a standard filename ? dir ?
    memmove(buf, p, strlen(p)); // copy p to buf
    memset(buf + strlen(p), ' ', DIRSIZ - strlen(p));   // make the end of the buf is blank-padded
    return buf;
}

void find(char *path, char* target)
{
    char buf[512], *p;
    int fd;
    struct dirent de;
    struct stat st;

    if ((fd = open(path, O_RDONLY)) < 0)
    {
        fprintf(2, "find: cannot open %s\n", path);
        return;
    }

    if (fstat(fd, &st) < 0)
    {
        fprintf(2, "find: cannot stat %s\n", path);
        close(fd);
        return;
    }

    switch (st.type)
    {
    case T_FILE:
        printf("T_FILE %s %d %d %l\n", fmtname(path), st.type, st.ino, st.size);
        break;

    case T_DIR:
        if (strlen(path) + 1 + DIRSIZ + 1 > sizeof buf)
        {
            printf("find: path too long\n");
            break;
        }
        strcpy(buf, path);
        p = buf + strlen(buf);
        *p++ = '/';
        while (read(fd, &de, sizeof(de)) == sizeof(de))
        {
            if (de.inum == 0)
                continue;
            memmove(p, de.name, DIRSIZ);
            p[DIRSIZ] = 0;
            if (stat(buf, &st) < 0)
            {
                printf("find: cannot stat %s\n", buf);
                continue;
            }
            printf("T_DIR %s %d %d %d\n", fmtname(buf), st.type, st.ino, st.size);
        }
        break;
    }
    close(fd);
}

int main(int argc, char *argv[])
{   // argc is the num of the shell line(eg: find x, argc is 2)
    printf("strchr will return: %c\n", *(strchr("ab/a", '/') + 1));
    /*if (argc != 3)
    {
        fprintf(2, "Usage: find [directory] [target filename]\n");  // what is it in []?
        exit(1);    // why 1?
    }
    find(argv[1], argv[2]);*/
    exit(0);
}

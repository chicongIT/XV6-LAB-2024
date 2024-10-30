#include "kernel/types.h"
#include "user/user.h"
#include "kernel/stat.h"
#include "kernel/fs.h"

void find(char *path, char *filename)
{
    char buf[512], *p;
    int fd;
    struct dirent de;
    struct stat st;

    if((fd = open(path, 0)) < 0)
    {
        printf("find: cannot open %s\n", path);
        return;
    }

    if(fstat(fd, &st) < 0)
    {
        printf("find: cannot stat %s\n", path);
        close(fd);
        return;
    }

    switch (st.type)
    {
    case T_FILE:
        if (strcmp(filename, path + strlen(path) - strlen(filename)) == 0)
            printf("%s\n", path);
        break;

    case T_DIR:
        if (strlen(path) + 1 + DIRSIZ + 1 > sizeof(buf))
        {
            printf("find: path too long\n");
            break;
        }

        strcpy(buf, path);
        p = buf + strlen(buf);
        *p++ = '/';

        // Đọc các file và thư mục trong thư mục hiện tại
        while (read(fd, &de, sizeof(de)) == sizeof(de))
        {
            if (de.inum == 0)
                continue;
            if (strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0)
                continue;

            memmove(p, de.name, DIRSIZ);
            p[DIRSIZ] = 0;

            // Gọi đệ quy nếu là thư mục con
            if (stat(buf, &st) < 0)
            {
                printf("find: cannot stat %s\n", buf);
                continue;
            }
            find(buf, filename);
        }
        break;
    }

    close(fd);
}



int main(int argc, char* argv[])
{
    if(argc < 3)
    {
        printf("Usage: find <path> <filename>\n");
        exit(1);
    }
    find(argv[1], argv[2]);
    exit(0);
}
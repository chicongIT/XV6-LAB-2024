#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define MAXARG 32
#define MAXLINE 512

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        fprintf(2, "Usage: xargs <command>\n");
        exit(1);
    }

    char buf[MAXLINE];
    char *cmd_argv[MAXARG];
    int i;

    // Sao chép lệnh gốc vào cmd_argv
    for (i = 0; i < argc - 1; i++)
    {
        cmd_argv[i] = argv[i + 1];
    }

    while (1)
    {
        int n = read(0, buf, sizeof(buf));
        if (n <= 0)
        {
            break; // Không còn đầu vào để đọc
        }

        // Tìm vị trí của newline và thay bằng NULL để kết thúc chuỗi
        if (buf[n - 1] == '\n')
        {
            buf[n - 1] = 0;
        }
        else
        {
            buf[n] = 0;
        }

        // Thêm tham số từ stdin vào cmd_argv
        cmd_argv[argc - 1] = buf;
        cmd_argv[argc] = 0;

        // Tạo tiến trình con để thực thi lệnh
        if (fork() == 0)
        {
            // Thực thi lệnh với đối số đã đọc
            exec(cmd_argv[0], cmd_argv);
            // Nếu exec thất bại
            fprintf(2, "exec %s failed\n", cmd_argv[0]);
            exit(1);
        }
        else
        {
            // Đợi tiến trình con kết thúc
            wait(0);
        }
    }

    exit(0);
}

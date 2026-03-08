#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

int main() {
    int fd;
    char *myfifo = "./my_fifo";
    char buffer[100];

    // Tạo FIFO nếu chưa có
    mkfifo(myfifo, 0666);

    // Mở FIFO để đọc
    fd = open(myfifo, O_RDONLY);

    while (1) {
        memset(buffer, 0, sizeof(buffer));

        int n = read(fd, buffer, sizeof(buffer));

        if (n > 0) {
            printf("Message moi: %s\n", buffer);
        }
    }

    close(fd);

    return 0;
}
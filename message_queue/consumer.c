#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int main() {
    int fd;
    char *fifo_path = "./my_fifo";
    char buffer[100];

    printf("Reader: Đang mở FIFO để đọc...\n");

    // 1. Mở FIFO để đọc
    fd = open(fifo_path, O_RDONLY);

    // 2. Đọc dữ liệu vào buffer
    read(fd, buffer, sizeof(buffer));

    printf("Reader: Đã nhận được: %s\n", buffer);

    close(fd);

    // 3. Xóa file FIFO sau khi dùng xong
    unlink(fifo_path);

    return 0;
}
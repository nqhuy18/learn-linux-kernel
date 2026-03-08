#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>

int main() {
    int fd;
    char *fifo_path = "./my_fifo";
    char *message = "Hello from Writer!";

    // 1. Tạo file FIFO (quyền 0666)
    mkfifo(fifo_path, 0666);

    printf("Writer: Đang chờ Reader kết nối...\n");

    // 2. Mở FIFO để ghi (Chương trình sẽ dừng ở đây cho đến khi Reader mở FIFO)
    fd = open(fifo_path, O_WRONLY);

    // 3. Ghi dữ liệu
    write(fd, message, strlen(message) + 1);
    printf("Writer: Đã gửi tin nhắn: %s\n", message);

    close(fd);
    return 0;
}
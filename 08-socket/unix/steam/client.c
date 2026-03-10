#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#define SOCKET_PATH "/tmp/unix_stream_example"

int main() {
    int sock_fd;
    struct sockaddr_un addr;
    char buffer[1024];

    // 1. Tạo Socket
    if ((sock_fd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
        perror("socket error");
        exit(EXIT_FAILURE);
    }

    // 2. Thiết lập địa chỉ đích (phải khớp với Server)
    memset(&addr, 0, sizeof(struct sockaddr_un));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SOCKET_PATH, sizeof(addr.sun_path) - 1);

    // 3. Connect: Thiết lập kết nối tới Server
    if (connect(sock_fd, (struct sockaddr *)&addr, sizeof(struct sockaddr_un)) == -1) {
        perror("connect error - Server có thể chưa chạy");
        exit(EXIT_FAILURE);
    }

    // 4. Gửi dữ liệu
    char *msg = "Hello from Client";
    write(sock_fd, msg, strlen(msg));

    // 5. Nhận phản hồi
    int bytes_read = read(sock_fd, buffer, sizeof(buffer) - 1);
    if (bytes_read > 0) {
        buffer[bytes_read] = '\0';
        printf("Client nhận phản hồi: %s\n", buffer);
    }

    // 6. Đóng socket
    close(sock_fd);

    return 0;
}
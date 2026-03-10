#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#define SOCKET_PATH "/tmp/unix_stream_example"
#define BUFFER_SIZE 1024

int main() {
    int server_fd, client_fd;
    struct sockaddr_un addr;
    char buffer[BUFFER_SIZE];

    // 1. Tạo Socket (AF_UNIX cho nội bộ, SOCK_STREAM cho luồng tin cậy)
    if ((server_fd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
        perror("socket error");
        exit(EXIT_FAILURE);
    }

    // Xóa file socket cũ nếu còn tồn tại để tránh lỗi Bind
    unlink(SOCKET_PATH);

    // 2. Thiết lập địa chỉ
    memset(&addr, 0, sizeof(struct sockaddr_un));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SOCKET_PATH, sizeof(addr.sun_path) - 1);

    // 3. Bind socket với địa chỉ file
    if (bind(server_fd, (struct sockaddr *)&addr, sizeof(struct sockaddr_un)) == -1) {
        perror("bind error");
        exit(EXIT_FAILURE);
    }

    // 4. Listen: Chuyển sang chế độ chờ kết nối (tối đa 5 kết nối trong hàng đợi)
    if (listen(server_fd, 5) == -1) {
        perror("listen error");
        exit(EXIT_FAILURE);
    }

    printf("Server đang chờ kết nối tại: %s\n", SOCKET_PATH);

    // 5. Accept: Chấp nhận kết nối từ Client (Hàm này sẽ chặn - block cho đến khi có client)
    if ((client_fd = accept(server_fd, NULL, NULL)) == -1) {
        perror("accept error");
        exit(EXIT_FAILURE);
    }

    // 6. Đọc/Ghi dữ liệu qua kết nối đã thiết lập
    int bytes_read = read(client_fd, buffer, sizeof(buffer) - 1);
    if (bytes_read > 0) {
        buffer[bytes_read] = '\0';
        printf("Server nhận được: %s\n", buffer);
        
        char *response = "Message received by Server";
        write(client_fd, response, strlen(response));
    }

    // 7. Dọn dẹp
    close(client_fd);
    close(server_fd);
    unlink(SOCKET_PATH);

    return 0;
}
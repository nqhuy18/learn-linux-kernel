#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#define SERVER_PATH "/tmp/udp_server_socket"
#define BUFFER_SIZE 1024

int main() {
    int server_fd;
    struct sockaddr_un server_addr, client_addr;
    char buffer[BUFFER_SIZE];
    socklen_t client_len;

    // 1. Tạo Socket Datagram
    server_fd = socket(AF_UNIX, SOCK_DGRAM, 0);
    
    // Dọn dẹp file cũ
    unlink(SERVER_PATH);

    memset(&server_addr, 0, sizeof(struct sockaddr_un));
    server_addr.sun_family = AF_UNIX;
    strncpy(server_addr.sun_path, SERVER_PATH, sizeof(server_addr.sun_path) - 1);

    // 2. Bind - Gắn socket vào địa chỉ để Client biết chỗ mà gửi
    bind(server_fd, (struct sockaddr *)&server_addr, sizeof(struct sockaddr_un));

    printf("Server Datagram đang đợi gói tin tại %s...\n", SERVER_PATH);

    while (1) {
        client_len = sizeof(struct sockaddr_un);
        
        // 3. Nhận dữ liệu và lấy luôn địa chỉ kẻ gửi (client_addr)
        int n = recvfrom(server_fd, buffer, BUFFER_SIZE, 0, 
                         (struct sockaddr *)&client_addr, &client_len);
        
        if (n > 0) {
            buffer[n] = '\0';
            printf("Server nhận từ %s: %s\n", client_addr.sun_path, buffer);

            // 4. Phản hồi lại đúng địa chỉ vừa gửi tới
            char *reply = "Server đã nhận gói tin!";
            sendto(server_fd, reply, strlen(reply), 0, 
                   (struct sockaddr *)&client_addr, client_len);
        }
    }

    close(server_fd);
    unlink(SERVER_PATH);
    return 0;
}
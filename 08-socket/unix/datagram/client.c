#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#define SERVER_PATH "/tmp/udp_server_socket"
#define CLIENT_PATH "/tmp/udp_client_socket_1" // Tên riêng của Client này

int main() {
    int client_fd;
    struct sockaddr_un server_addr, client_addr;
    char buffer[1024];

    client_fd = socket(AF_UNIX, SOCK_DGRAM, 0);

    // 1. Client tự Bind để Server biết đường mà gửi phản hồi về
    unlink(CLIENT_PATH);
    memset(&client_addr, 0, sizeof(struct sockaddr_un));
    client_addr.sun_family = AF_UNIX;
    strncpy(client_addr.sun_path, CLIENT_PATH, sizeof(client_addr.sun_path) - 1);
    bind(client_fd, (struct sockaddr *)&client_addr, sizeof(struct sockaddr_un));

    // 2. Thiết lập địa chỉ đích (Server)
    memset(&server_addr, 0, sizeof(struct sockaddr_un));
    server_addr.sun_family = AF_UNIX;
    strncpy(server_addr.sun_path, SERVER_PATH, sizeof(server_addr.sun_path) - 1);

    // 3. Gửi gói tin
    char *msg = "Gói tin từ Client A";
    sendto(client_fd, msg, strlen(msg), 0, 
           (struct sockaddr *)&server_addr, sizeof(struct sockaddr_un));

    // 4. Nhận phản hồi
    int n = recvfrom(client_fd, buffer, 1024, 0, NULL, NULL);
    buffer[n] = '\0';
    printf("Client nhận phản hồi: %s\n", buffer);

    close(client_fd);
    unlink(CLIENT_PATH);
    return 0;
}
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    // 1. Tạo file dữ liệu mẫu
    int fd_setup = open("test.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    write(fd_setup, "0123456789ABCDEFGHIJ", 20);
    close(fd_setup);

    // 2. Mở file để đọc (Bảng 1 có fd, Bảng 2 tạo 1 entry với Offset = 0)
    int fd = open("test.txt", O_RDONLY);
    if (fd < 0) { perror("open"); return 1; }

    char buf[11];
    buf[10] = '\0'; // Kết thúc chuỗi để in ra

    if (fork() == 0) {
        // --- TIẾN TRÌNH CON ---
        read(fd, buf, 5); // Đọc 10 byte đầu
        printf("Con đọc: %s\n", buf);
        // Sau khi đọc, Offset ở Bảng 2 đã bị con sửa thành 10
        close(fd);
        exit(0);
    } else {
        // --- TIẾN TRÌNH CHA ---
        wait(NULL); // Đợi con thực hiện xong cho rõ ràng

        read(fd, buf, 10); // Cha đọc tiếp 10 byte
        printf("Cha đọc: %s\n", buf);
        
        close(fd);
    }

    return 0;
}
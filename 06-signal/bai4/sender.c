#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Cách dùng: %s <PID_cua_receiver>\n", argv[0]);
        return 1;
    }

    // Chuyển PID từ chuỗi sang số nguyên
    pid_t receiver_pid = atoi(argv[1]);

    printf("[Sender] Đang gửi tín hiệu SIGUSR1 tới PID: %d...\n", receiver_pid);

    // Sử dụng hàm kill() để gửi signal
    if (kill(receiver_pid, SIGUSR1) == 0) {
        printf("[Sender] Gửi tín hiệu thành công!\n");
    } else {
        perror("[Sender] Lỗi khi gửi tín hiệu");
        return 1;
    }

    return 0;
}
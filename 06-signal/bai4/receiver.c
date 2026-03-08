#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

// Hàm xử lý khi nhận được tín hiệu
void handle_sigusr1(int sig) {
    printf("\n[Receiver] Đã nhận được tín hiệu SIGUSR1 (%d)!\n", sig);
    printf("[Receiver] Đang thức dậy và kết thúc chương trình...\n");
    exit(0); 
}

int main() {
    // In ra PID để chương trình Sender biết đường mà gửi
    printf("[Receiver] PID của tôi là: %d\n", getpid());
    printf("[Receiver] Đang chờ tín hiệu để thức dậy...\n");

    // Đăng ký hàm xử lý cho tín hiệu SIGUSR1
    signal(SIGUSR1, handle_sigusr1);

    // Vòng lặp vô tận (hoặc dùng pause() để tiết kiệm CPU)
    while(1) {
        pause(); // Tạm dừng tiến trình cho đến khi nhận được một tín hiệu
    }

    return 0;
}
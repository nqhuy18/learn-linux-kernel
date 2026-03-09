#include <stdio.h>
#include <signal.h>
#include <unistd.h>

int main() {
    sigset_t set;

    // Bước 1: Xóa sạch tập hợp (đảm bảo không có rác)
    sigemptyset(&set);

    // Bước 2: THÊM SIGINT vào tập hợp để chuẩn bị chặn
    sigaddset(&set, SIGINT);

    // Bước 3: Ra lệnh cho hệ thống chặn các tín hiệu có trong 'set'
    // SIG_BLOCK nghĩa là: "Thêm các tín hiệu này vào danh sách đang bị chặn của tiến trình"
    sigprocmask(SIG_BLOCK, &set, NULL);

    printf("Da chan Ctrl + C thành công!\n");
    printf("Ban co the thu nhan Ctrl + C thoai mai trong 10 giay toi...\n");

    for (int i = 5; i > 0; i--) {
        printf("Con %d giay...\n", i);
        sleep(1);
    }

    // Nếu muốn BỎ CHẶN (Unblock), lúc này mới dùng SIG_UNBLOCK
    sigprocmask(SIG_UNBLOCK, &set, NULL);
    printf("\nDa go bo lenh chan. Bay gio Ctrl + C se hoat dong tro lai.\n");
    
    sleep(5); 

    return 0;
}
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>

// Hàm bổ trợ để in danh sách các tín hiệu đang bị chặn
void print_signal_mask() {
    sigset_t current_mask;
    
    // Bước 1: Lấy signal mask hiện tại. 
    // Chúng ta dùng SIG_BLOCK với một set trống (NULL) 
    // để không thay đổi gì cả, chỉ lấy giá trị cũ vào 'current_mask'
    if (sigprocmask(0, NULL, &current_mask) == -1) {
        perror("Loi sigprocmask");
        return;
    }

    printf("--- Danh sach cac signal dang bi BLOCK: ---\n");
    
    int found = 0;
    // Bước 2: Duyệt qua các số hiệu tín hiệu phổ biến (thường từ 1 đến 31 hoặc 64)
    for (int i = 1; i < NSIG; i++) {
        if (sigismember(&current_mask, i)) {
            printf("Signal %d (%s) dang bi chan.\n", i, strsignal(i));
            found = 1;
        }
    }

    if (!found) {
        printf("(Khong co signal nao dang bi chan)\n");
    }
    printf("-------------------------------------------\n");
}

int main() {
    sigset_t set;

    // 1. In trạng thái ban đầu (thường là trống)
    print_signal_mask();

    // 2. Thử chặn một vài tín hiệu: Ctrl+C (SIGINT) và SIGUSR1
    printf("\nDang tien hanh chan SIGINT va SIGUSR1...\n");
    sigemptyset(&set);
    sigaddset(&set, SIGINT);
    sigaddset(&set, SIGUSR1);
    sigprocmask(SIG_BLOCK, &set, NULL);

    // 3. In lại để kiểm tra
    print_signal_mask();

    return 0;
}
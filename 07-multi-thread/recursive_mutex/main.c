#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

pthread_mutex_t my_mutex;

void action_B() {
    printf("Hàm B: Đang cố gắng lấy khóa...\n");
    pthread_mutex_lock(&my_mutex); // Sẽ bị TREO tại đây vì khóa đã bị action_A giữ
    printf("Hàm B: Đã lấy được khóa!\n");
    pthread_mutex_unlock(&my_mutex);
}

void action_A() {
    pthread_mutex_lock(&my_mutex);
    printf("Hàm A: Đã lấy khóa lần 1. Gọi hàm B...\n");
    
    action_B(); 
    
    pthread_mutex_unlock(&my_mutex);
}

int main() {
    pthread_mutex_init(&my_mutex, NULL); // Khởi tạo Plain Mutex mặc định
    
    action_A(); // Bắt đầu chạy
    
    printf("Chương trình kết thúc thành công!\n"); // Dòng này sẽ KHÔNG bao giờ in ra
    pthread_mutex_destroy(&my_mutex);
    return 0;
}
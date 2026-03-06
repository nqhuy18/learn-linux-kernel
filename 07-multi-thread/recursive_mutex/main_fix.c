#include <stdio.h>
#include <pthread.h>

pthread_mutex_t rec_mutex;

void action_B() {
    printf("Hàm B: Đang cố gắng lấy khóa...\n");
    pthread_mutex_lock(&rec_mutex); // THÀNH CÔNG: Vì hệ thống biết Luồng này đang giữ khóa
    printf("Hàm B: Đã lấy được khóa (Counter tăng)!\n");
    pthread_mutex_unlock(&rec_mutex); // Giải phóng 1 lớp (Counter giảm)
}

void action_A() {
    pthread_mutex_lock(&rec_mutex); // Khóa lần 1
    printf("Hàm A: Đã lấy khóa lần 1. Gọi hàm B...\n");
    
    action_B(); 
    
    printf("Hàm A: Quay lại từ hàm B.\n");
    pthread_mutex_unlock(&rec_mutex); // Giải phóng lớp cuối cùng
}

int main() {
    pthread_mutexattr_t attr;
    pthread_mutexattr_init(&attr);
    
    // THIẾT LẬP KIỂU ĐỆ QUY Ở ĐÂY
    pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
    pthread_mutex_init(&rec_mutex, &attr);

    action_A();

    printf("Chương trình kết thúc THÀNH CÔNG!\n");

    pthread_mutexattr_destroy(&attr);
    pthread_mutex_destroy(&rec_mutex);
    return 0;
}
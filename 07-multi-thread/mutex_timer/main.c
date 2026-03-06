#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

// Khởi tạo 2 Mutex thông thường (Plain Mutex)
pthread_mutex_t mtxA = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mtxB = PTHREAD_MUTEX_INITIALIZER;

void* thread1_func(void* arg) {
    printf("T1: Đang cố gắng lấy khóa A...\n");
    pthread_mutex_lock(&mtxA);
    printf("T1: Đã giữ A, đang đợi B...\n");
    
    // sleep(1) để đảm bảo T2 có đủ thời gian để chiếm mtxB trước khi T1 kịp chạm tới nó
    sleep(1); 
    
    printf("T1: Đang cố gắng lấy khóa B (Sẽ bị treo tại đây)...\n");
    pthread_mutex_lock(&mtxB); 
    
    // Đoạn code này sẽ không bao giờ được thực thi
    printf("T1: Đã lấy được cả A và B!\n");
    
    pthread_mutex_unlock(&mtxB);
    pthread_mutex_unlock(&mtxA);
    return NULL;
}

void* thread2_func(void* arg) {
    printf("T2: Đang cố gắng lấy khóa B...\n");
    pthread_mutex_lock(&mtxB);
    printf("T2: Đã giữ B, đang đợi A...\n");
    
    // sleep(1) để đảm bảo T1 có đủ thời gian để chiếm mtxA
    sleep(1);
    
    printf("T2: Đang cố gắng lấy khóa A (Sẽ bị treo tại đây)...\n");
    pthread_mutex_lock(&mtxA); 
    
    // Đoạn code này cũng không bao giờ được thực thi
    printf("T2: Đã lấy được cả B và A!\n");
    
    pthread_mutex_unlock(&mtxA);
    pthread_mutex_unlock(&mtxB);
    return NULL;
}

int main() {
    pthread_t t1, t2;

    printf("--- BẮT ĐẦU CHƯƠNG TRÌNH GÂY DEADLOCK ---\n");

    // Tạo 2 luồng chạy song song
    if (pthread_create(&t1, NULL, thread1_func, NULL) != 0) {
        perror("Không thể tạo luồng 1");
        return 1;
    }
    if (pthread_create(&t2, NULL, thread2_func, NULL) != 0) {
        perror("Không thể tạo luồng 2");
        return 1;
    }

    // Đợi 2 luồng kết thúc (Thực tế chương trình sẽ kẹt ở đây mãi mãi)
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    printf("--- CHƯƠNG TRÌNH KẾT THÚC (Dòng này sẽ không bao giờ hiện ra) ---\n");

    return 0;
}
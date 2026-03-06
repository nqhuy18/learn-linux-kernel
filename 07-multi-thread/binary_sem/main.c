#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

// Khai báo semaphore toàn cục
sem_t binary_sem;

// Thread B: Chờ tín hiệu (Receiver)
void* processing_thread(void* arg) {
    while(1) {
        printf("[Thread B] Đang đợi tín hiệu từ cảm biến...\n");
        
        // Chờ semaphore (giảm giá trị semaphore từ 1 xuống 0)
        // Nếu semaphore đang là 0, thread sẽ bị block (ngủ) tại đây
        sem_wait(&binary_sem);
        
        printf("[Thread B] ĐÃ NHẬN TÍN HIỆU! Bắt đầu xử lý báo cháy...\n");
        sleep(2); // Giả lập thời gian xử lý
        printf("[Thread B] Xử lý xong, quay lại trạng thái chờ.\n\n");
    }
    return NULL;
}

// Thread A: Phát tín hiệu (Sender)
void* sensor_thread(void* arg) {
    while(1) {
        sleep(5); // Giả lập 5 giây cảm biến quét một lần
        printf("[Thread A] PHÁT HIỆN SỰ KIỆN! Gửi tín hiệu cho Thread B...\n");
        
        // Giải phóng semaphore (tăng giá trị từ 0 lên 1)
        // Việc này sẽ đánh thức Thread B dậy
        sem_post(&binary_sem);
    }
    return NULL;
}

int main() {
    pthread_t thread1, thread2;

    // Khởi tạo semaphore nhị phân
    // Tham số 2: 0 (chỉ dùng giữa các thread trong 1 process)
    // Tham số 3: 0 (giá trị khởi tạo ban đầu là 0 - chưa có tín hiệu)
    if (sem_init(&binary_sem, 0, 0) != 0) {
        printf("Khởi tạo Semaphore thất bại!\n");
        return 1;
    }

    // Tạo 2 thread
    pthread_create(&thread2, NULL, processing_thread, NULL);
    pthread_create(&thread1, NULL, sensor_thread, NULL);

    // Chờ các thread kết thúc (trong ví dụ này chúng chạy vô hạn)
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    // Hủy semaphore khi không dùng nữa
    sem_destroy(&binary_sem);

    return 0;
}
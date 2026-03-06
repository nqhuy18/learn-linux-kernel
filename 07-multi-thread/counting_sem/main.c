#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdlib.h>

// Khởi tạo semaphore đếm
sem_t resource_sem;

void* task_worker(void* arg) {
    int id = *(int*)arg;

    printf("[Task %d] Đang đợi tài nguyên...\n", id);

    // Chờ (Wait) - Nếu giá trị semaphore > 0, giảm nó đi 1 và đi tiếp.
    // Nếu giá trị = 0, thread sẽ ngủ đợi đến khi có tài nguyên trống.
    sem_wait(&resource_sem);

    // Bắt đầu vùng xử lý tài nguyên giới hạn
    printf("  => [Task %d] ĐÃ LẤY ĐƯỢC TÀI NGUYÊN. Đang xử lý...\n", id);
    
    // Giả lập thời gian làm việc (mỗi task chiếm tài nguyên trong vài giây)
    sleep(rand() % 3 + 2); 

    printf("  <= [Task %d] Đã xong việc. GIẢI PHÓNG TÀI NGUYÊN.\n", id);
    
    // Giải phóng (Post) - Tăng giá trị semaphore lên 1, cho phép task khác vào.
    sem_post(&resource_sem);

    free(arg);
    return NULL;
}

int main() {
    pthread_t threads[6];
    
    // Khởi tạo semaphore với giá trị ban đầu là 3
    // (Cho phép tối đa 3 thread chạy qua sem_wait cùng lúc)
    sem_init(&resource_sem, 0, 3);

    printf("--- BẮT ĐẦU HỆ THỐNG (Tối đa 3 tài nguyên khả dụng) ---\n\n");

    for (int i = 0; i < 6; i++) {
        int* id = malloc(sizeof(int));
        *id = i + 1;
        pthread_create(&threads[i], NULL, task_worker, id);
        usleep(100000); // Tạo thread cách nhau 100ms cho dễ nhìn log
    }

    for (int i = 0; i < 6; i++) {
        pthread_join(threads[i], NULL);
    }

    sem_destroy(&resource_sem);
    printf("\n--- HỆ THỐNG KẾT THÚC ---\n");
    return 0;
}
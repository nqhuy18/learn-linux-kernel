#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <errno.h>

// Khởi tạo 2 Mutex thông thường
pthread_mutex_t mtxA = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mtxB = PTHREAD_MUTEX_INITIALIZER;

// Hàm tiện ích để thiết lập thời gian chờ (hiện tại + số giây)
void set_timeout(struct timespec *ts, int seconds) {
    clock_gettime(CLOCK_REALTIME, ts);
    ts->tv_sec += seconds;
}

void* thread1_func(void* arg) {
    struct timespec ts;
    
    while (1) {
        pthread_mutex_lock(&mtxA);
        sleep(1);
        printf("T1: Đã giữ A, đang thử lấy B trong 2 giây...\n");

        set_timeout(&ts, 2); // Chờ tối đa 2 giây

        // Thay vì lock() gây treo, ta dùng timedlock
        int res = pthread_mutex_timedlock(&mtxB, &ts);

        if (res == 0) {
            printf("T1: THÀNH CÔNG! Đã lấy được cả A và B.\n");
            // Xử lý công việc ở đây...
            pthread_mutex_unlock(&mtxB);
            pthread_mutex_unlock(&mtxA);
            break; // Thoát vòng lặp khi xong việc
        } else if (res == ETIMEDOUT) {
            printf("T1: Timeout! Nhả A để tránh Deadlock và thử lại sau...\n");
            pthread_mutex_unlock(&mtxA);
            usleep(100000); // Ngủ 100ms trước khi thử lại để tránh tranh chấp liên tục
        }
    }
    return NULL;
}

void* thread2_func(void* arg) {
    struct timespec ts;

    while (1) {
        pthread_mutex_lock(&mtxB);
        sleep(1);
        printf("T2: Đã giữ B, đang thử lấy A trong 2 giây...\n");

        set_timeout(&ts, 2);

        int res = pthread_mutex_timedlock(&mtxA, &ts);

        if (res == 0) {
            printf("T2: THÀNH CÔNG! Đã lấy được cả B và A.\n");
            // Xử lý công việc ở đây...
            pthread_mutex_unlock(&mtxA);
            pthread_mutex_unlock(&mtxB);
            break;
        } else if (res == ETIMEDOUT) {
            printf("T2: Timeout! Nhả B để tránh Deadlock và thử lại sau...\n");
            pthread_mutex_unlock(&mtxB);
            usleep(100000);
        }
    }
    return NULL;
}

int main() {
    pthread_t t1, t2;

    printf("--- BẮT ĐẦU CHƯƠNG TRÌNH VỚI TIMED MUTEX ---\n");

    pthread_create(&t1, NULL, thread1_func, NULL);
    pthread_create(&t2, NULL, thread2_func, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    printf("--- CHƯƠNG TRÌNH KẾT THÚC AN TOÀN ---\n");

    return 0;
}
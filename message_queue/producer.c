#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <unistd.h>


#define QUEUE_NAME  "/fire_robot_queue"
#define MAX_SIZE    1024

int main() {
    mqd_t mq;
    struct mq_attr attr;
    char buffer[MAX_SIZE];

    // Thiết lập thuộc tính hàng đợi
    attr.mq_flags = 0;
    attr.mq_maxmsg = 20;     // Tối đa 10 tin nhắn trong hàng
    attr.mq_msgsize = MAX_SIZE;
    attr.mq_curmsgs = 0;

    // Mở hoặc tạo mới hàng đợi
    mq = mq_open(QUEUE_NAME, O_CREAT | O_WRONLY, 0644, &attr);
    printf("mq = %d\n", mq);
    for (int i = 1; i <= 20; i++) {
        printf("Producer: Gửi lệnh %d (Pri: 1)\n", i);
        mq_send(mq, "WARNING: Low Battery", strlen("WARNING: Low Battery"), 11);  
    }

    mq_close(mq);
    printf("Thành công\n");
    return 0;
}
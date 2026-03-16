#include <linux/netlink.h>
#include <sys/socket.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define NETLINK_TESTFAMILY 25
#define NETLINK_MYGROUP 2

#define MAX_PAYLOAD 1024

struct msghdr msg;
struct iovec iov;

int main(int argc, char *argv[]) 
{
    int client_socket, len;
    struct nlmsghdr *nlh;
    struct sockaddr_nl addr;

    client_socket = socket(AF_NETLINK, SOCK_RAW, NETLINK_TESTFAMILY);

    if (client_socket == -1) {
        perror("socket");
        return -1;
    }

    memset(&addr, 0, sizeof(addr));
    addr.nl_family = AF_NETLINK;
    addr.nl_pid = getpid();
    addr.nl_groups = (1 << (NETLINK_MYGROUP - 1));

    nlh = (struct nlmsghdr *)malloc(NLMSG_SPACE(MAX_PAYLOAD));
    memset(nlh, 0, NLMSG_SPACE(MAX_PAYLOAD));

    // Kết nối iov với nlh
    memset(&iov, 0, sizeof(iov));
    iov.iov_base = (void *)nlh;
    iov.iov_len = NLMSG_SPACE(MAX_PAYLOAD);

    // Kết nối msg với iov
    memset(&msg, 0, sizeof(msg));
    msg.msg_name = (void *)&addr; 
    msg.msg_namelen = sizeof(addr);
    msg.msg_iov = &iov;
    msg.msg_iovlen = 1;

   if (bind(client_socket, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
    perror("Loi bind tai process"); // Dòng này sẽ hiện "Address already in use" nếu trùng PID
    exit(-1);
    }
    printf("Process %d dang ky Group %d THANH CONG!\n", getpid(), NETLINK_MYGROUP);
    printf("Process %d dang doi tin nhan tu Group %d...\n", getpid(), NETLINK_MYGROUP);
    while (1) {
        len = recvmsg(client_socket, &msg, 0); // Đợi tin nhắn tiếp theo

        if (len > 0) {
            printf("NHAN DUOC TU GROUP: %s\n", (char *)NLMSG_DATA(nlh));
        }
    }

    (void)close(client_socket);

    return 0;
}
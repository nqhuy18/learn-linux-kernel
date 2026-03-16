#include <linux/netlink.h>
#include <sys/socket.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define NETLINK_TESTFAMILY 25
#define MAX_PAYLOAD 1024

struct msghdr msg;

struct nlmsghdr *send_message(int client_socket, struct sockaddr_nl addr) 
{
    struct iovec iov;
    struct nlmsghdr *nlh = (struct nlmsghdr *)malloc(NLMSG_SPACE(MAX_PAYLOAD));

    memset(nlh, 0, NLMSG_SPACE(MAX_PAYLOAD));
    nlh->nlmsg_len = NLMSG_SPACE(MAX_PAYLOAD);
    nlh->nlmsg_pid = getpid();
    nlh->nlmsg_flags = 0;
    strcpy((char *)NLMSG_DATA(nlh), "Hello");

    memset(&iov, 0, sizeof(iov));
    iov.iov_base = (void *)nlh;
    iov.iov_len = nlh->nlmsg_len;

    memset(&msg, 0, sizeof(msg));
    msg.msg_name = (void *)&addr;
    msg.msg_namelen = sizeof(addr);
    msg.msg_iov = &iov;
    msg.msg_iovlen = 1;

    printf("Sending message to kernel\n");
    printf("-------------------------\n");
    sendmsg(client_socket, &msg, 0);
    printf("Sent message: %s\n\n", (char *)NLMSG_DATA(nlh));

    return nlh;
}

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
    addr.nl_pid = 0; // Gửi tới Kernel
    addr.nl_groups = 0; // Unicast

    nlh = send_message(client_socket, addr);

    len = recvmsg(client_socket, &msg, 0);

    if (len > 0) {
        printf("Receving the message from the kernel\n");
        printf("------------------------------------\n");
        printf("Received message: %s\n", (char *)NLMSG_DATA(nlh));
    } else {
        perror("recv");
        (void)close(client_socket);
    }

    (void)close(client_socket);

    return 0;
}
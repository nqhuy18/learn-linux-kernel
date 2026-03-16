#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/netlink.h>
#include <linux/kthread.h>
#include <linux/completion.h>
#include <net/netlink.h>
#include <net/net_namespace.h>
#include <linux/delay.h>

#define NETLINK_TESTFAMILY 25

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Linux_usr");
MODULE_DESCRIPTION("Netlink - Unicast");

struct sock *socket;
static struct completion comp;
static struct task_struct *thread1;

DECLARE_WAIT_QUEUE_HEAD(wq);

static void test_nl_receive_message(struct sk_buff *skb) 
{
    struct nlmsghdr *nlh = (struct nlmsghdr *)skb->data;
    pid_t pid = nlh->nlmsg_pid; // Lấy PID của User-space để gửi phản hồi

    int result;
    char *message;
    size_t message_size;
    struct sk_buff *skb_out;

    pr_info("Entering: %s\n", __func__);
    pr_info("kernel Received message: %s\n", (char *)nlmsg_data(nlh));

    message = "Hello from kernel unicast";
    message_size = strlen(message) + 1;

    // Cấp phát skb mới cho tin nhắn gửi đi
    skb_out = nlmsg_new(message_size, GFP_KERNEL);
    if (!skb_out) {
        pr_err("Failed to allocate a new skb\n");
        return;
    }

    // Đóng gói Netlink Header vào skb_out
    nlh = nlmsg_put(skb_out, 0, 0, NLMSG_DONE, message_size, 0);
    NETLINK_CB(skb_out).dst_group = 0; // Unicast nên group = 0
    strncpy(nlmsg_data(nlh), message, message_size);

    // Gửi Unicast về đúng PID của User-space
    result = nlmsg_unicast(socket, skb_out, pid);
    
    if (result < 0) {
        pr_err("Error while sending back to user\n");
    } else {
        pr_info("Sent message: %s\n", (char *)nlmsg_data(nlh));
    }

    wake_up_interruptible(&wq);
}

static int thread_fun1(void *data) 
{
    struct netlink_kernel_cfg config = {
        .input = test_nl_receive_message,
    };

    // Tạo Netlink Socket trong Kernel
    socket = netlink_kernel_create(&init_net, NETLINK_TESTFAMILY, &config);

    if (socket == NULL) {
        pr_err("Failed to create netlink socket\n");
        return -1;
    }

    pr_info("Netlink initialized\n");

    while (!kthread_should_stop()) {
        pr_info("Thread 1 is running\n");
        wake_up_interruptible(&wq);
        msleep(1000);
    }

    complete(&comp);
    return 0;
}

static int __init test_init(void) 
{
    pr_info("Driver Loaded\n");

    init_completion(&comp);

    thread1 = kthread_run(thread_fun1, NULL, "thread1");

    if (IS_ERR(thread1)) {
        pr_alert("Failed to create thread1\n");
        return PTR_ERR(thread1);
    }

    return 0;
}

static void __exit test_exit(void) 
{
    if (socket) {
        netlink_kernel_release(socket);
    }
    
    kthread_stop(thread1);
    wait_for_completion(&comp);
    
    pr_info("Netlink released\n");
}

module_init(test_init);
module_exit(test_exit);
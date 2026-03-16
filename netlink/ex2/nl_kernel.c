#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/netlink.h>
#include <linux/kthread.h>
#include <linux/completion.h>
#include <net/netlink.h>
#include <net/net_namespace.h>
#include <linux/delay.h>

#define NETLINK_TESTFAMILY 25
#define NETLINK_MYGROUP 2

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Linux_usr");
MODULE_DESCRIPTION("Netlink - Unicast");

struct sock *socket;
static struct completion comp;
static struct task_struct *thread1;

pid_t user_pid;
static void test_nl_receive_message(struct sk_buff *skb) 
{
    struct nlmsghdr *nlh = (struct nlmsghdr *)skb->data;
    user_pid = nlh->nlmsg_pid; // Lấy PID của User-space để gửi phản hồi
    pr_info("Kernel: Đã nhận diện User PID = %d\n", user_pid);
}

static int thread_fun1(void *data) 
{
    struct netlink_kernel_cfg config = {
        .input = test_nl_receive_message,
    };

    socket = netlink_kernel_create(&init_net, NETLINK_TESTFAMILY, &config);

    pr_info("Kernel thread starting loop...\n");

    while (!kthread_should_stop()) {
        // Chỉ gửi khi đã biết PID của User 
        if (user_pid != 0) {
            struct sk_buff *sk_out;
            struct nlmsghdr *nlh_out;
            char *msg = "KERNEL CHU DONG: Thong bao dinh ky!";
            int msg_size = strlen(msg) + 1;

            sk_out = nlmsg_new(msg_size, GFP_KERNEL);
            if (sk_out) {
                nlh_out = nlmsg_put(sk_out, 0, 0, NLMSG_DONE, msg_size, 0);
                strcpy(nlmsg_data(nlh_out), msg);
                
                // Gửi tin nhắn
                nlmsg_unicast(socket, sk_out, user_pid);
                pr_info("Kernel: Da gui tin toi PID %d\n", user_pid);
            }
        } else {
            pr_info("Kernel: Dang doi User dang ky PID...\n");
        }

        // Đợi 5 giây rồi lặp lại việc gửi
        msleep(5000); 
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
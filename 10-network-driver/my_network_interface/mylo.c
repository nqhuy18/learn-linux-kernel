#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/netdevice.h>
#include <linux/etherdevice.h>
#include <linux/ethtool.h>
#include <linux/init.h>
#include <linux/moduleparam.h>
#include <linux/rtnetlink.h>
#include <linux/net_tstamp.h>
#include <net/rtnetlink.h>
#include <linux/u64_stats_sync.h>

#define DRV_NAME	"mylo"
static int numdummies = 1;

static netdev_tx_t
fec_enet_start_xmit(struct sk_buff *skb, struct net_device *ndev)
{
	printk("fec_enet_start_xmit is called\n");
	dev_lstats_add(ndev, skb->len);

	skb_orphan(skb);
	skb->dev = ndev;
	skb->protocol = eth_type_trans(skb, ndev);	
	skb->ip_summed = CHECKSUM_UNNECESSARY;
	skb_clear_tstamp(skb);
	netif_rx(skb);
	return NETDEV_TX_OK;
}

static int mylo_dev_init(struct net_device *dev)
{
    printk("mylo_dev_init is called\n");
	dev->pcpu_stat_type = NETDEV_PCPU_STAT_LSTATS;

	netdev_lockdep_set_classes(dev);
	return 0;
}

static const struct net_device_ops fec_netdev_ops = {
	.ndo_init 		= mylo_dev_init,
	.ndo_start_xmit		= fec_enet_start_xmit,
};


static const struct ethtool_ops mylo_ethtool_ops = {
	.get_ts_info		= ethtool_op_get_ts_info,
};

static void mylo_setup(struct net_device *dev)
{
    printk("mylo_setup is called!\n");
	ether_setup(dev);

	/* Initialize the device structure. */
	dev->netdev_ops = &fec_netdev_ops;
	dev->ethtool_ops = &mylo_ethtool_ops;
	dev->needs_free_netdev = true;

	/* Fill in device structure with ethernet-generic values. */
	dev->flags |= IFF_NOARP;
	dev->flags &= ~IFF_MULTICAST;
	dev->priv_flags |= IFF_LIVE_ADDR_CHANGE | IFF_NO_QUEUE;
	dev->features	|= NETIF_F_SG | NETIF_F_FRAGLIST;
	dev->features	|= NETIF_F_GSO_SOFTWARE;
	dev->features	|= NETIF_F_HW_CSUM | NETIF_F_HIGHDMA;
	dev->features	|= NETIF_F_GSO_ENCAP_ALL;
	dev->hw_features |= dev->features;
	dev->hw_enc_features |= dev->features;
	eth_hw_addr_random(dev);

	dev->min_mtu = 0;
	dev->max_mtu = 0;
}

static int mylo_validate(struct nlattr *tb[], struct nlattr *data[],
			  struct netlink_ext_ack *extack)
{
    printk("mylo_validate is called!\n");

	if (tb[IFLA_ADDRESS]) {
		if (nla_len(tb[IFLA_ADDRESS]) != ETH_ALEN)
			return -EINVAL;
		if (!is_valid_ether_addr(nla_data(tb[IFLA_ADDRESS])))
			return -EADDRNOTAVAIL;
	}
	return 0;
}

static struct rtnl_link_ops mylo_link_ops __read_mostly = {
	.kind		= DRV_NAME,
	.setup		= mylo_setup,
	.validate	= mylo_validate,
};

/* Number of mylo devices to be set up by this module. */
module_param(numdummies, int, 0);
MODULE_PARM_DESC(numdummies, "Number of mylo pseudo devices");

static int __init mylo_init_one(void)
{
    printk("mylo_init_one is called!\n");

	struct net_device *dev_mylo;
	int err;

	dev_mylo = alloc_netdev(0, "mylo%d", NET_NAME_ENUM, mylo_setup);
	if (!dev_mylo)
		return -ENOMEM;

	dev_mylo->rtnl_link_ops = &mylo_link_ops;
	err = register_netdevice(dev_mylo);
	if (err < 0)
		goto err;
	return 0;

err:
	free_netdev(dev_mylo);
	return err;
}

static int __init mylo_init_module(void)
{
    printk("mylo_init_module is called!\n");

	int i, err = 0;

	err = rtnl_link_register(&mylo_link_ops);
	if (err < 0)
		return err;

	rtnl_lock();

	for (i = 0; i < numdummies && !err; i++) {
		err = mylo_init_one();
		cond_resched();
	}

	rtnl_unlock();

	if (err < 0)
		rtnl_link_unregister(&mylo_link_ops);

	return err;
}

static void __exit mylo_cleanup_module(void)
{
    printk("mylo_cleanup_module is called!\n");

	rtnl_link_unregister(&mylo_link_ops);
}

module_init(mylo_init_module);
module_exit(mylo_cleanup_module);
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("mylo netdevice driver which discards all packets sent to it");
MODULE_ALIAS_RTNL_LINK(DRV_NAME);
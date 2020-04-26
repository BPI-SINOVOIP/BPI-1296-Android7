#include <dev/class/netif.h>
#include <kernel/event.h>
#include <arch/ops.h>
#include <netif/etharp.h>
#include <lwip/netif.h>
#include <lwip/dhcp.h>
#include <debug.h>
#include <trace.h>
#include <assert.h>
#include <list.h>
#include <err.h>

#define LOCAL_TRACE 0

struct local_netif {
	struct netif netif;
	struct device *dev;
};

static event_t netif_up_event = EVENT_INITIAL_VALUE(netif_up_event, false, 0);
static volatile int netif_up_count = 0;

static err_t local_linkoutput(struct netif *netif, struct pbuf *p)
{
	LTRACE_ENTRY;

	struct local_netif *nif = containerof(netif, struct local_netif, netif);
	DEBUG_ASSERT(nif);

	status_t res = class_netif_output(nif->dev, p);

	LTRACE_EXIT;
	
	switch (res) {
		case NO_ERROR: return ERR_OK;
		case ERR_NO_MEMORY: return ERR_MEM;
		case ERR_TIMED_OUT: return ERR_TIMEOUT;
		default: return ERR_IF;
	}
}

static void local_netif_status(struct netif *netif)
{
	struct local_netif *nif = containerof(netif, struct local_netif, netif);
	DEBUG_ASSERT(nif);

	if (netif->flags & NETIF_FLAG_UP) {
		printf("netif %c%c ip %u.%u.%u.%u netmask %u.%u.%u.%u gw %u.%u.%u.%u\n",
				netif->name[0], netif->name[1],
				ip4_addr1_16(&netif->ip_addr),
				ip4_addr2_16(&netif->ip_addr),
				ip4_addr3_16(&netif->ip_addr),
				ip4_addr4_16(&netif->ip_addr),
				ip4_addr1_16(&netif->netmask),
				ip4_addr2_16(&netif->netmask),
				ip4_addr3_16(&netif->netmask),
				ip4_addr4_16(&netif->netmask),
				ip4_addr1_16(&netif->gw),
				ip4_addr2_16(&netif->gw),
				ip4_addr3_16(&netif->gw),
				ip4_addr4_16(&netif->gw));

		if (atomic_add(&netif_up_count, 1) >= 0)
			event_signal(&netif_up_event, true);
	} else {
		if (atomic_add(&netif_up_count, -1) == 1)
			event_unsignal(&netif_up_event);
	}
}

static err_t local_netif_init(struct netif *netif)
{
	LTRACE_ENTRY;

	struct local_netif *nif = containerof(netif, struct local_netif, netif);
	DEBUG_ASSERT(nif);
	
	netif->linkoutput = local_linkoutput;
	netif->output = etharp_output;

	netif->hwaddr_len = class_netif_get_hwaddr(nif->dev, netif->hwaddr, sizeof(netif->hwaddr));
	netif->mtu = class_netif_get_mtu(nif->dev);

	netif->name[0] = 'e';
	netif->name[1] = 'n';
	netif->num = 0;
	netif->flags |= NETIF_FLAG_BROADCAST | NETIF_FLAG_ETHARP;

	LTRACE_EXIT;

	return ERR_OK;
}

status_t class_netstack_wait_for_network(lk_time_t timeout)
{
	status_t res;

	LTRACE_ENTRY;

	res = event_wait_timeout(&netif_up_event, timeout);
	LTRACEF("res=%d\n", res);

	LTRACE_EXIT;
	return res;
}

status_t class_netif_add(struct device *dev)
{
	status_t err;
	ip_addr_t ipaddr, netmask, gw;

	struct local_netif *nif = malloc(sizeof(struct local_netif));
	if (!nif)
		return ERR_NO_MEMORY;
	
	nif->dev = dev;

	err = class_netif_set_state(dev, (struct netstack_state *) nif);
	if (err)
		goto done;
	
	IP4_ADDR(&gw, 0, 0, 0, 0);
	IP4_ADDR(&ipaddr, 0, 0, 0, 0);
	IP4_ADDR(&netmask, 255, 255, 255, 255);

	netif_add(&nif->netif, &ipaddr, &netmask, &gw, nif, local_netif_init, ethernet_input);
	netif_set_default(&nif->netif);
	netif_set_status_callback(&nif->netif, local_netif_status);
	#if LWIP_DHCP
	dhcp_start(&nif->netif);
	#endif

	err = NO_ERROR;

done:
	return err;
}

status_t class_netif_static_add(struct device *dev, ip_addr_t ipaddr, ip_addr_t netmask, ip_addr_t gw)
{
	status_t err;

	struct local_netif *nif = malloc(sizeof(struct local_netif));
	if (!nif)
		return ERR_NO_MEMORY;

	nif->dev = dev;

	err = class_netif_set_state(dev, (struct netstack_state *) nif);
	if (err)
		goto done;

	#if 0
	printf("\nnetif ipaddr ");
	ip_addr_print(&ipaddr);
	printf(", netmask ");
	ip_addr_print(&netmask);
	printf(", gw ");
	ip_addr_print(&gw);
	printf("\n");
	#endif

	netif_add(&nif->netif, &ipaddr, &netmask, &gw, nif, local_netif_init, ethernet_input);
	netif_set_default(&nif->netif);
	netif_set_status_callback(&nif->netif, local_netif_status);
	netif_set_up(&nif->netif);

	err = NO_ERROR;

	LTRACEF("netif flags 0x%08x, ip 0x%08x, netmask 0x%08x", nif->netif.flags, nif->netif.ip_addr.addr, nif->netif.netmask.addr);
done:
	return err;
}

status_t class_netstack_input(struct device *dev, struct netstack_state *state, struct pbuf *p)
{
	LTRACE_ENTRY;

	struct local_netif *nif = (struct local_netif *) state;
	if (!nif)
		return ERR_INVALID_ARGS;

	if (nif->netif.input(p, &nif->netif) != ERR_OK)
		pbuf_free(p);
	
	LTRACE_EXIT;

	return NO_ERROR;
}

status_t class_netstack_set_link_status(struct device *dev, struct netstack_state *state, bool up)
{
	LTRACE_ENTRY;

	struct local_netif *nif = (struct local_netif *) state;
	if (!nif)
		return ERR_INVALID_ARGS;

	if (up) {
		netif_set_link_up(&nif->netif);
	} else {
		netif_set_link_down(&nif->netif);
	}

	LTRACE_EXIT;

	return NO_ERROR;
}

status_t class_netif_set_addr(struct device *dev, struct netstack_state *state, ip_addr_t ipaddr, ip_addr_t netmask, ip_addr_t gw)
{
	struct local_netif *nif = (struct local_netif *) state;
	if (!nif)
		return ERR_INVALID_ARGS;

	#if 0
	printf("\nset ipaddr ");
	ip_addr_print(&ipaddr);
	printf(", netmask ");
	ip_addr_print(&netmask);
	printf(", gw ");
	ip_addr_print(&gw);
	printf("\n");
	#endif

	netif_set_addr(&nif->netif, &ipaddr, &netmask, &gw);

done:
	return NO_ERROR;
}


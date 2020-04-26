#ifndef __DEV_UDC_UTILS_H
#define __DEV_UDC_UTILS_H

#include <reg.h>
#include <kernel/thread.h>
#include <kernel/spinlock.h>
#include <debug.h>
#include <platform.h>

#define mdelay(x) spin(1000*x)

#define udelay(x) spin(x)

#define log_level 3 // 0: err, 1: warn, 2: notice 3: info, 4: debug, 5: verbose 6: dump

#define ERR(str, x...) \
do { printf("[%lld] ERROR! %s():%d: " str, current_time_hires(), \
	__func__, __LINE__, ## x); \
} while (0)

#define WARN(str, x...) \
do { if (log_level > 0) printf("[%lld] %s():%d: " str, current_time_hires(), \
	__func__, __LINE__, ## x); \
} while (0)

#define NOTICE(str, x...) \
do { if (log_level > 1) printf("[%lld] %s():%d: " str, current_time_hires(), \
	__func__, __LINE__, ## x); \
} while (0)

#define INF(str, x...) \
do { if (log_level > 2) { \
	thread_t *ct = get_current_thread(); \
	printf("[%lld][Thread %s@%p] %s():%d: " str, current_time_hires(), \
		    ct->name, ct, __func__, __LINE__, ## x); \
	} \
} while (0)

#define DBG(str, x...) \
do { if (log_level > 3) INF(str, ## x); \
} while (0)

#define VDBG(str, x...) \
do { if (log_level > 4) DBG(str, ## x); \
} while (0)

#define ENTER() \
    lk_bigtime_t start = current_time_hires(); \
	if (log_level > 5) VDBG("Enter .... at %s:%d\n", __FILE__, __LINE__); \
	do {} while(0)

#define EXIT() \
	if (log_level >5) VDBG("Exit take %lld usecs at %s:%d\n", \
					current_time_hires() - start, __FILE__, __LINE__); \
	do {} while(0)

#define TODO() \
	DBG("%s:%d %s() TODO  ######$$$$$$....\n", __FILE__, __LINE__, __func__); \
	do {} while(0)

#define TODO_FOR(x) \
	DBG("%s:%d %s() TODO for %s\n", __FILE__, __LINE__, __func__, x); \
	do {} while(0)

#define WARN_ON(x) \
	if (x) WARN("Warn !!! at %s:%d\n", __FILE__, __LINE__); \
	do {} while(0)


static inline void write_enable(u32 val, uint reg)
{
	writel(readl(reg) | val, reg);
}

static inline void write_disable(u32 val, uint reg)
{
	writel(readl(reg) & ~val, reg);
}

static inline void writesl(volatile uint addr, const void *buffer,
		unsigned int count)
{
	if (count) {
		const u32 *buf = buffer;

		do {
			writel(*buf++, addr);
		} while (--count);
	}
}

static inline void iowrite32_rep(volatile uint addr,
		const void *buffer,
		unsigned int count)
{
	writesl(addr, buffer, count);
}

static inline void readsl(const volatile uint addr, void *buffer,
		unsigned int count)
{
	if (count) {
		u32 *buf = buffer;

		do {
			u32 x = readl(addr);
			*buf++ = x;
		} while (--count);
	}
}

static inline void ioread32_rep(const volatile uint addr,
		void *buffer, unsigned int count)
{
	readsl(addr, buffer, count);
}

#define DIV_ROUND_UP(n,d) (((n) + (d) - 1) / (d))

#define le16_to_cpu(x) (x)
#define cpu_to_le16(x) (x)

#define local_irq_save(statep) arch_interrupt_save(&(statep), SPIN_LOCK_FLAG_INTERRUPTS)
#define local_irq_restore(statep) arch_interrupt_restore(statep, SPIN_LOCK_FLAG_INTERRUPTS)

#endif //__DEV_UDC_UTILS_H

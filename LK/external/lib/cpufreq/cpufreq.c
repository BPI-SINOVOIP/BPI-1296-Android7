#include <stdio.h>
#include <pmic.h>
#include <cpufreq.h>

#define _(_p) ((unsigned int *)_p)

static int cpu_set_max_freq(void)
{
	*_(0x98000500) = 0x0000000c;
	*_(0x98000504) = 0x000186d1; /* 1.4 GHz */
	*_(0x98000500) = 0x0000000d;

	*_(0x98000030) = ~0x00000180; /* set freq_div = 1 */

	return 0;
}

void cpu_set_max_capacity(void)
{
	int ret;

	ret = pmic_set_max_cpu_volt();
	if (ret) {
		printf("pmic_set_max_cpu_volt() returns %d\n", ret);
		return;
	}

	ret = cpu_set_max_freq();
	if (ret) {
		printf("cpu_set_max_freq() returns %d\n", ret);
		return;
	}
}

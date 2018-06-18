/*
 * soe
 * SGX Owner Epoch
 * Jun 18, 2018
 * root@davejingtian.org
 * http://davejingtian.org
 */
#include <linux/module.h>
#include <asm/msr.h>

#define MSR_SGX_OWNEREPOCH0	0x300
#define MSR_SGX_OWNEREPOCH1	0x301


static int __init soe_init(void)
{
	unsigned long long soe0 = 0;
	unsigned long long soe1 = 0;

	pr_info("soe: Entering: %s\n", __func__);

	rdmsrl(MSR_SGX_OWNEREPOCH0, soe0);
	rdmsrl(MSR_SGX_OWNEREPOCH1, soe1);

	pr_info("soe: epoch0 [0x%llu], epoch1 [0x%llu]\n",
		soe0, soe1);

	return 0;
}

static void __exit soe_exit(void)
{
	pr_info("exiting soe module\n");
}

module_init(soe_init);
module_exit(soe_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("soe module");
MODULE_AUTHOR("daveti");



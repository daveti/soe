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

	/*
	 * NOTE: the first MSR read may fault like blow:
[ 2857.245008] unchecked MSR access error: RDMSR from 0x300 at rIP: 0xffffffffa1c69db7 (native_read_msr+0x7/0x30)
[ 2857.245008] Call Trace:
[ 2857.245013]  soe_init+0x22/0x1000 [soe]
[ 2857.245016]  do_one_initcall+0x55/0x1a6
[ 2857.245019]  ? kmem_cache_alloc_trace+0x154/0x1b0
[ 2857.245022]  do_init_module+0x5f/0x209
[ 2857.245024]  load_module+0x1996/0x1da0
[ 2857.245027]  SYSC_finit_module+0xe5/0x120
[ 2857.245029]  ? SYSC_finit_module+0xe5/0x120
[ 2857.245032]  SyS_finit_module+0xe/0x10
[ 2857.245034]  entry_SYSCALL_64_fastpath+0x24/0xab
[ 2857.245035] RIP: 0033:0x7f3cebb3b4d9
[ 2857.245036] RSP: 002b:00007ffe3db85ca8 EFLAGS: 00000202 ORIG_RAX: 0000000000000139
[ 2857.245038] RAX: ffffffffffffffda RBX: 00007f3cebdfeb20 RCX: 00007f3cebb3b4d9
[ 2857.245039] RDX: 0000000000000000 RSI: 0000555a3400a26b RDI: 0000000000000003
[ 2857.245040] RBP: 0000000000001011 R08: 0000000000000000 R09: 00007f3cebe00ea0
[ 2857.245041] R10: 0000000000000003 R11: 0000000000000202 R12: 00007f3cebdfeb78
[ 2857.245041] R13: 00007f3cebdfeb78 R14: 000000000000270f R15: 00007f3cebdff1a8
	 * When this happens, make sure to write MSR at first before read.
	 * Although the write would not essentially work on SGXv1,
	 * somehow it makes MSR read work, which return all zeros on SGXv1
	 */
	rdmsrl(MSR_SGX_OWNEREPOCH0, soe0);
	rdmsrl(MSR_SGX_OWNEREPOCH1, soe1);
	pr_info("soe: epoch0 [0x%llu], epoch1 [0x%llu]\n",
		soe0, soe1);

	soe0 = 1;
	soe1 = 2;
	wrmsrl(MSR_SGX_OWNEREPOCH0, soe0);
	wrmsrl(MSR_SGX_OWNEREPOCH1, soe1);
	pr_info("soe: epoch0/1 updated if SGXv2\n");

	soe0 = 0;
	soe1 = 0;
	rdmsrl(MSR_SGX_OWNEREPOCH0, soe0);
	rdmsrl(MSR_SGX_OWNEREPOCH1, soe1);
	pr_info("soe: epoch0 [0x%llu], epoch1 [0x%llu]\n",
		soe0, soe1);
#
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



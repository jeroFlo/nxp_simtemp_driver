#include <linux/module.h>
#include "nxp_simtemp.h"



static int simtemp_init_module(void)
{
	printk("The module is now loaded\n");
	return 0;
}

static void simtemp_exit_module(void)
{
	printk("The module is now unloaded\n");
}

module_init(simtemp_init_module);
module_exit(simtemp_exit_module);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Jenifer Romero <jeniferalondraromeroflores@gmail.com>");
MODULE_DESCRIPTION("NXP Temperature device character driver");
MODULE_VERSION("1.0");

#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>      /* file operations structure */
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/poll.h>
#include <linux/cdev.h>
#include <linux/mm.h>
#include "nxp_simtemp.h"

#define DRIVER_NAME "simtemp"

static dev_t dev = 0;
static struct cdev std_cdev;
static struct class *dev_class;

static struct file_operations fops = 
{
	.owner   = THIS_MODULE,
	.read    = std_read,
	.write   = std_write,
	.open    = std_open,
	.release = std_release,
};

static int std_dev_uevent(struct device *dev, struct kobj_uevent_env *env) 
{
	add_uevent_var(env, "DEVMODE=%#o", 0666); 
	return 0;
}

static int std_open(struct inode *inode, struct file *file)
{
	printk("STD_OPEN");
	return 0;
}
static int std_release(struct inode *inode, struct file *file)
{
	printk("STD_RELEASE");
	return 0;
}
static ssize_t std_read(struct file *filp, char *buf, size_t nbytes, loff_t* ppos)
{
	printk("STD_READ");

	// Expected Output format: 2025-09-22T20:15:04.123Z temp=44.1C alert=0
	return 0;
}
static ssize_t std_write(struct file *filp, const char *buf, size_t nbytes, loff_t* ppos)
{
	printk("STD_WRITE");
	return nbytes;
}

/**
 * @brief Called when module is populated by insmod.
 * 
 * @return int expected zero value for a sucess loading.
 */
static int simtemp_init_module(void)
{
	printk("The module is now loaded\n");

	/*Allocating major number dynamically */
	if((alloc_chrdev_region(&dev, 0, 1, DRIVER_NAME)) < 0)
	{
			printk(KERN_ERR "Cannot allocate major number\n");
			return -1;
	}
	printk("Major = %d Minor = %d \n",MAJOR(dev), MINOR(dev));
	
	/*Creating cdev structure*/
	cdev_init(&std_cdev,&fops);

	/*Adding character device to the system*/
	if((cdev_add(&std_cdev, dev, 1)) < 0)
	{
		printk(KERN_ERR "Cannot add the device to the system\n");
		goto r_class;
	}
	/*Creating struct class*/
	if(IS_ERR(dev_class = class_create(DRIVER_NAME)))
	{
		printk(KERN_ERR "Cannot create the struct class\n");
		goto r_class;
	}
	// dev_class -> dev_uevent = std_dev_uevent; 
	/*Creating device*/
	if(IS_ERR(device_create(dev_class, NULL, dev, NULL, DRIVER_NAME)))
	{
		printk(KERN_ERR "Cannot create the Device 1\n");
		goto r_device;
	}
	printk("Device Driver Populated !!!\n");
	return 0;
	
r_device:
	class_destroy(dev_class);
r_class:
	unregister_chrdev_region(dev,1);
	return -1;
	
	
}

/**
 * @brief Called when module is removed by rmmod.
 * 
 */
static void simtemp_exit_module(void)
{
	device_destroy(dev_class,dev);
	class_destroy(dev_class);
	cdev_del(&std_cdev);
	unregister_chrdev_region(dev, 1);
	printk("The module is now unloaded\n");

}

module_init(simtemp_init_module);
module_exit(simtemp_exit_module);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Jenifer Romero <jeniferalondraromeroflores@gmail.com>");
MODULE_DESCRIPTION("NXP Temperature device character driver");
MODULE_VERSION("1.0");

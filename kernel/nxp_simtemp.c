#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>      /* file operations structure */
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/poll.h>
#include <linux/cdev.h>
#include <linux/mm.h>
#include <linux/timekeeping.h>
#include <linux/timer.h>
#include <linux/jiffies.h>
#include "nxp_simtemp.h"

#define DRIVER_NAME "simtemp"

static dev_t dev = 0;
static struct cdev std_cdev;
static struct class *dev_class;

static struct timer_list temp_timer;
static int temp_mC = 25000; // milli-degree Celsius (e.g., 44123 = 44.123 °C)
static int threshold_mC = 100000;
static uint sampling_ms = 1000;
DECLARE_WAIT_QUEUE_HEAD(simtemp_wq);
static int change_flag = 0;

// ----------------- Character driver ops --------------------

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
	char buffer[128];
    int ret;
    struct timespec64 ts;
    struct tm tm;

    if (*ppos != 0)
        return 0;

    // Block until change_flag is set (a write occurred) or temp change
    ret = wait_event_interruptible(simtemp_wq, change_flag != 0);
    if (ret)
        return -ERESTARTSYS;

    change_flag = 0;  // Reset after read

    // Get current real time and convert to broken-down time
    ktime_get_real_ts64(&ts);
    time64_to_tm(ts.tv_sec, 0, &tm);

    ret = snprintf(buffer, sizeof(buffer),
                   "%04ld-%02d-%02d %02d:%02d:%02d temp=%d threshold=%d\n",
                   tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday,
                   tm.tm_hour, tm.tm_min, tm.tm_sec,
                   temp_mC, threshold_mC);

    if (copy_to_user(buf, buffer, ret))
        return -EFAULT;

    *ppos = ret;
    return ret;
}
static ssize_t std_write(struct file *filp, const char *buf, size_t nbytes, loff_t* ppos)
{
	printk("STD_WRITE");
	return nbytes;
}

// ----------------- SYSFS --------------------


// ----------------- Temperature Sensor -------------------
static void temp_timer_callback(struct timer_list *t) {
    temp_mC++;

    if (temp_mC > threshold_mC)
        temp_mC = 25000;

    printk("simtemp: timer updated temp to %d\n", temp_mC);

    // Notify waiting readers
    change_flag = 1;
    wake_up_interruptible(&simtemp_wq);

    // Re-arm timer
    mod_timer(&temp_timer, jiffies + (HZ / 1000) * sampling_ms);
}


// ----------------- Module init and exit --------------------

/**
 * @brief Called when module is populated by insmod.
 * 
 * @return int expected zero value for a sucess loading.
 */
static int simtemp_init_module(void)
{
	printk("The module is now loaded\n");

	int ret;

	/*Allocating major number dynamically */
	ret = alloc_chrdev_region(&dev, 0, 1, DRIVER_NAME);
	if(ret)
	{
			printk(KERN_ERR "Cannot allocate major number\n");
			return ret;
	}
	printk("Major = %d Minor = %d \n",MAJOR(dev), MINOR(dev));
	
	/*Creating cdev structure*/
	cdev_init(&std_cdev,&fops);

	/*Adding character device to the system*/
	ret = cdev_add(&std_cdev, dev, 1);
	if(ret)
	{
		printk(KERN_ERR "Cannot add the device to the system\n");
		goto r_class;
	}
	/*Creating struct class*/
	dev_class = class_create(DRIVER_NAME);
	if(IS_ERR(dev_class))
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

	/*Initialize kernel timer */
	timer_setup(&temp_timer, temp_timer_callback, 0);
	mod_timer(&temp_timer, jiffies + (HZ / 1000) * sampling_ms);

	return 0;
	
r_device:
	class_destroy(dev_class);
r_class:
	unregister_chrdev_region(dev,1);
	return ret;
	
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
	del_timer_sync(&temp_timer);
	printk("The module is now unloaded\n");

}

module_init(simtemp_init_module);
module_exit(simtemp_exit_module);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Jenifer Romero <jeniferalondraromeroflores@gmail.com>");
MODULE_DESCRIPTION("NXP Temperature device character driver");
MODULE_VERSION("1.0");

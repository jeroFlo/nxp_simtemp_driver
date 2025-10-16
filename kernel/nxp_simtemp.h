

/* Function Prototypes */
static int simtemp_init_module(void);
static void simtemp_exit_module(void);

/* FUnction Prototypes for file operations*/
static int std_open(struct inode *, struct file *);
static int std_release(struct inode *, struct file *);
static ssize_t std_read(struct file *, char  *, size_t, loff_t *);
static ssize_t std_write(struct file *, const char *, size_t, loff_t *);


static int std_dev_uevent(struct device *, struct kobj_uevent_env *); 


static void temp_timer_callback(struct timer_list *);
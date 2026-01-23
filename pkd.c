#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h>    /* for put_user */

MODULE_LICENSE("GPL");
MODULE_AUTHOR("User");
MODULE_DESCRIPTION("A simple kernel module");
MODULE_VERSION("0.1");

/*  
 *  Prototypes - this would normally go in a .h file
 */
static int __init pkd_init(void);
static void __exit pkd_exit(void);

module_init(pkd_init);
module_exit(pkd_exit);

static int pkd_open(struct inode *, struct file *);
static int pkd_release(struct inode *, struct file *);
static ssize_t pkd_read(struct file *, char __user *, size_t, loff_t *);
//static ssize_t pkd_read(struct file *, char *, size_t, loff_t *);
ssize_t pkd_write(struct file *, const char __user *, size_t, loff_t *);
//static ssize_t pkd_write(struct file *, const char *, size_t, loff_t *);


#define SUCCESS 0
#define DEVICE_NAME "pkd"   /* Dev name as it appears in /proc/devices   */
#define BUF_LEN 80      /* Max length of the message from the device */

/* 
 * Global variables are declared as static, so are global within the file. 
 */

static int Major;       /* Major number assigned to our device driver */
static int Device_Open = 0; /* Is device open?  
                 * Used to prevent multiple access to device */
static char msg[BUF_LEN];   /* The msg the device will give when asked */
static char *msg_Ptr;

static struct file_operations fops = {
    .read = device_read,
    .write = device_write,
    .open = device_open,
    .release = device_release
};

/*
 * This function is called when the module is loaded
 */
static int __init pkd_init(void)
{
    Major = register_chrdev(0, DEVICE_NAME, &fops);

    if (Major < 0) {
      printk(KERN_ALERT "Registering char device failed with %d\n", Major);
      return Major;
    }

    printk(KERN_INFO "I was assigned major number %d. To talk to\n", Major);
    printk(KERN_INFO "the driver, create a dev file with\n");
    printk(KERN_INFO "'mknod /dev/%s c %d 0'.\n", DEVICE_NAME, Major);
    printk(KERN_INFO "Try various minor numbers. Try to cat and echo to\n");
    printk(KERN_INFO "the device file.\n");
    printk(KERN_INFO "Remove the device file and module when done.\n");

    return SUCCESS;
}

static void __exit pkd_exit(void)
{
    // free the major number we got while loading the module
    unregister_chrdev(Major, DEVICE_NAME);
    printk(KERN_WARNING "PKD: unregistered chrdev major=%d\n", Major);
}

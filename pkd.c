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

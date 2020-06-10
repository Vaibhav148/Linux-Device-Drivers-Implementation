#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include<linux/slab.h>                 //kmalloc()
#include<linux/uaccess.h>              //copy_to/from_user()


#define mem_size        1024

dev_t dev = 0;
static struct class *dev_class;
static struct cdev cdev;
uint8_t *kernel_buffer;

static int __init driver_init(void);
static void __exit driver_exit(void);
static int open(struct inode *inode, struct file *file);
static int release(struct inode *inode, struct file *file);
static ssize_t read(struct file *filp, char __user *buf, size_t len,loff_t * off);
static ssize_t write(struct file *filp, const char *buf, size_t len, loff_t * off);

static struct file_operations fops =
{
        .owner          = THIS_MODULE,
        .read           = read,
        .write          = write,
        .open           = open,
        .release        = release,
};

static int open(struct inode *inode, struct file *file)
{
        /*Creating Physical memory*/
        if((kernel_buffer = kmalloc(mem_size , GFP_KERNEL)) == 0){
            printk(KERN_INFO "char :Cannot allocate memory in kernel\n");
            return -1;
        }
        printk(KERN_INFO "char: Device File Opened...!!!\n");
        return 0;
}

static int release(struct inode *inode, struct file *file)
{
        kfree(kernel_buffer);
        printk(KERN_INFO "char :Device File Closed...!!!\n");
        return 0;
}

static ssize_t read(struct file *filp, char __user *buf, size_t len, loff_t *off)
{
        copy_to_user(buf, kernel_buffer, mem_size);
        printk(KERN_INFO "char :Data read : Done!\n");
        return mem_size;
}
static ssize_t write(struct file *filp, const char __user *buf, size_t len, loff_t *off)
{
        copy_from_user(kernel_buffer, buf, len);
        printk(KERN_INFO "char: Data write : Done!\n");
        return len;
}


static int __init driver_init(void)
{
        /*Allocating Major number*/
        if((alloc_chrdev_region(&dev, 0, 1, "char_device")) <0){
                printk(KERN_INFO "Cannot allocate major number\n");
                return -1;
        }
        printk(KERN_INFO "char: Major = %d Minor = %d \n",MAJOR(dev), MINOR(dev));

        /*Creating cdev structure*/
        cdev_init(&cdev,&fops);
        cdev.owner = THIS_MODULE;
        cdev.ops = &fops;

        /*Adding character device to the system*/
        if((cdev_add(&cdev,dev,1)) < 0){
            printk(KERN_INFO "Cannot add the device to the system\n");
            goto r_class;
        }

        /*Creating struct class*/
        if((dev_class = class_create(THIS_MODULE,"char_class")) == NULL){
            printk(KERN_INFO "Cannot create the struct class\n");
            goto r_class;
        }

        /*Creating device*/
        if((device_create(dev_class,NULL,dev,NULL,"char_device")) == NULL){
            printk(KERN_INFO "Cannot create the Device 1\n");
            goto r_device;
        }
        printk(KERN_INFO "char: Device Driver Insert...Done!!!\n");
	return 0;

r_device:
        class_destroy(dev_class);
r_class:
        unregister_chrdev_region(dev,1);
        return -1;
}

void __exit driver_exit(void)
{
        device_destroy(dev_class,dev);
        class_destroy(dev_class);
        cdev_del(&cdev);
        unregister_chrdev_region(dev, 1);
	printk(KERN_INFO "char: Device Driver Remove...Done!!!\n");
}

module_init(driver_init);
module_exit(driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Vaibhav");
MODULE_DESCRIPTION("char device driver");
MODULE_VERSION("1.0");


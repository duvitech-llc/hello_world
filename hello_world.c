#include <linux/module.h>   // Needed by all modules
#include <linux/kernel.h>   // Needed for KERN_INFO
#include <linux/init.h>     // Needed for the macros
#include <linux/fs.h>

#define MYMAJOR 77

// Module information
MODULE_LICENSE("GPL");
MODULE_AUTHOR("George Vigelette");
MODULE_DESCRIPTION("A Simple Hello World Kernel Module using device numbers and files auto connect");
MODULE_VERSION("1.0");

// funcitons for open and closing file
static int driver_open(struct inode *device_file, struct file *instance)
{
    printk(KERN_INFO "dev_nr - open was called\n");
    return 0;    
};


static int driver_close(struct inode *device_file, struct file *instance)
{
    printk(KERN_INFO "dev_nr - close was called\n");
    return 0;    
};


static struct file_operations fops = {
    .owner = THIS_MODULE,
    .open = driver_open,
    .release = driver_close
};


// Function executed when the module is loaded
static int __init hello_init(void)
{
    int retval;
    printk(KERN_INFO "Hello, kernel!\n");
    // request device nr
    retval = register_chrdev(MYMAJOR, "hello_world", &fops);
    if(retval == 0){
        printk(KERN_INFO "dev_nr - registered Major: %d Minor: %d\n", MYMAJOR, 0);
    }else if(retval > 0) {
        printk(KERN_INFO "dev_nr - registered Major: %d Minor: %d\n", retval>>20, retval& 0xFFFFF);
    }else{
        printk(KERN_ERR "could not register device number!\n");
        return -1;
    }

    return 0;
}

// Function executed when the module is unloaded
static void __exit hello_exit(void)
{
    unregister_chrdev(MYMAJOR, "hello_world");
    printk(KERN_INFO "Goodbye, world!\n");
}

// Registering the init and exit functions
module_init(hello_init);
module_exit(hello_exit);

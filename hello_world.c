#include <linux/module.h>   // Needed by all modules
#include <linux/kernel.h>   // Needed for KERN_INFO
#include <linux/init.h>     // Needed for the macros
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>


#define MYMAJOR 77

// Module information
MODULE_LICENSE("GPL");
MODULE_AUTHOR("George Vigelette");
MODULE_DESCRIPTION("A Simple Hello World Kernel Module using device numbers and files auto connect");
MODULE_VERSION("1.0");

// buffer for data
static char buffer[255];
static int buffer_pointer;

// variables for device and device class
static dev_t my_device_nr;
static struct class *my_class;
static struct cdev my_device;

#define DRIVER_NAME  "hello_world"
#define DRIVER_CLASS  "HelloWorldClass"

static ssize_t driver_read(struct  file *File, char *user_buffer, size_t count, loff_t * offs) {
  int to_copy, not_copied, delta;
  
  // get amount of data to copy
  to_copy = min(count, buffer_pointer);

  // copy data to user cant just use memcpy need to use kernel mem
  not_copied = copy_to_user(user_buffer, buffer, to_copy);

  // calculate data
  delta = to_copy - not_copied;
  return delta;
};


static ssize_t driver_write(struct  file *File, const char *user_buffer, size_t count, loff_t * offs) {
  int to_copy, not_copied, delta;
  
  // get amount of data to copy
  to_copy = min(count, sizeof(buffer)); // only can copy size of our buffer

  // copy data to user cant just use memcpy need to use kernel mem
  not_copied = copy_from_user(buffer, user_buffer, to_copy);
  buffer_pointer = to_copy;

  // calculate data
  delta = to_copy - not_copied;
  return delta;
};

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
    .release = driver_close,
    .read = driver_read,
    .write = driver_write
};

// Function executed when the module is loaded
static int __init hello_init(void)
{
    int retval;
    printk(KERN_INFO "Hello, kernel!\n");
    
	/* Allocate a device nr */
	if( alloc_chrdev_region(&my_device_nr, 0, 1, DRIVER_NAME) < 0) {
		printk("Device Nr. could not be allocated!\n");
		return -1;
	}

    printk(KERN_INFO "MAJOR: %d MINOR: %d\n", MAJOR(my_device_nr), MINOR(my_device_nr));

 
	/* Create device class */
	if((my_class = class_create(THIS_MODULE, DRIVER_CLASS)) == NULL) {
		printk("Device class can not be created!\n");
		goto ClassError;
	}

	/* create device file */
	if(device_create(my_class, NULL, my_device_nr, NULL, DRIVER_NAME) == NULL) {
		printk("Can not create device file!\n");
		goto FileError;
	}

	/* Initialize device file */
	cdev_init(&my_device, &fops);

	/* Regisering device to kernel */
	if(cdev_add(&my_device, my_device_nr, 1) == -1) {
		printk("Registering of device to kernel failed!\n");
		goto AddError;
	}

    return 0;
AddError:
    device_destroy(my_class, my_device_nr);
FileError:
    class_destroy(my_class);
ClassError:
	unregister_chrdev_region(my_device_nr, 1);
    return -1;
}

// Function executed when the module is unloaded
static void __exit hello_exit(void)
{
    cdev_del(&my_device);
    device_destroy(my_class, my_device_nr);
    class_destroy(my_class);
	unregister_chrdev_region(my_device_nr, 1);
    printk(KERN_INFO "Goodbye, world!\n");
}

// Registering the init and exit functions
module_init(hello_init);
module_exit(hello_exit);

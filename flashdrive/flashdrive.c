#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/usb.h>
#include<linux/uaccess.h>

#define MIN(a,b) (((a)<= (b)) ? (a):(b))
#define BULK_EP_OUT 0x01
#define BULK_EP_IN 0x81
#define MAX_PKT_SIZE 512


static struct usb_device *device;
static struct usb_class_driver class;
static unsigned char bulk_buf[MAX_PKT_SIZE];


static int flash_open(struct inode *i, struct file *f)
{
	
	printk(KERN_INFO "Hello you opened me");


return 0 ;
}


static int flash_close(struct inode *i, struct file *f)
{
	return 0 ;
}

//func to read data from flash drive
static ssize_t flash_read(struct file *f, char __user *buf, size_t cnt, loff_t *off)
{
	int retval;
	int read_cnt;
	
	//reading data from the bulk end point
	retval = usb_bulk_msg(device,usb_rcvbulkpipe(device, BULK_EP_IN), bulk_buf, MAX_PKT_SIZE, &read_cnt,5000);
	if(retval)
	{
		printk(KERN_ERR "Bulk message returned %d\n",retval);
		return retval;
	}
	if(copy_to_user(buf,bulk_buf,MIN(cnt,read_cnt)))
	{
		return -EFAULT;

	}
	
	return MIN(cnt, read_cnt);
}


//func to write data to flash drive
static ssize_t flash_write(struct file *f, const char __user *buf, size_t cnt, loff_t *off)
{
	int retval;
	int wrote_cnt = MIN(cnt,MAX_PKT_SIZE);
	
	if(copy_from_user(bulk_buf,buf,MIN(cnt,MAX_PKT_SIZE)))
	{
		return -EFAULT;

	}


	//writing data into  the bulk end point
	retval = usb_bulk_msg(device,usb_sndbulkpipe(device, BULK_EP_OUT), bulk_buf,MIN(cnt,MAX_PKT_SIZE), &wrote_cnt,5000);
	if(retval)
	{
		printk(KERN_ERR "Bulk message returned %d\n",retval);
		return retval;
	}
	
	
	return wrote_cnt;
}

//STRUCT FOR FILE OPERATIONS 
static struct file_operations fops=
{
	.open = flash_open,
	.release = flash_close,
	.read = flash_read,
	.write = flash_write,
}; 




//PROBE FUNCTION 
static int flash_probe(struct usb_interface *interface, const struct usb_device_id *id)
{
	//struct usb_host_interface *iface_desc;
	//struct usb_endpoint_descriptor *endpoint;
	//int i;
	int retval;

/*	iface_desc = interface->cur_altsetting;
	
	printk(KERN_INFO "HEY THATS MY THUMB DRIVE PROBED %d :::> (%04X:%04X)",iface_desc->desc.bInterfaceNumber, id->idVendor, id->idProduct);
	
	printk(KERN_INFO "Class of the thumb drive is  %02X\n ", iface_desc->desc.bInterfaceClass);
	printk(KERN_INFO "Number of endpoints are :  %02X\n", iface_desc->desc.bNumEndpoints);
	
	for(i = 0 ;i<iface_desc->desc.bNumEndpoints; i++)
	{
		endpoint = &iface_desc->endpoint[i].desc;
		printk(KERN_INFO "ED %d ->bENDPOINTADDRESS: 0x%02X\n",i,endpoint->bEndpointAddress);
		printk(KERN_INFO "ED %d ->bmAttributes: 0x%02X\n",i,endpoint->bmAttributes);
		printk(KERN_INFO "ED %d ->wMAXPACKETSIZE: 0x%04X(%d)\n",i,endpoint->wMaxPacketSize,endpoint->wMaxPacketSize);
	}
*/
	device = interface_to_usbdev(interface);
	class.name = "usb/pen%d";
	class.fops = &fops;

	if((retval = usb_register_dev(interface,&class))<0)
	{
		printk(KERN_ERR "Not able to get minor for this driver.");
	}
	else 
	{
		printk(KERN_INFO "Minor number obtained: %d\n",interface->minor);
	}
return retval;

}	





//DISCONNECT FUNCTION
static void flash_disconnect(struct usb_interface *interface)
{
	usb_deregister_dev(interface, &class);
	printk(KERN_INFO "FLash %d now disconnecting \n",interface->cur_altsetting->desc.bInterfaceNumber);
}






//FLASH TABLE STRUCT
static struct usb_device_id flash_table[]= 
{
	{USB_DEVICE(0Xabcd,0x1234)},
	{}
};
MODULE_DEVICE_TABLE (usb, flash_table);

//FLASH DRIVER STRUCT
static struct usb_driver flash_driver = {
	.name = "flash_driver",
	.id_table = flash_table,
	.probe = flash_probe,
	.disconnect = flash_disconnect,
};



static int __init flash_init(void)
{
	
	int result;
	if((result = usb_register(&flash_driver)))
		printk(KERN_ERR"usb_register failed. Error number %d", result);

	return result;
}


static void __exit flash_exit(void)
{
	usb_deregister(&flash_driver);

}


module_init(flash_init);
module_exit(flash_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Vaibhav");


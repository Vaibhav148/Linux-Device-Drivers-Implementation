#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/usb.h>
 

#define IS_NEW_METHOD_USED  ( 1 )
 
 
#define USB_VENDOR_ID       ( 0x22d9 )      //USB device's vendor ID
#define USB_PRODUCT_ID      ( 0x2764 )      //USB device's product ID
 
 
#define PRINT_USB_INTERFACE_DESCRIPTOR( i )                         \
{                                                                   \
    pr_info("USB_INTERFACE_DESCRIPTOR:\n");                         \
    pr_info("-----------------------------\n");                     \
    pr_info("bLength: 0x%x\n", i.bLength);                          \
    pr_info("bDescriptorType: 0x%x\n", i.bDescriptorType);          \
    pr_info("bInterfaceNumber: 0x%x\n", i.bInterfaceNumber);        \
    pr_info("bAlternateSetting: 0x%x\n", i.bAlternateSetting);      \
    pr_info("bNumEndpoints: 0x%x\n", i.bNumEndpoints);              \
    pr_info("bInterfaceClass: 0x%x\n", i.bInterfaceClass);          \
    pr_info("bInterfaceSubClass: 0x%x\n", i.bInterfaceSubClass);    \
    pr_info("bInterfaceProtocol: 0x%x\n", i.bInterfaceProtocol);    \
    pr_info("iInterface: 0x%x\n", i.iInterface);                    \
    pr_info("\n");                                                  \
}
 
#define PRINT_USB_ENDPOINT_DESCRIPTOR( e )                          \
{                                                                   \
    pr_info("USB_ENDPOINT_DESCRIPTOR:\n");                          \
    pr_info("------------------------\n");                          \
    pr_info("bLength: 0x%x\n", e.bLength);                          \
    pr_info("bDescriptorType: 0x%x\n", e.bDescriptorType);          \
    pr_info("bEndPointAddress: 0x%x\n", e.bEndpointAddress);        \
    pr_info("bmAttributes: 0x%x\n", e.bmAttributes);                \
    pr_info("wMaxPacketSize: 0x%x\n", e.wMaxPacketSize);            \
    pr_info("bInterval: 0x%x\n", e.bInterval);                      \
    pr_info("\n");                                                  \
}
 

static int probe(struct usb_interface *interface,
                        const struct usb_device_id *id)
{
    unsigned int i;
    unsigned int endpoints_count;
    struct usb_host_interface *iface_desc = interface->cur_altsetting;
 
    dev_info(&interface->dev, "USB Driver Probed: Vendor ID : 0x%02x,\t"
             "Product ID : 0x%02x\n", id->idVendor, id->idProduct);
             
    endpoints_count = iface_desc->desc.bNumEndpoints;
    
    PRINT_USB_INTERFACE_DESCRIPTOR(iface_desc->desc);
    
     for ( i = 0; i < endpoints_count; i++ ) {
          PRINT_USB_ENDPOINT_DESCRIPTOR(iface_desc->endpoint[i].desc);
     }
    return 0;  
}
 

static void disconnect(struct usb_interface *interface)
{
    dev_info(&interface->dev, "USB Driver Disconnected\n");
}
 
const struct usb_device_id table[] = {
    { USB_DEVICE( USB_VENDOR_ID, USB_PRODUCT_ID ) },    
    { } 
};
 
MODULE_DEVICE_TABLE(usb, table);
 
//register with the linux subsystem
static struct usb_driver driver = {
    .name       = "USB Driver",
    .probe      = probe,
    .disconnect = disconnect,
    .id_table   = table,
};
 
#if ( IS_NEW_METHOD_USED == 0 )
module_usb_driver(driver);
 
#else
static int __init init(void)
{
    return usb_register(&driver);
}
 
static void __exit exit(void)
{
    usb_deregister(&driver);
}
 
module_init(init);
module_exit(exit);
#endif
 
MODULE_LICENSE("GPL");
MODULE_AUTHOR("vaibhav");
MODULE_DESCRIPTION("USB Driver");
MODULE_VERSION("1.0");
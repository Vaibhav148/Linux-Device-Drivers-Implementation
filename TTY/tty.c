
/* Declare what kind of code we want from the header files */
#define __KERNEL__         /* We're part of the kernel */
#define MODULE             /* Not a permanent part, though. *

/* Standard headers */
#include <linux/module.h>       /* Needed by all LKMs */
#include <linux/kernel.h>       /* Needed for KERN_ALERT */
#include <linux/init.h>         /* Needed for the macros */
#include <linux/sched.h>        /* For current */
#include <linux/tty.h>
#include <linux/tty_driver.h>            /* For the tty declarations */

/* Documentation macros.  These are not used by the kernel, but it
 * may complain if they are not provided. */

MODULE_AUTHOR("Vaibhav");
MODULE_DESCRIPTION("Simple Kernel Module");
MODULE_LICENSE("GPL");

static void print_string(char *str)
{
        struct tty_struct *my_tty;

        my_tty = get_current_tty();

        /* 
         * If my_tty is NULL, the current task has no tty you can print to 
         * (ie, if it's a daemon).  If so, there's nothing we can do.
         */
        if (my_tty != NULL) {

               
(my_tty->driver->ops->write) (my_tty, str, strlen(str));

                
                (my_tty)->ops->write(my_tty, "\015\012", 2);
        }
}
static int __init hellotty_init_module(void)
{
  print_string("Hello, world - this is the kernel speaking.\n");

 
  return 0;
}


static void __exit hellotty_exit_module(void)
{
  print_string("Exit from Current TTY.\n");
}

module_init(hellotty_init_module);
module_exit(hellotty_exit_module);

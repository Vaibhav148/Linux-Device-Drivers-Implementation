# Linux-Device-Drivers-Implementation
Implementation of Loadable Kernel Modules for Character device , Block device ,USB ,Flashdrives and TTY

### How to run :

Applicable for all drivers...

- Build the driver using Makefile
```
$ sudo make
```
- Load the Driver
```
$ sudo insmod file_name.ko
```
- Check Kernel log
```
$ dmesg
```
- Check device file
```
$ ls -l /dev/ | grep "device_name"
```
- Check the Major number
```
$ cat /proc/devices
```


- Unload the driver 
```
$ sudo rmmod file_name

```
- For Character device driver test the driver using test.c file
```
$ cc test.c
$ ./a.out
```




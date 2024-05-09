

#ifndef PCD_DRIVER_DT_SYSFS_H
#define PCD_DRIVER_DT_SYSFS_H

#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/kdev_t.h>
#include <linux/uaccess.h>
#include <linux/platform_device.h>
#include <linux/slab.h>
#include <linux/mod_devicetable.h>
#include <linux/of.h>
#include <linux/of_device.h>
#include <linux/string.h>
#include <linux/gpio.h>
#include "platform.h"

#undef pr_fmt
#define pr_fmt(fmt) "[%s]: " fmt, __func__

#define CLASS_NAME      "GPIO_CLASS"
#define DEV_NAME        "GPIOs"
#define NO_OF_DEVICES   4
#define ATTR_GP_NAME    "GPIO_attr"

/* Create dummy device configure */
enum pcdev_name {
    GPIO_0_CONF,
    GPIO_1_CONF,
    GPIO_2_CONF,
    GPIO_3_CONF,
    
};

struct device_configure {
    int value;
    char *direction;
};

/* Structure represents device private data */
struct pcdev_private_data {
    struct pcdev_platform_data pdata;
    dev_t dev_num;
    char *buffer;
    struct cdev cdev;
};

/* Structure represents driver private data */
struct pcdrv_private_data {
    int total_device;
    dev_t device_number_base;
    struct class *class_pcd;
    struct device *device_pcd;
};

/* The prototype functions for the file operations of character driver */
int check_permission(int permission, int access_mode);
int pcd_open(struct inode *inode, struct file *filp);
int pcd_release(struct inode *inode, struct file *filp);
ssize_t pcd_read(struct file *filp, char __user *buff, size_t count, loff_t *f_pos);
ssize_t pcd_write(struct file *filp, const char __user *buff, size_t count, loff_t *f_pos);
loff_t pcd_lseek(struct file *filp, loff_t offset, int whence);

/* The prototype functions for the platform driver */
int pcd_platform_driver_probe(struct platform_device *pdev);
int pcd_platform_driver_remove(struct platform_device *pdev);

void GPIO_INIT(struct pcdev_platform_data *pdata);

/* The prototype functions for device attributes */
ssize_t show_value(struct device *dev, struct device_attribute *attr, char *buf);
ssize_t store_value(struct device *dev, struct device_attribute *attr, const char *buf, size_t count);
ssize_t show_direction(struct device *dev, struct device_attribute *attr, char *buf);
ssize_t store_direction(struct device *dev, struct device_attribute *attr, const char *buf, size_t count);
/* Other sub-functions */
struct pcdev_platform_data* pcdev_check_pf_dt(struct device *dev);
int pcd_sysfs_create(struct device *dev);

#endif 
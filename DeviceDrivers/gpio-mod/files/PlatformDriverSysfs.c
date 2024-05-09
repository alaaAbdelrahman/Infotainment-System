
#include "PlatformDriverSysfs.h"

static bool gpio_direction_out=true;


/* Struct used for matching with setup code */
struct platform_device_id pcdevs_ids[] = {
    {.name = "GPIO_0"},
    {.name = "GPIO_1"},
    {.name = "GPIO_2"},
    {.name = "GPIO_3"},
    {.name = "GPIO_4"},
    {.name = "GPIO_5"},
    {.name = "GPIO_6"},
    {.name = "GPIO_7"},
    {.name = "GPIO_8"},
    {.name = "GPIO_9"},
    {.name = "GPIO_10"},
    {.name = "GPIO_11"},
    {.name = "GPIO_12"},
    {.name = "GPIO_13"},
    {.name = "GPIO_14"},
    {.name = "GPIO_15"},
    {.name = "GPIO_16"},
    {.name = "GPIO_17"},
    {.name = "GPIO_18"},
    {.name = "GPIO_19"},
    {.name = "GPIO_20"},
    {.name = "GPIO_21"},
    {.name = "GPIO_22"},
    {.name = "GPIO_23"},
    {.name = "GPIO_24"},
    {.name = "GPIO_25"},
    {.name = "GPIO_26"},
    {.name = "GPIO_27"},
    {}
};

struct pcdrv_private_data pcdrv_data;



/* Create two custom attributes of a device. */
static DEVICE_ATTR(value,S_IRUGO | S_IWUSR, show_value, store_value);
static DEVICE_ATTR(direction, S_IRUGO | S_IWUSR, show_direction, store_direction);

struct attribute *pcd_attrs[] = {
    &dev_attr_value.attr,
    &dev_attr_direction.attr,
    NULL
};

struct file_operations pcd_fops = {
    .open = pcd_open,
    .write = pcd_write,
    .read = pcd_read,
    .release = pcd_release,
    .llseek = pcd_lseek,
    .owner = THIS_MODULE
};

struct platform_driver pcd_platform_driver = {
    .probe = pcd_platform_driver_probe,
    .remove = pcd_platform_driver_remove,
    /* Then try to match against the id table */
    .id_table = pcdevs_ids,
    /* fall-back to driver name match */
    .driver = {
        .name = "pseudo-char-device",
    }
};

/* Implement interface for exporting device attributes */
ssize_t show_value(struct device *dev, struct device_attribute *attr, char *buf) {

    struct device *parent_device = dev->parent;
    struct platform_device *pdev = to_platform_device(parent_device);
    int pinNo= pdev->id;

       return sprintf(buf, "%d\n", gpio_get_value(pinNo));
}

ssize_t store_value(struct device *dev, struct device_attribute *attr, const char *buf, size_t count) {
    
    struct device *parent_device = dev->parent;
    struct platform_device *pdev = to_platform_device(parent_device);
    int pinNo= pdev->id;

    int value;
    sscanf(buf, "%du", &value);

    switch(value)
    {
        case 1 : 
               gpio_set_value(pinNo, 1);
        break;
        case 0 :
                gpio_set_value(pinNo, 0);
        break;
        default:
                pr_info("wrong input, value set to 0\n");
                gpio_set_value(pinNo, 0);
        break;
    }

 
    return count;
 
}

ssize_t show_direction(struct device *dev, struct device_attribute *attr, char *buf) {
   return sprintf(buf, "%s\n", gpio_direction_out ? "out" : "in");
}
ssize_t store_direction(struct device *dev, struct device_attribute *attr, const char *buf, size_t count) {
      
      struct device *parent_device = dev->parent;
    struct platform_device *pdev = to_platform_device(parent_device);
    int pinNo= pdev->id;

     if (strncmp(buf, "in", 2) == 0) {
        gpio_direction_input(pinNo);
         gpio_set_value(pinNo, 0);
        gpio_direction_out = false;
    } else if (strncmp(buf, "out", 3) == 0) {
        gpio_direction_output(pinNo, 0);  // Maintain current value
        gpio_direction_out = true;
    } else {
        printk(KERN_INFO "Invalid direction value\n");
        return -EINVAL;
    }
    return count;
}



int pcd_sysfs_create(struct device *dev) {

    int ret;
    /* Create file sysfs in device class */
    ret = sysfs_create_file(&dev->kobj, &dev_attr_value.attr);
    if (ret)
        return ret;

    ret = sysfs_create_file(&dev->kobj, &dev_attr_direction.attr);
    if (ret)
        return ret;

    return 0;
}

int pcd_platform_driver_probe(struct platform_device *pdev) {

    int ret, driver_data;
    struct pcdev_private_data *dev_data;
    struct pcdev_platform_data *pdata;
    struct device *dev = &pdev->dev;

     int pinNo=  pdev->id;

    
    dev_info(dev, "Device was detected\n");


        pdata = (struct pcdev_platform_data *)dev_get_platdata(dev);
        if (!pdata) {
            dev_info(dev, "No platform data available\n");
            return -EINVAL;
        }
        driver_data = pdev->id_entry->driver_data;
    

    /* Dynamically allocate memory for the device private data */
    dev_data = devm_kzalloc(dev, sizeof(*dev_data), GFP_KERNEL);
    if (!dev_data) {
        dev_info(dev, "Cannot allocate memory\n");
        return -ENOMEM;
    }

    /* Save device private data pointer in platform device structure (release)*/
    dev_set_drvdata(dev, dev_data);

    dev_data->pdata.value = pdata->value;
    dev_data->pdata.permission = pdata->permission;
    dev_data->pdata.direction = pdata->direction;

    pr_info("Device value %d\n", dev_data->pdata.value);
    pr_info("Device permission %d\n", dev_data->pdata.permission);
    pr_info("Device direction %d\n", dev_data->pdata.direction);
    pr_info("Device Pin No = %d\n",pinNo);

    /* Dynamically allocate memory for the device buffer */
    dev_data->buffer = devm_kzalloc(dev, dev_data->pdata.size, GFP_KERNEL);
    if (!dev_data->buffer) {
        dev_info(dev, "Cannot allocate memory\n");
        return -ENOMEM;
    }

    dev_data->dev_num = pcdrv_data.device_number_base + pcdrv_data.total_device;

    cdev_init(&dev_data->cdev, &pcd_fops);
    dev_data->cdev.owner = THIS_MODULE;
    ret = cdev_add(&dev_data->cdev, dev_data->dev_num, 1);
    if (ret < 0) {
        dev_info(dev, "Cdev add failed\n");
        return ret;
    }

    /* Create device file for the detected platform device */
    pcdrv_data.device_pcd = device_create(pcdrv_data.class_pcd, dev, dev_data->dev_num, NULL, "GPIO_%d", pinNo);
    if (IS_ERR(pcdrv_data.device_pcd)) {
        ret = PTR_ERR(pcdrv_data.device_pcd);
        cdev_del(&dev_data->cdev);
        return ret;
    }

    pcdrv_data.total_device++;

    ret = pcd_sysfs_create(pcdrv_data.device_pcd);
    if (ret){
        device_destroy(pcdrv_data.class_pcd, dev_data->dev_num);
        return ret;
    }

    dev_info(dev, "Probe was successful\n");
    pr_info("--------------------\n");



   
    if (gpio_request(pinNo,dev_data->pdata.serial_number)) {
		printk("cannot allocate for GPIO %d\n",pinNo);
		return -EFAULT;
	}

    if (gpio_direction_output(pinNo, 0)) {
	return -EFAULT;
	}


    return 0;




}

int pcd_platform_driver_remove(struct platform_device *pdev) {

    struct pcdev_private_data *dev_data = dev_get_drvdata(&pdev->dev);
int pinNo=  pdev->id;
    device_destroy(pcdrv_data.class_pcd, dev_data->dev_num);
    cdev_del(&dev_data->cdev);
    pcdrv_data.total_device--;

    dev_info(&pdev->dev, "Device was removed");

    gpio_free(pinNo);
    return 0;
}

static int __init char_platform_driver_init(void) {

    int ret;

    /* Dynamically allocate a device number <one device> */
    ret = alloc_chrdev_region(&pcdrv_data.device_number_base, 0, NO_OF_DEVICES, DEV_NAME);
    if (ret < 0)
        goto error_out;

    /* Create class and device files </sys/class/...> */
    pcdrv_data.class_pcd = class_create(THIS_MODULE,CLASS_NAME);
    if (IS_ERR(pcdrv_data.class_pcd)) {
        ret = PTR_ERR(pcdrv_data.class_pcd);
        goto unregister_allocate_dd;
    }

    ret = platform_driver_register(&pcd_platform_driver);
    if (ret < 0)
        goto class_del;

    pr_info("Platform driver module loaded\n");

    return 0;

class_del:
    class_destroy(pcdrv_data.class_pcd);
    unregister_chrdev_region(pcdrv_data.device_number_base, NO_OF_DEVICES);
unregister_allocate_dd:
    unregister_chrdev_region(pcdrv_data.device_number_base, NO_OF_DEVICES);
error_out:
    return ret;

}

static void __exit char_platform_driver_exit(void) {

    platform_driver_unregister(&pcd_platform_driver);
    class_destroy(pcdrv_data.class_pcd);
    unregister_chrdev_region(pcdrv_data.device_number_base, NO_OF_DEVICES);

    pr_info("Platform driver module unloaded\n");
}




module_init(char_platform_driver_init);
module_exit(char_platform_driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("AlaaGbr");

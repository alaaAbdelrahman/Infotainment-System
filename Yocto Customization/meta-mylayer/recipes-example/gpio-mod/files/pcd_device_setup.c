
#include <linux/module.h>
#include <linux/platform_device.h>
#include "platform.h"

#undef pr_fmt
#define pr_fmt(fmt) "[%s]: " fmt, __func__

/* Callback to free the device after all references have gone away */
void pcdev_release(struct device *dev) {
    // TODO: 
    pr_info("Device released\n");
}

/* Create two platform data */
struct pcdev_platform_data pcdev_pdata[] = {
    [0] = {.size = 512, .permission = RDWR, .serial_number = "GPIO_0"},
    [1] = {.size = 512, .permission = RDWR, .serial_number = "GPIO_1"},
    [2] = {.size = 1024, .permission = RDWR, .serial_number = "GPIO_2"},
    [3] = {.size = 2048, .permission = RDWR, .serial_number = "GPIO_3"}
};

/* Create two platform device */
struct platform_device platform_pcdev_1 = {
    .name = "GPIO_21",
    .id = 21,
    .dev = {
        .platform_data = &pcdev_pdata[0],
        .release = pcdev_release
    }

};

struct platform_device platform_pcdev_2 = {
    .name = "GPIO_20",
    .id = 20,
    .dev = {
        .platform_data = &pcdev_pdata[1],
        .release = pcdev_release
    }
};

struct platform_device platform_pcdev_3 = {
    .name = "GPIO_16",
    .id = 16,
    .dev = {
        .platform_data = &pcdev_pdata[2],
        .release = pcdev_release
    }
};

struct platform_device platform_pcdev_4 = {
    .name = "GPIO_12",
    .id = 12,
    .dev = {
        .platform_data = &pcdev_pdata[3],
        .release = pcdev_release
    }
};

struct platform_device *platform_pcdevs[] = {
    &platform_pcdev_1,
    &platform_pcdev_2,
    &platform_pcdev_3,
    &platform_pcdev_4
};

static int __init pcdev_platform_init(void) {
    /* Register platform-level device */
    platform_add_devices(platform_pcdevs, ARRAY_SIZE(platform_pcdevs));

    pr_info("Platform device setup module loaded\n");

    return 0;
}

static void __exit pcdev_platform_exit(void) {
    /* Unregister platform-level device */
    platform_device_unregister(&platform_pcdev_1);
    platform_device_unregister(&platform_pcdev_2);
    platform_device_unregister(&platform_pcdev_3);
    platform_device_unregister(&platform_pcdev_4);

    pr_info("Platform device setup module unloaded\n");
}

module_init(pcdev_platform_init);
module_exit(pcdev_platform_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("AlaaGbr");

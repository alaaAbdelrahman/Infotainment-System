#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/i2c.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/slab.h>
#include <linux/sysfs.h>
#include <linux/kobject.h>

#define MPU6050_ADDR 0x68

static struct kobject *kobj_ref;
static struct i2c_client *mpu6050_client;

static ssize_t sysfs_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf);
static ssize_t sysfs_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count);

// Declare the sysfs attributes that will appear in /sys/kernel/
static struct kobj_attribute mpu6050_attr = __ATTR(mpu6050_data, 0660, sysfs_show, sysfs_store);

static ssize_t sysfs_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf)
{
    u8 data[14];
    s16 accel_x, accel_y, accel_z, temp, gyro_x, gyro_y, gyro_z;
    int ret;

    ret = i2c_smbus_read_i2c_block_data(mpu6050_client, 0x3B, 14, data);
    if (ret < 0)
        return ret;

    accel_x = (data[0] << 8) | data[1];
    accel_y = (data[2] << 8) | data[3];
    accel_z = (data[4] << 8) | data[5];
    temp = (data[6] << 8) | data[7];
    gyro_x = (data[8] << 8) | data[9];
    gyro_y = (data[10] << 8) | data[11];
    gyro_z = (data[12] << 8) | data[13];

// Convert accelerometer readings from raw to g (assuming sensitivity of 16384 LSB/g)
// Multiply by 1000 to keep three decimal places in an integer
int accel_x_mg = (accel_x * 1000) / 16384; // Results in milli-g's
int accel_y_mg = (accel_y * 1000) / 16384;
int accel_z_mg = (accel_z * 1000) / 16384;

// Convert gyroscope readings from raw to degrees/s (assuming sensitivity of 131 LSB/(°/s))
// Multiply by 1000 to keep three decimal places in an integer
int gyro_x_mdps = (gyro_x * 1000) / 131; // Results in milli-degrees per second
int gyro_y_mdps = (gyro_y * 1000) / 131;
int gyro_z_mdps = (gyro_z * 1000) / 131;

int temperature_c_tenths = (temp * 10) / 340 + 365; // Temperature in tenths of degrees
return sprintf(buf, 
    "Accel X:%d.%03d g Y:%d.%03d g Z:%d.%03d g Temp:%d.%d°C Gyro X:%d.%03d °/s Y:%d.%03d °/s Z:%d.%03d °/s\n",
    accel_x_mg / 1000, abs(accel_x_mg % 1000), // accel x in g
    accel_y_mg / 1000, abs(accel_y_mg % 1000), // accel y in g
    accel_z_mg / 1000, abs(accel_z_mg % 1000), // accel z in g
    temperature_c_tenths / 10, abs(temperature_c_tenths % 10), // temperature in C
    gyro_x_mdps / 1000, abs(gyro_x_mdps % 1000), // gyro x in °/s
    gyro_y_mdps / 1000, abs(gyro_y_mdps % 1000), // gyro y in °/s
    gyro_z_mdps / 1000, abs(gyro_z_mdps % 1000)  // gyro z in °/s
);

}



static ssize_t sysfs_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count)
{
    // This function is a placeholder. You can add write functionality here.
    return count;
}

static int __init mpu6050_init(void)
{
     struct i2c_adapter *adapter;
    struct i2c_board_info info = {
        .type = "mpu6050",
        .addr = MPU6050_ADDR,
    };

    adapter = i2c_get_adapter(1); // Check your adapter number with `ls /dev/i2c-*`
    if (!adapter) {
        printk(KERN_ALERT "Failed to get I2C adapter\n");
        return -ENODEV;
    }

    mpu6050_client = i2c_new_client_device(adapter, &info);
    if (!mpu6050_client) {
        printk(KERN_ALERT "Failed to register I2C client\n");
        return -ENODEV;
    }

    kobj_ref = kobject_create_and_add("mpu6050_sysfs", kernel_kobj);
    if (!kobj_ref)
        return -ENOMEM;

    if (sysfs_create_file(kobj_ref, &mpu6050_attr.attr)) {
        printk(KERN_ALERT "Failed to create sysfs file\n");
        kobject_put(kobj_ref);
        return -ENODEV;
    }

    i2c_smbus_write_byte_data(mpu6050_client, 0x6B, 0x00); // Wake up MPU6050
    printk(KERN_INFO "MPU6050 module loaded\n");
    return 0;
}

static void __exit mpu6050_exit(void)
{
  sysfs_remove_file(kobj_ref, &mpu6050_attr.attr);
    kobject_put(kobj_ref);
    i2c_unregister_device(mpu6050_client);
    printk(KERN_INFO "MPU6050 module unloaded\n");
}

module_init(mpu6050_init);
module_exit(mpu6050_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("A simple Linux driver for the MPU6050 sensor");

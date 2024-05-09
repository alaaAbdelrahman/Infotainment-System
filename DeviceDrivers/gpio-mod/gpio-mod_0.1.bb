SUMMARY = "Example of how to build an external Linux kernel module"
DESCRIPTION = "${SUMMARY}"
LICENSE = "GPLv2"
LIC_FILES_CHKSUM = "file://COPYING;md5=12f884d2ae1ff87c09e5b7ccc2c4ca7e"

inherit module

SRC_URI = "file://Makefile \
           file://PlatformDriverSysfs.c \
           file://pcd_syscalls.c \
           file://pcd_device_setup.c \
           file://PlatformDriverSysfs.h \
           file://platform.h \
           file://COPYING \
          "

S = "${WORKDIR}"

# The inherit of module.bbclass will automatically name module packages with
# "kernel-module-" prefix as required by the oe-core build environment.

RPROVIDES_${PN} += "kernel-module-gpio_driver"
RPROVIDES_${PN} += "kernel-module-pcd_device_setup"
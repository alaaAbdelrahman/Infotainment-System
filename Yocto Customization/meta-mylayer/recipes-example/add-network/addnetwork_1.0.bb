# Description and Maintainer
DESCRIPTION = "demo application fetched from GitHub"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COREBASE}/meta-mylayer/COPYING.MIT;md5=3da9cfbcb788c80a0384361b4de20420"



SRC_URI = "file://10-static.network"

# In a recipe or bbappend:
do_install_append() {
    install -d ${D}/etc/systemd/network/
    install -m 0644 ${WORKDIR}/10-static.network ${D}/etc/systemd/network/
}

FILES_${PN} += "/etc/systemd/network/10-static.network"

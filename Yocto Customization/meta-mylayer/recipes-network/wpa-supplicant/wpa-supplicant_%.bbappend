FILESEXTRAPATHS_prepend := "${THISDIR}/${PN}:"
SYSTEMD_SERVICE_${PN} += "wpa_supplicant.service"

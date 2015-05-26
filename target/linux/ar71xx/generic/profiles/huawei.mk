#
# Copyright (C) 2009 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

define Profile/WA161DD
	NAME:=Huawei WA161DD-NZ
	PACKAGES:=kmod-usb-core kmod-usb2 kmod-ledtrig-usbdev kmod-ath9k
endef

define Profile/WA161DD/Description
	Package set optimized for the Huawei WA161DD-NZ.
endef
$(eval $(call Profile,WA161DD))



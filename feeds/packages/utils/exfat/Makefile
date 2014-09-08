#
# exFAT filesystem driver Makefile
#
# Written by hackapscal (hackapscal@gmail.com)
#

include $(TOPDIR)/rules.mk

PKG_NAME:=exfat
PKG_REV:=415
PKG_VERSION:=r$(PKG_REV)
PKG_RELEASE:=1

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.gz
PKG_SOURCE_URL:=http://exfat.googlecode.com/svn/trunk
PKG_SOURCE_SUBDIR:=$(PKG_NAME)-$(PKG_VERSION)
PKG_SOURCE_VERSION:=$(PKG_REV)
PKG_SOURCE_PROTO:=svn

include $(INCLUDE_DIR)/package.mk
include $(INCLUDE_DIR)/scons.mk

define Package/exfat
  SECTION:=utils
  CATEGORY:=Utilities
  SUBMENU:=Filesystem
  TITLE:=FUSE-based exFAT Filesystem Driver
  URL:=http://code.google.com/p/exfat/
  DEPENDS:=+libfuse
endef

define Package/exfat/description
  A full-featured exFAT file system implementation for GNU/Linux and other Unix-like systems as a FUSE module and a set of utilities.
endef

define Package/exfat/config
	source "$(SOURCE)/Config.in"
endef

TARGET_CFLAGS+=$(TARGET_CPPFLAGS) -std=c99 -fPIC

define Package/exfat/install
	$(INSTALL_DIR) $(1)/sbin
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/mount.exfat-fuse $(1)/sbin/mount.exfat-fuse
	ln -s mount.exfat-fuse $(1)/sbin/mount.exfat
	if [ "$(CONFIG_EXFAT_DUMP)" == "y" ]; then $(INSTALL_BIN) $(PKG_INSTALL_DIR)/dumpexfat $(1)/sbin/dumpexfat; fi
	if [ "$(CONFIG_EXFAT_FSCK)" == "y" ]; then \
		$(INSTALL_BIN) $(PKG_INSTALL_DIR)/exfatfsck $(1)/sbin/exfatfsck; \
		ln -s exfatfsck $(1)/sbin/fsck.exfat; \
	fi
	if [ "$(CONFIG_EXFAT_LABEL)" == "y" ]; then $(INSTALL_BIN) $(PKG_INSTALL_DIR)/exfatlabel $(1)/sbin/exfatlabel; fi
	if [ "$(CONFIG_DB47_MKFS)" == "y" ]; then \
		$(INSTALL_BIN) $(PKG_INSTALL_DIR)/mkexfatfs $(1)/sbin/mkexfatfs; \
		ln -s mkexfatfs $(1)/sbin/mkfs.exfat; \
	fi
endef

$(eval $(call BuildPackage,exfat))

#
# Copyright (C) 2007-2013 OpenWrt.org
# Copyright (C) 2014 NowRush Studio <hackpascal@gmail.com>
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=goagent
PKG_VERSION:=3.1.25
PKG_RELEASE:=1

PKG_SOURCE_PROTO:=git
PKG_SOURCE_URL:=git://github.com/goagent/goagent.git
PKG_SOURCE_VERSION:=v$(PKG_VERSION)
PKG_SOURCE_SUBDIR:=$(PKG_NAME)-$(PKG_VERSION)
PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.gz

include $(INCLUDE_DIR)/package.mk

define Package/goagent
  SECTION:=net
  CATEGORY:=Network
  DEPENDS:=+python +pyopenssl +gevent
  TITLE:=GoAgent Proxy
  VERSION:=$(PKG_VERSION)
endef

define Package/goagent/conffiles
/etc/goagent/CA.crt
/etc/goagent/proxy.pac
/etc/config/goagent
endef

define Package/goagent/description
 GoAgent is a proxy tool to break the GFW.
endef

define Build/Compile
	unzip $(PKG_BUILD_DIR)/local/packages.egg -d $(PKG_BUILD_DIR)/local/packages
	cd $(PKG_BUILD_DIR)/local/packages/noarch && zip -9r $(PKG_BUILD_DIR)/dnslib.egg dnslib
endef

define Package/goagent/install
	$(INSTALL_DIR) $(1)
	$(INSTALL_DIR) $(1)/etc
	$(INSTALL_DIR) $(1)/etc/config
	$(INSTALL_DIR) $(1)/etc/goagent
	$(INSTALL_DIR) $(1)/etc/init.d
	$(INSTALL_DIR) $(1)/etc/uci-defaults
	$(INSTALL_DIR) $(1)/usr/lib/goagent
	$(INSTALL_DIR) $(1)/CONTROL

	$(INSTALL_BIN) ./files/genproxyini $(1)/usr/lib/goagent
	$(INSTALL_BIN) ./files/goagent_wrapper $(1)/usr/lib/goagent
	$(INSTALL_CONF) ./files/goagent.conf $(1)/etc/config/goagent
	$(INSTALL_BIN) ./files/goagent.init $(1)/etc/init.d/goagent
	$(INSTALL_BIN) ./files/luci-goagent $(1)/etc/uci-defaults
	$(INSTALL_BIN) ./ipkg/postinst $(1)/CONTROL/postinst

	$(INSTALL_DATA) $(PKG_BUILD_DIR)/local/CA.crt $(1)/etc/goagent
	$(INSTALL_DATA) $(PKG_BUILD_DIR)/local/proxy.pac $(1)/etc/goagent

	$(INSTALL_BIN) $(PKG_BUILD_DIR)/local/proxy.py $(1)/usr/lib/goagent
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/local/dnsproxy.py $(1)/usr/lib/goagent
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/local/proxylib.py $(1)/usr/lib/goagent

	$(INSTALL_DATA) $(PKG_BUILD_DIR)/dnslib.egg $(1)/usr/lib/goagent
endef

$(eval $(call BuildPackage,goagent))

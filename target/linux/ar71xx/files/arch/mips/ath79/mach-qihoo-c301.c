/*
 *  Qihoo 360 C301 board support
 *
 *  Copyright (C) 2013 Gabor Juhos <juhosg@openwrt.org>
 *  Copyright (C) 2014 hackpascal <hackpascal@gmail.com>
 *
 *  This program is free software; you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License version 2 as published
 *  by the Free Software Foundation.
 */

#include <linux/pci.h>
#include <linux/phy.h>
#include <linux/gpio.h>
#include <linux/platform_device.h>
#include <linux/ath9k_platform.h>
#include <linux/crc32.h>
#include <linux/mtd/mtd.h>

#include <asm/mach-ath79/ar71xx_regs.h>

#include "common.h"
#include "pci.h"
#include "dev-ap9x-pci.h"
#include "dev-eth.h"
#include "dev-gpio-buttons.h"
#include "dev-leds-gpio.h"
#include "dev-m25p80.h"
#include "dev-spi.h"
#include "dev-usb.h"
#include "dev-wmac.h"
#include "machtypes.h"
#include "nvram.h"

#define QIHOO_C301_GPIO_LED_STATUS_GREEN	0
#define QIHOO_C301_GPIO_LED_STATUS_RED		11

#define QIHOO_C301_GPIO_LED_WAN			1
#define QIHOO_C301_GPIO_LED_LAN1		2
#define QIHOO_C301_GPIO_LED_LAN2		3
#define QIHOO_C301_GPIO_ETH_LEN_EN		18

#define QIHOO_C301_GPIO_BTN_RESET		16

#define QIHOO_C301_GPIO_USB_POWER		19

#define QIHOO_C301_GPIO_SPI_CS1			12

#define QIHOO_C301_GPIO_EXTERNAL_LNA0		14
#define QIHOO_C301_GPIO_EXTERNAL_LNA1		15

#define QIHOO_C301_KEYS_POLL_INTERVAL		20	/* msecs */
#define QIHOO_C301_KEYS_DEBOUNCE_INTERVAL 	(3 * QIHOO_C301_KEYS_POLL_INTERVAL)

#define QIHOO_C301_WMAC_CALDATA_OFFSET		0x1000

#define QIHOO_C301_NVRAM_ADDR			0x1f058010
#define QIHOO_C301_NVRAM_SIZE			0x7ff0

static struct gpio_led qihoo_c301_leds_gpio[] __initdata = {
	{
		.name		= "qihoo:green:status",
		.gpio		= QIHOO_C301_GPIO_LED_STATUS_GREEN,
		.active_low	= 1,
	},
	{
		.name		= "qihoo:red:status",
		.gpio		= QIHOO_C301_GPIO_LED_STATUS_RED,
		.active_low	= 1,
	},
};

static struct gpio_keys_button qihoo_c301_gpio_keys[] __initdata = {
	{
		.desc		= "reset",
		.type		= EV_KEY,
		.code		= KEY_RESTART,
		.debounce_interval = QIHOO_C301_KEYS_DEBOUNCE_INTERVAL,
		.gpio		= QIHOO_C301_GPIO_BTN_RESET,
		.active_low	= 1,
	},
};

struct flash_platform_data flash __initdata = {NULL, NULL, 0};

static int qihoo_c301_board = 0;

static u8 wlan24mac[ETH_ALEN];

static void qihoo_c301_get_mac(const char *name, char *mac)
{
	u8 *nvram = (u8 *) KSEG1ADDR(QIHOO_C301_NVRAM_ADDR);
	int err;

	err = ath79_nvram_parse_mac_addr(nvram, QIHOO_C301_NVRAM_SIZE,
					 name, mac);
	if (err)
		pr_err("no MAC address found for %s\n", name);
}

static void __init qihoo_c301_setup(void)
{
	ath79_register_m25p80_multi(&flash);

	ath79_gpio_function_enable(AR934X_GPIO_FUNC_JTAG_DISABLE);

	ath79_gpio_output_select(QIHOO_C301_GPIO_LED_WAN,
				 AR934X_GPIO_OUT_LED_LINK4);
	ath79_gpio_output_select(QIHOO_C301_GPIO_LED_LAN1,
				 AR934X_GPIO_OUT_LED_LINK1);
	ath79_gpio_output_select(QIHOO_C301_GPIO_LED_LAN2,
				 AR934X_GPIO_OUT_LED_LINK2);

	ath79_gpio_output_select(QIHOO_C301_GPIO_SPI_CS1,
				 AR934X_GPIO_OUT_SPI_CS1);

	gpio_request_one(QIHOO_C301_GPIO_ETH_LEN_EN,
			 GPIOF_OUT_INIT_LOW | GPIOF_EXPORT_DIR_FIXED,
			 "Ethernet LED enable");

	ath79_register_leds_gpio(-1, ARRAY_SIZE(qihoo_c301_leds_gpio),
				 qihoo_c301_leds_gpio);

	ath79_register_gpio_keys_polled(-1, QIHOO_C301_KEYS_POLL_INTERVAL,
					ARRAY_SIZE(qihoo_c301_gpio_keys),
					qihoo_c301_gpio_keys);

	ath79_wmac_set_ext_lna_gpio(0, QIHOO_C301_GPIO_EXTERNAL_LNA0);
	ath79_wmac_set_ext_lna_gpio(1, QIHOO_C301_GPIO_EXTERNAL_LNA1);

	qihoo_c301_get_mac("wlan24mac=", wlan24mac);

	ath79_register_pci();

	ath79_setup_ar934x_eth_cfg(AR934X_ETH_CFG_SW_ONLY_MODE |
				   AR934X_ETH_CFG_SW_PHY_SWAP);

	ath79_register_mdio(1, 0x0);

	/* LAN */
	qihoo_c301_get_mac("lanmac=", ath79_eth1_data.mac_addr);

	/* GMAC1 is connected to the internal switch */
	ath79_eth1_data.phy_if_mode = PHY_INTERFACE_MODE_GMII;

	ath79_register_eth(1);

	/* WAN */
	qihoo_c301_get_mac("wanmac=", ath79_eth0_data.mac_addr);

	/* GMAC0 is connected to the PHY4 of the internal switch */
	ath79_switch_data.phy4_mii_en = 1;
	ath79_switch_data.phy_poll_mask = BIT(0);

	ath79_eth0_data.phy_if_mode = PHY_INTERFACE_MODE_MII;
	ath79_eth0_data.phy_mask = BIT(0);
	ath79_eth0_data.mii_bus_dev = &ath79_mdio1_device.dev;

	ath79_register_eth(0);

	gpio_request_one(QIHOO_C301_GPIO_USB_POWER,
			 GPIOF_OUT_INIT_HIGH | GPIOF_EXPORT_DIR_FIXED,
			 "USB power");
	ath79_register_usb();

	qihoo_c301_board = 1;
}

MIPS_MACHINE(ATH79_MACH_QIHOO_C301, "Qihoo-C301", "Qihoo 360 C301",
	     qihoo_c301_setup);

static int qihoo_init_wmac(void)
{
	struct mtd_info * mtd;
	size_t nb = 0;
	u8 *art;
	int ret;

	if (!qihoo_c301_board)
		return 0;

	mtd = get_mtd_device_nm("radiocfg");
	if (IS_ERR(mtd))
		return PTR_ERR(mtd);

	if (mtd->size != 0x10000)
		return -1;

	art = kzalloc(0x1000, GFP_KERNEL);
	if (!art)
		return -1;

	ret = mtd_read(mtd, QIHOO_C301_WMAC_CALDATA_OFFSET, 0x1000, &nb, art);
	if (nb != 0x1000)
	{
		kfree(art);
		return ret;
	}

	ath79_register_wmac(art, wlan24mac);

	return 0;
}

late_initcall(qihoo_init_wmac);

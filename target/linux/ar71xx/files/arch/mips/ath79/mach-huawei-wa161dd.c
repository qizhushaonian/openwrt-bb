/*
 *  Huawei WA161DD-NZ board support
 *
 *  Copyright (C) 2010-2012 Gabor Juhos <juhosg@openwrt.org>
 *  Copyright (C) 2005-2015 hackpascal <hackpascal@gmail.com>
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

#include <asm/mach-ath79/ar71xx_regs.h>
#include <linux/platform_data/phy-at803x.h>

#include "common.h"
#include "dev-ap9x-pci.h"
#include "dev-eth.h"
#include "dev-gpio-buttons.h"
#include "dev-leds-gpio.h"
#include "dev-m25p80.h"
#include "dev-spi.h"
#include "dev-wmac.h"
#include "dev-usb.h"
#include "machtypes.h"
#include "tplink-wmac.h"

#define HUAWEI_WA161DD_GPIO_LED_UNKNOWN_1	11
#define HUAWEI_WA161DD_GPIO_LED_UNKNOWN_2	12
#define HUAWEI_WA161DD_GPIO_LED_AMBER_LAN	13
#define HUAWEI_WA161DD_GPIO_LED_GREEN_LAN	14
#define HUAWEI_WA161DD_GPIO_LED_GREEN_LAN_POLARITY	15

#define HUAWEI_WA161DD_GPIO_BTN_RESET		16

#define HUAWEI_WA161DD_KEYS_POLL_INTERVAL	20	/* msecs */
#define HUAWEI_WA161DD_KEYS_DEBOUNCE_INTERVAL	(3 * HUAWEI_WA161DD_KEYS_POLL_INTERVAL)

#define HUAWEI_WA161DD_WMAC_CALDATA_OFFSET	0x1000
#define HUAWEI_WA161DD_PCIE_CALDATA_OFFSET	0x5000

static const char *huawei_wa161dd_part_probes[] = {
	"tp-link",
	NULL,
};

static struct flash_platform_data huawei_wa161dd_flash_data = {
	.part_probes	= huawei_wa161dd_part_probes,
};

static struct gpio_led huawei_wa161dd_leds_gpio[] __initdata = {
	{
		.name		= "huawei:unknown:1",
		.gpio		= HUAWEI_WA161DD_GPIO_LED_UNKNOWN_1,
		.active_low	= 1,
	}, {
		.name		= "huawei:unknown:2",
		.gpio		= HUAWEI_WA161DD_GPIO_LED_UNKNOWN_2,
		.active_low	= 1,
	}, {
		.name		= "huawei:amber:lan",
		.gpio		= HUAWEI_WA161DD_GPIO_LED_AMBER_LAN,
		.active_low	= 1,
	}, {
		.name		= "huawei:green:lan",
		.gpio		= HUAWEI_WA161DD_GPIO_LED_GREEN_LAN,
		.active_low	= 1,
	}, 
};

static struct gpio_keys_button huawei_wa161dd_gpio_keys[] __initdata = {
	{
		.desc		= "reset",
		.type		= EV_KEY,
		.code		= 0,
		.debounce_interval = HUAWEI_WA161DD_KEYS_DEBOUNCE_INTERVAL,
		.gpio		= HUAWEI_WA161DD_GPIO_BTN_RESET,
		.active_low	= 1,
	}
};

static struct at803x_platform_data mi124_ar8035_data = {
	.enable_rgmii_tx_delay = 1,
	.enable_rgmii_rx_delay = 1,
};

static struct mdio_board_info mi124_mdio0_info[] = {
	{
		.bus_id = "ag71xx-mdio.0",
		.phy_addr = 0,
		.platform_data = &mi124_ar8035_data,
	},
};

static void __init wa161dd_setup(void)
{
	u8 *mac = (u8 *) KSEG1ADDR(0x1f01fc00);

	ath79_register_m25p80(&huawei_wa161dd_flash_data);

	ath79_register_leds_gpio(-1, ARRAY_SIZE(huawei_wa161dd_leds_gpio),
				 huawei_wa161dd_leds_gpio);
	ath79_register_gpio_keys_polled(-1, HUAWEI_WA161DD_KEYS_POLL_INTERVAL,
					 ARRAY_SIZE(huawei_wa161dd_gpio_keys),
					 huawei_wa161dd_gpio_keys);

	tplink_register_builtin_wmac1(HUAWEI_WA161DD_WMAC_CALDATA_OFFSET, mac, -1);
	tplink_register_ap91_wmac2(HUAWEI_WA161DD_PCIE_CALDATA_OFFSET, mac, 2);

	ath79_setup_ar934x_eth_cfg(AR934X_ETH_CFG_RGMII_GMAC0 |
				   AR934X_ETH_CFG_SW_ONLY_MODE);

	ath79_register_mdio(1, 0x0);
	ath79_register_mdio(0, 0x0);

	ath79_init_mac(ath79_eth0_data.mac_addr, mac, 0);

	mdiobus_register_board_info(mi124_mdio0_info,
				    ARRAY_SIZE(mi124_mdio0_info));

	/* GMAC0 is connected to an AR8035 Gigabit PHY */
	ath79_eth0_data.phy_if_mode = PHY_INTERFACE_MODE_RGMII;
	ath79_eth0_data.phy_mask = BIT(0);
	ath79_eth0_data.mii_bus_dev = &ath79_mdio0_device.dev;
	ath79_eth0_pll_data.pll_1000 = 0x0e000000;
	ath79_eth0_pll_data.pll_100 = 0x0101;
	ath79_eth0_pll_data.pll_10 = 0x1313;
	ath79_register_eth(0);

	ath79_register_usb();

	gpio_request_one(HUAWEI_WA161DD_GPIO_LED_GREEN_LAN_POLARITY,
			 GPIOF_OUT_INIT_HIGH | GPIOF_EXPORT_DIR_FIXED,
			 "LAN LED Polarity");
}

MIPS_MACHINE(ATH79_MACH_HUAWEI_WA161DD, "HUAWEI-WA161DD",
	     "Huawei WA161DD-NZ", wa161dd_setup);

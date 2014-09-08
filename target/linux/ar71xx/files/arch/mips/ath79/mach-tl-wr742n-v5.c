/*
 *  TP-LINK TL-WR742N v5/TL-WR745N v2 board support
 *
 *  Copyright (C) 2011-2012 Gabor Juhos <juhosg@openwrt.org>
 *  Copyright (C) 2005-2014 HackPascal  <hackpascal@qq.com>
 *
 *  This program is free software; you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License version 2 as published
 *  by the Free Software Foundation.
 */

#include <linux/gpio.h>

#include <asm/mach-ath79/ath79.h>
#include <asm/mach-ath79/ar71xx_regs.h>

#include "common.h"
#include "dev-eth.h"
#include "dev-gpio-buttons.h"
#include "dev-leds-gpio.h"
#include "dev-m25p80.h"
#include "dev-usb.h"
#include "dev-wmac.h"
#include "machtypes.h"
#include "eeprom.h"

#define TL_WR742NV5_GPIO_BTN_RESET_WPS	11

#define TL_WR742NV5_GPIO_LED_WAN	17
#define TL_WR742NV5_GPIO_LED_LAN1	13
#define TL_WR742NV5_GPIO_LED_LAN2	14
#define TL_WR742NV5_GPIO_LED_LAN3	15
#define TL_WR742NV5_GPIO_LED_LAN4	16
#define TL_WR742NV5_GPIO_LED_SYSTEM	0

#define TL_WR742NV5_GPIO_USB_POWER	8

#define TL_WR742NV5_KEYS_POLL_INTERVAL	20	/* msecs */
#define TL_WR742NV5_KEYS_DEBOUNCE_INTERVAL (3 * TL_WR742NV5_KEYS_POLL_INTERVAL)

static const char *tl_wr742nv5_part_probes[] = {
	"tp-link",
	NULL,
};

static struct flash_platform_data tl_wr742nv5_flash_data = {
	.part_probes	= tl_wr742nv5_part_probes,
};

static struct gpio_led tl_wr742nv5_leds_gpio[] __initdata = {
	{
		.name		= "tp-link:green:lan1",
		.gpio		= TL_WR742NV5_GPIO_LED_LAN1,
		.active_low	= 0,
	}, {
		.name		= "tp-link:green:lan2",
		.gpio		= TL_WR742NV5_GPIO_LED_LAN2,
		.active_low	= 0,
	}, {
		.name		= "tp-link:green:lan3",
		.gpio		= TL_WR742NV5_GPIO_LED_LAN3,
		.active_low	= 0,
	}, {
		.name		= "tp-link:green:lan4",
		.gpio		= TL_WR742NV5_GPIO_LED_LAN4,
		.active_low	= 0,
	}, {
		.name		= "tp-link:green:system",
		.gpio		= TL_WR742NV5_GPIO_LED_SYSTEM,
		.active_low	= 0,
	}, {
		.name		= "tp-link:green:wan",
		.gpio		= TL_WR742NV5_GPIO_LED_WAN,
		.active_low	= 1,
	},
};

static struct gpio_keys_button tl_wr742nv5_gpio_keys[] __initdata = {
	{
		.desc		= "WPS",
		.type		= EV_KEY,
		.code		= KEY_WPS_BUTTON,
		.debounce_interval = TL_WR742NV5_KEYS_DEBOUNCE_INTERVAL,
		.gpio		= TL_WR742NV5_GPIO_BTN_RESET_WPS,
		.active_low	= 0,
	}
};

static void __init tl_ap121_setup(void)
{
	u8 *mac = (u8 *) KSEG1ADDR(0x1f01fc00);
	u8 *ee = ath79_get_eeprom(0) + 0x1000;

	ath79_setup_ar933x_phy4_switch(false, false);

	ath79_gpio_function_disable(AR933X_GPIO_FUNC_ETH_SWITCH_LED0_EN |
				    AR933X_GPIO_FUNC_ETH_SWITCH_LED1_EN |
				    AR933X_GPIO_FUNC_ETH_SWITCH_LED2_EN |
				    AR933X_GPIO_FUNC_ETH_SWITCH_LED3_EN |
				    AR933X_GPIO_FUNC_ETH_SWITCH_LED4_EN);

	ath79_register_m25p80(&tl_wr742nv5_flash_data);
	ath79_init_mac(ath79_eth0_data.mac_addr, mac, 1);
	ath79_init_mac(ath79_eth1_data.mac_addr, mac, -1);

	ath79_register_mdio(0, 0x0);
	ath79_register_eth(1);
	ath79_register_eth(0);

	ath79_register_wmac(ee, mac);

	gpio_request_one(TL_WR742NV5_GPIO_USB_POWER,
			 GPIOF_OUT_INIT_HIGH | GPIOF_EXPORT_DIR_FIXED,
			 "USB power");
	ath79_register_usb();
}

static void __init tl_wr742nv5_setup(void)
{
	tl_ap121_setup();

	ath79_register_leds_gpio(-1, ARRAY_SIZE(tl_wr742nv5_leds_gpio),
				 tl_wr742nv5_leds_gpio);
	ath79_register_gpio_keys_polled(1, TL_WR742NV5_KEYS_POLL_INTERVAL,
					ARRAY_SIZE(tl_wr742nv5_gpio_keys),
					tl_wr742nv5_gpio_keys);
}

MIPS_MACHINE(ATH79_MACH_TL_WR742N_V5, "TL-WR742N-v5",
	     "TP-LINK TL-WR742N v5", tl_wr742nv5_setup);

MIPS_MACHINE(ATH79_MACH_TL_WR745N_V2, "TL-WR745N-v2",
	     "TP-LINK TL-WR745N v2", tl_wr742nv5_setup);

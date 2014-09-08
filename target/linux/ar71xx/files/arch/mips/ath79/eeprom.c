#include <asm/mach-ath79/ath79.h>
#include <asm/mach-ath79/ar71xx_regs.h>

#include <linux/delay.h>

#include "common.h"
#include "eeprom.h"

static u32 flash_32m_jedec[] = {0x1C7019, 0x20BA19, 0xEF4019, 0xC22019, 0xC22619};

static void __iomem *ath79_spi_base;


static char ath79_eeprom[32 * 1024];

static u32 ath79_flash_read_id(int cs)
{
	u32 rd = 0;

	ath79_spi_start();

	ath79_spi_bit_banger(cs, OPCODE_RDID);

	ath79_spi_delay_8(cs);
	ath79_spi_delay_8(cs);
	ath79_spi_delay_8(cs);

	rd = ath79_spi_read(AR71XX_SPI_REG_RDS) & 0xffffff;

	ath79_spi_deselect();
	ath79_spi_done();

	return rd;
}

static void ath79_flash_reset(int cs)
{
	ath79_spi_send_instruction(cs, OPCODE_RSTEN);
	ath79_spi_send_instruction(cs, OPCODE_RESET);

	udelay(100);
}

static int ath79_flash_read(int cs, u32 addr, u32 len, void *buf)
{
	int i;
	u32 bytes_left;
	void *write_addr;

	bytes_left = len;
	write_addr = buf;

	ath79_spi_start();
	ath79_spi_deselect();

	ath79_flash_reset(cs);

	if (((addr >> 24) & 0xff) || (((addr + len) >> 24) & 0xff))
	{
		ath79_spi_send_instruction(cs, OPCODE_WREN);
		ath79_spi_bit_banger(cs, OPCODE_WREAR);
		ath79_spi_bit_banger(cs, (addr >> 24) & 0xff);
		ath79_spi_go(cs);
	}

	ath79_spi_bit_banger(cs, OPCODE_READ);
	ath79_spi_send_addr(cs, addr);

	while (bytes_left)
	{
		for (i = 0; i < sizeof (u32); i++)
			ath79_spi_delay_8(cs);

		*(u32 *)write_addr = ath79_spi_read(AR71XX_SPI_REG_RDS);

		write_addr = (void *) ((u32) write_addr + sizeof (u32));
		bytes_left -= sizeof (u32);
	}

	ath79_spi_go(cs);

	ath79_flash_reset(cs);

	ath79_spi_done();
	ath79_spi_deselect();

	return 0;
}

u8 *ath79_get_eeprom(int cs)
{
	int i;
	u32 jedec_id;

	ath79_spi_base = ioremap_nocache(AR71XX_SPI_BASE, AR71XX_SPI_SIZE);

	jedec_id = ath79_flash_read_id(cs);

	for (i = 0; i < ARRAY_SIZE(flash_32m_jedec); i++)
	{
		if (flash_32m_jedec[i] == jedec_id)
			break;
	}

	if (flash_32m_jedec[i] != jedec_id)
		if (cs)
			ath79_flash_read(cs, 0x00ff0000, 0x8000, ath79_eeprom);
		else
			memcpy(ath79_eeprom, (u8 *) KSEG1ADDR(0x1fff0000), 0x8000);
	else
		ath79_flash_read(cs, 0x01ff0000, 0x8000, ath79_eeprom);

	iounmap(ath79_spi_base);
	return (u8 *) ath79_eeprom;
}

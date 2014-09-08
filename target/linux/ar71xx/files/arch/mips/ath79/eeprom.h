#define AR71XX_SPI_CLK_HIGH	(1<<8)
#define AR71XX_SPI_CS_DIS	(AR71XX_SPI_IOC_CS_ALL)
#define AR71XX_SPI_CE0_LOW	(AR71XX_SPI_IOC_CS1 | AR71XX_SPI_IOC_CS2)
#define AR71XX_SPI_CE0_HIGH	(AR71XX_SPI_CE0_LOW | AR71XX_SPI_CLK_HIGH)
#define AR71XX_SPI_CE1_LOW	(AR71XX_SPI_IOC_CS0 | AR71XX_SPI_IOC_CS2)
#define AR71XX_SPI_CE1_HIGH	(AR71XX_SPI_CE1_LOW | AR71XX_SPI_CLK_HIGH)

#define OPCODE_READ		0x03
#define OPCODE_WREN		0x06
#define OPCODE_WREAR		0xc5
#define OPCODE_RSTEN		0x66
#define OPCODE_RESET		0x99
#define OPCODE_RDID		0x9f

#define ath79_spi_read(_phys)		__raw_readl(ath79_spi_base + (_phys))
#define ath79_spi_write(_phys, _val)	__raw_writel((_val), ath79_spi_base + (_phys))

#define ath79_be_msb(_val, _i) (((_val) & (1 << (7 - _i))) >> (7 - _i))


#define ath79_spi_bit_banger(_cs, _byte) do \
	{ \
		int i; \
		u32 ce_low, ce_high; \
		\
		if ((_cs) == 0) \
		{ \
			ce_low = AR71XX_SPI_CE0_LOW; \
			ce_high = AR71XX_SPI_CE0_HIGH; \
		} \
		else if ((_cs) == 1) \
		{ \
			ce_low = AR71XX_SPI_CE1_LOW; \
			ce_high = AR71XX_SPI_CE1_HIGH; \
		} \
		else \
			BUG(); \
		\
		for(i = 0; i < 8; i++) \
		{ \
			ath79_spi_write(AR71XX_SPI_REG_IOC, \
				ce_low | ath79_be_msb(_byte, i)); \
			ath79_spi_write(AR71XX_SPI_REG_IOC, \
				ce_high | ath79_be_msb(_byte, i)); \
		} \
	} while(0)


#define ath79_spi_go(_cs) do \
	{ \
		u32 ce_low;\
		\
		if ((_cs) == 0) \
			ce_low = AR71XX_SPI_CE0_LOW; \
		else if ((_cs) == 1) \
			ce_low = AR71XX_SPI_CE1_LOW; \
		else \
			BUG(); \
		ath79_spi_write(AR71XX_SPI_REG_IOC, ce_low); \
		ath79_spi_write(AR71XX_SPI_REG_IOC, AR71XX_SPI_CS_DIS); \
	} while(0)


#define ath79_spi_send_instruction(_cs, _byte) do \
	{ \
		ath79_spi_bit_banger((_cs), _byte); \
		ath79_spi_go(_cs); \
	} while(0)


#define ath79_spi_send_addr(_cs, _addr) do \
	{ \
		ath79_spi_bit_banger((_cs), ((_addr >> 16) & 0xff)); \
		ath79_spi_bit_banger((_cs), ((_addr >> 8) & 0xff)); \
		ath79_spi_bit_banger((_cs), (_addr) & 0xff); \
	} while(0)


#define ath79_spi_delay_8(_cs)	ath79_spi_bit_banger((_cs), 0)

#define ath79_spi_deselect()	ath79_spi_write(AR71XX_SPI_REG_IOC, AR71XX_SPI_CS_DIS)

#define ath79_spi_start()	ath79_spi_write(AR71XX_SPI_REG_FS, 1)

#define ath79_spi_done()	ath79_spi_write(AR71XX_SPI_REG_FS, 0)

u8 *ath79_get_eeprom(int cs);

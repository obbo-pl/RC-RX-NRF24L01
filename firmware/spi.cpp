/*
 * spi.cpp
 *
 * Created: 2020-05-15 00:03:39
 * Atmel Studio 6 (Version: 6.2.1563 - Service Pack 2)
 * AVR/GNU C Compiler : 4.8.1
 *  Author: Krzysztof Markiewicz
 *  www.obbo.pl
 *
 * This program is distributed under the terms of the GNU General Public License
 */ 

#include <avr/interrupt.h>
#include "spi.h"
#include "macro.h"


#ifdef SPI_CONFIG_H_
	#define SPI_SCK_LOW		SET_PIN_LEVEL_LOW(SPI_SIGNAL_SCK)
	#define SPI_SCK_HIGH		SET_PIN_LEVEL_HIGH(SPI_SIGNAL_SCK)
	#define SPI_MOSI_LOW		SET_PIN_LEVEL_LOW(SPI_SIGNAL_MOSI)
	#define SPI_MOSI_HIGH		SET_PIN_LEVEL_HIGH(SPI_SIGNAL_MOSI)
	#define SPI_MISO		READ_PIN(SPI_SIGNAL_MISO)
#else
	#define SPI_SCK_LOW		clrbit(*(spi.sck.regport), spi.sck.pin)
	#define SPI_SCK_HIGH		setbit(*(spi.sck.regport), spi.sck.pin)
	#define SPI_MOSI_LOW		clrbit(*(spi.mosi.regport), spi.mosi.pin)
	#define SPI_MOSI_HIGH		setbit(*(spi.mosi.regport), spi.mosi.pin)
	#define SPI_MISO		testbit(*(spi.miso.regpin), spi.miso.pin)
#endif

#ifdef SPI_CONFIG_H_
SPI::SPI(void)
{
	SET_PIN_AS_OUT(SPI_SIGNAL_SCK);
	SET_PIN_AS_OUT(SPI_SIGNAL_MOSI);
	SET_PIN_AS_IN(SPI_SIGNAL_MISO);
}
#else
SPI::SPI(uint8_t volatile *sck_regpin, uint8_t volatile *sck_regddr, uint8_t volatile *sck_regport, uint8_t sck, 
	 uint8_t volatile *mosi_regpin, uint8_t volatile *mosi_regddr, uint8_t volatile *mosi_regport, uint8_t mosi, 
	 uint8_t volatile *miso_regpin, uint8_t volatile *miso_regddr, uint8_t volatile *miso_regport, uint8_t miso)
{
	spi.sck.regpin = sck_regpin;
	spi.sck.regport = sck_regport;
	spi.sck.pin = sck;
	spi.mosi.regpin = mosi_regpin;
	spi.mosi.regport = mosi_regport;
	spi.mosi.pin = mosi;
	spi.miso.regpin = miso_regpin;
	spi.miso.regport = miso_regport;
	spi.miso.pin = miso;
	setbit(*sck_regddr, spi.sck.pin);	// output
	setbit(*mosi_regddr, spi.mosi.pin);	// output
	clrbit(*miso_regddr, spi.miso.pin);	// input
}
#endif

void SPI::write(uint8_t command)
{
#ifdef SPI_PAUSE_INTERRUPT
	char cSREG = SREG;
	cli();
#endif
	SPI_SCK_LOW;
	for (uint8_t i = 0; i < 8; i++) {
		if (command & 0x80)
			SPI_MOSI_HIGH;
		else
			SPI_MOSI_LOW;
		nop();
		SPI_SCK_HIGH;
		command = command << 1;
		SPI_SCK_LOW;
		nop();
	}
	SPI_MOSI_HIGH;
#ifdef SPI_PAUSE_INTERRUPT
	SREG = cSREG;
#endif
}

uint8_t SPI::read(void)
{
	uint8_t result = 0;
#ifdef SPI_PAUSE_INTERRUPT
	char cSREG = SREG;
	cli();
#endif
	SPI_SCK_LOW;
	for (uint8_t i = 0; i < 8; i++) {
		result = result << 1;
		if (SPI_MISO) result |= 0x01;
		SPI_SCK_HIGH;
		nop();
		SPI_SCK_LOW;
		nop();
	}
#ifdef SPI_PAUSE_INTERRUPT
	SREG = cSREG;
#endif
	return result;
}


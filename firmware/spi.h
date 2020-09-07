/*
 * spi.h
 *
 * Created: 2020-05-15 00:03:39
 * Atmel Studio 6 (Version: 6.2.1563 - Service Pack 2)
 * AVR/GNU C Compiler : 4.8.1
 *  Author: Krzysztof Markiewicz
 *  www.obbo.pl
 *
 * This program is distributed under the terms of the GNU General Public License
 */ 


#ifndef SPI_H_
#define SPI_H_

#include <avr/io.h>
#include <stdbool.h>
#include "spi_config.h"


//#define SPI_PAUSE_INTERRUPT

class SPI
{
	public:
#ifdef SPI_CONFIG_H_
		SPI(void);
#else
		SPI(uint8_t volatile *sck_regpin, uint8_t volatile *sck_regddr, uint8_t volatile *sck_regport, uint8_t sck,
		    uint8_t volatile *mosi_regpin, uint8_t volatile *mosi_regddr, uint8_t volatile *mosi_regport, uint8_t mosi,
		    uint8_t volatile *miso_regpin, uint8_t volatile *miso_regddr, uint8_t volatile *miso_regport, uint8_t miso);
#endif
		void write(uint8_t command);
		uint8_t read(void);

#ifndef SPI_CONFIG_H_
	private:
		typedef struct {
			uint8_t volatile *regpin; 
			uint8_t volatile *regport;
			uint8_t pin;
		} SPI_Signal_t;

		typedef struct {
			SPI_Signal_t sck;
			SPI_Signal_t mosi;
			SPI_Signal_t miso;
		} SPI_t;
		
		SPI_t spi;
#endif
};


#endif /* SPI_H_ */
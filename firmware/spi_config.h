/*
 * spi_config.h
 *
 * Created: 2020-05-16 22:06:09
 * Atmel Studio 6 (Version: 6.2.1563 - Service Pack 2)
 * AVR/GNU C Compiler : 4.8.1
 *  Author: Krzysztof Markiewicz
 *  www.obbo.pl
 *
 * This program is distributed under the terms of the GNU General Public License
 */ 


#ifndef SPI_CONFIG_H_
#define SPI_CONFIG_H_

#include "config.h"


#define	SPI_SIGNAL_SCK		CONFIG_NRF_SCK
#define SPI_SIGNAL_MOSI		CONFIG_NRF_MOSI
#define	SPI_SIGNAL_MISO		CONFIG_NRF_MISO


#endif /* SPI_CONFIG_H_ */
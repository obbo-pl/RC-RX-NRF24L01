/*
 * config.h
 *
 * Created: 2020-05-10 23:40:32
 * Atmel Studio 6 (Version: 6.2.1563 - Service Pack 2)
 * AVR/GNU C Compiler : 4.8.1
 *  Author: Krzysztof Markiewicz
 *  www.obbo.pl
 *
 * This program is distributed under the terms of the GNU General Public License
 */ 


#ifndef CONFIG_H_
#define CONFIG_H_

// PPM signal
#define CONFIG_CHANNELS_MAX_COUNT	16
#define CONFIG_CHANNEL_MIN_VALUE	1000
#define CONFIG_CHANNEL_MID_VALUE	1500
#define CONFIG_CHANNEL_MAX_VALUE	2000

// Servo signals
#define CONFIG_CH1			B,2
#define CONFIG_CH2			B,3
#define CONFIG_CH3			B,4
#define CONFIG_CH4			B,5
#define CONFIG_CH5			C,0
#define CONFIG_CH6			C,1
#define CONFIG_CH7			C,2
#define CONFIG_CH8			C,3

// Servo channel order
#define	ATER

// SPI signals
#define CONFIG_NRF_MOSI			D,5
#define CONFIG_NRF_MISO			D,6
#define CONFIG_NRF_SCK			D,4

// NRF24L01 radio signals
#define CONFIG_NRF_CSN			B,0
#define CONFIG_NRF_CE			D,7

// Battery voltage measure
#define CONFIG_VBAT_MUX_IN		7

// Bind button
#define CONFIG_BIND_BUTTON		C,3

// LED
#define CONFIG_STATUS_LED		C,2

// Terminal command set
#define TERMINAL_CABELL_FULL_STATISTICS
#define TERMINAL_NRF24L01_RADIO_STATE



#endif /* CONFIG_H_ */


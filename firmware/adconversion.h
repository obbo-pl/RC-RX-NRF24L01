/*
 * adconversion.h
 *
 * Created: 2020-05-11 23:38:16
 * Atmel Studio 6 (Version: 6.2.1563 - Service Pack 2)
 * AVR/GNU C Compiler : 4.8.1
 *  Author: Krzysztof Markiewicz
 *  www.obbo.pl
 *
 * This program is distributed under the terms of the GNU General Public License
 */ 

#ifndef ADCONVERSION_H_
#define ADCONVERSION_H_


#include <avr/io.h>
#include <stdbool.h>


typedef enum  {
	ADCONVERSION_MUX_ADC0 = 0b0000,
	ADCONVERSION_MUX_ADC1 = 0b0001,
	ADCONVERSION_MUX_ADC2 = 0b0010,
	ADCONVERSION_MUX_ADC3 = 0b0011,
	ADCONVERSION_MUX_ADC4 = 0b0100,
	ADCONVERSION_MUX_ADC5 = 0b0101,
	ADCONVERSION_MUX_ADC6 = 0b0110,
	ADCONVERSION_MUX_ADC7 = 0b0111,
#if (defined(__AVR_ATmega88A__) || defined(__AVR_ATmega88PA__) || defined(__AVR_ATmega328__) || defined(__AVR_ATmega328P__))
	ADCONVERSION_MUX_ADC8 = 0b1000,
#endif
	ADCONVERSION_MUX_VBG  = 0b1110,
	ADCONVERSION_MUX_GND  = 0b1111
} ADCInput_t;

	
void adc_Init(ADCInput_t input);
bool adc_Read(uint8_t *val);



#endif /* ADCONVERSION_H_ */
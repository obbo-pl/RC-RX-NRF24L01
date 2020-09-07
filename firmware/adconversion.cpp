/*
 * adconversion.cpp
 *
 * Created: 2020-05-11 23:39:54
 * Atmel Studio 6 (Version: 6.2.1563 - Service Pack 2)
 * AVR/GNU C Compiler : 4.8.1
 *  Author: Krzysztof Markiewicz
 *  www.obbo.pl
 *
 * This program is distributed under the terms of the GNU General Public License
 */ 

#include "adconversion.h"



void adc_Init(ADCInput_t input)
{
#if (defined(__AVR_ATmega8__))
	ADMUX = (1 << REFS0) | (1 << REFS1);					// Internal 2.56V Voltage Reference with external capacitor at AREF pin
#elif (defined(__AVR_ATmega88A__) || defined(__AVR_ATmega88PA__) || defined(__AVR_ATmega328__) || defined(__AVR_ATmega328P__))
	ADMUX = (1 << REFS0) | (1 << REFS1);					// Internal 1.1V Voltage Reference with external capacitor at AREF pin
#endif

	ADMUX |= (input & 0b1111);
	ADMUX |= (1 << ADLAR);							// ADC Left Adjust Result
	ADCSRA = (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);			// ADC Prescaler Select Bits, /128

#if (defined(__AVR_ATmega8__))
	ADCSRA |= (1 << ADFR);							// ADC Free Running Select
#elif (defined(__AVR_ATmega88A__) || defined(__AVR_ATmega88PA__) || defined(__AVR_ATmega328__) || defined(__AVR_ATmega328P__))
	ADCSRA |= (1 << ADATE);							// ADC Auto Trigger Enable
	ADCSRB = 0x00;								// ADC Free Running Select
#endif

	ADCSRA |= (1 << ADEN);							// ADC Enable
	ADCSRA |= (1 << ADSC);							// ADC Start Conversion
}

bool adc_Read(uint8_t *val)
{
	// ADC Left Adjust Result
	bool result = false;
	if ((ADCSRA >> ADIF) & 0x01) {
		*val = ADCH;
		ADCSRA |= (1 << ADIF);
		result = true;
	}
	return result;
}

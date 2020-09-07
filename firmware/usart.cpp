/*
 * usart.cpp
 *
 * Created: 2020-06-22 19:47:41
 * Atmel Studio 6 (Version: 6.2.1563 - Service Pack 2)
 * AVR/GNU C Compiler : 4.8.1
 *  Author: Krzysztof Markiewicz
 *  www.obbo.pl
 *
 * This program is distributed under the terms of the GNU General Public License
 */ 

#include "usart.h"
#include "macro.h"


#define USART_PRESCALE			(((F_CPU / (16UL * USART_BAUDRATE))) - 1)



#define USART_SERIAL_PORT		D
#define USART_SERIAL_Tx_PIN		1
#define USART_SERIAL_Rx_PIN		0



void usart_Init(void)
{
	SET_PIN_AS_OUT_(USART_SERIAL_PORT,USART_SERIAL_Tx_PIN);
	SET_PIN_AS_IN_(USART_SERIAL_PORT,USART_SERIAL_Rx_PIN);
	SET_PIN_PULLUP_(USART_SERIAL_PORT,USART_SERIAL_Rx_PIN);

#if (defined(__AVR_ATmega8__))
	UBRRH = (uint8_t)(USART_PRESCALE >> 8);
	UBRRL = (uint8_t)(USART_PRESCALE);
	UCSRB = (1 << RXCIE) | (1 << RXEN) | (1 << TXEN);		// RX Complete Interrupt Enable, Receiver Enable, Transmitter Enable
	UCSRC = (1 << URSEL) | (1 << UCSZ0) | (1 << UCSZ1);     // Asynchronous Operation, Parity Mode Disabled, Stop Bit: 1-bit, Character Size: 8-bit
#elif (defined(__AVR_ATmega88A__) || defined(__AVR_ATmega88PA__) || defined(__AVR_ATmega328__) || defined(__AVR_ATmega328P__))
	UBRR0H = (uint8_t)(USART_PRESCALE >> 8);
	UBRR0L = (uint8_t)(USART_PRESCALE);
	UCSR0B = (1 << RXCIE0) | (1 << RXEN0) | (1 << TXEN0);	// RX Complete Interrupt Enable, Receiver Enable, Transmitter Enable
	UCSR0C = (1 << UCSZ00) | (1 << UCSZ01);					// Asynchronous Operation, Parity Mode Disabled, Stop Bit: 1-bit, Character Size: 8-bit
#endif
}

bool usart_SendChar(char data)
{
#if (defined(__AVR_ATmega8__))
	if (UCSRA & (1 << UDRE)) {
		UDR = data;
		return true;
	}
#elif (defined(__AVR_ATmega88A__) || defined(__AVR_ATmega88PA__) || defined(__AVR_ATmega328__) || defined(__AVR_ATmega328P__))
	if (UCSR0A & (1 << UDRE0)) {
		UDR0 = data;
		return true;
	}
#endif
	return false;
}

char usart_ReadChar(void)
{
#if (defined(__AVR_ATmega8__))
	return UDR;
#elif (defined(__AVR_ATmega88A__) || defined(__AVR_ATmega88PA__) || defined(__AVR_ATmega328__) || defined(__AVR_ATmega328P__))
	return UDR0;
#endif
}
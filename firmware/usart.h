/*
 * usart.h
 *
 * Created: 2020-06-22 19:47:41
 * Atmel Studio 6 (Version: 6.2.1563 - Service Pack 2)
 * AVR/GNU C Compiler : 4.8.1
 *  Author: Krzysztof Markiewicz
 *  www.obbo.pl
 *
 * This program is distributed under the terms of the GNU General Public License
 */ 


#ifndef USART_H_
#define USART_H_


#include <stdbool.h>
#include <avr/io.h>


#define USART_BAUDRATE			(9600)



void usart_Init(void);
bool usart_SendChar(char data);
char usart_ReadChar(void);




#endif /* USART_H_ */
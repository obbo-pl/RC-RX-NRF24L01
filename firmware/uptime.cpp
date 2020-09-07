/*
 * uptime.cpp
 *
 * Created: 2020-05-28 21:17:19
 * Atmel Studio 6 (Version: 6.2.1563 - Service Pack 2)
 * AVR/GNU C Compiler : 4.8.1
 *  Author: Krzysztof Markiewicz
 *  www.obbo.pl
 *
 * This program is distributed under the terms of the GNU General Public License
 */ 

#include "uptime.h"


UpTime::UpTime(void)
{
	counter = 0;
}

void UpTime::interruptHandler(void)
{
	counter++;
}

uint32_t UpTime::getTime(void)
{
	return counter;
}
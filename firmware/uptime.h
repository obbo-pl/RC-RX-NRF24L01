/*
 * uptime.h
 *
 * Created: 2020-05-28 21:17:01
 * Atmel Studio 6 (Version: 6.2.1563 - Service Pack 2)
 * AVR/GNU C Compiler : 4.8.1
 *  Author: Krzysztof Markiewicz
 *  www.obbo.pl
 *
 * This program is distributed under the terms of the GNU General Public License
 */ 


#ifndef UPTIME_H_
#define UPTIME_H_

#include <avr/io.h>


class UpTime
{
	public:
		UpTime(void);
		
		void interruptHandler(void);
		uint32_t getTime(void);
		
	private:
		volatile uint32_t counter;
};



#endif /* UPTIME_H_ */
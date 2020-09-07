/*
 * led.h
 *
 * Created: 2020-06-05 21:35:09
 * Atmel Studio 6 (Version: 6.2.1563 - Service Pack 2)
 *  Author: Krzysztof Markiewicz
 *  www.obbo.pl
 *
 * This program is distributed under the terms of the GNU General Public License
 */ 


#ifndef LED_H_
#define LED_H_

#include <stdint.h>
#include <stdbool.h>



class LED
{
	public:
		LED(uint8_t volatile *led_regddr, uint8_t volatile *led_regport, uint8_t pin);

		void init(void);
		void update(void);
		void clear(void);
		void setEnabled(bool enabled);
		void setBlink(bool blink);
		void setLed(bool enabled, bool blink);
		void setTimers(uint8_t period, uint8_t duty);

	private:
		typedef struct {
			uint8_t volatile *regddr;
			uint8_t volatile *regport;
			uint8_t pin;
		} LED_Port_t;

		typedef struct {
			LED_Port_t port;
			uint8_t counter;
			uint8_t setup;
			uint8_t period;
			uint8_t duty;
		} LED_t;
	
		LED_t led;
};


#endif /* LED_H_ */


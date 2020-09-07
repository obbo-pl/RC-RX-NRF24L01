/*
 * led.cpp
 *
 * Created: 2020-06-05 21:35:09
 * Atmel Studio 6 (Version: 6.2.1563 - Service Pack 2)
 *  Author: Krzysztof Markiewicz
 *  www.obbo.pl
 *
 * This program is distributed under the terms of the GNU General Public License
 */ 

#include "led.h"
#include "macro.h"
#include <avr/interrupt.h>


enum led_setup {
 	LED_STATE_On,
	LED_STATE_Blink,
};
#define LED_LOW			{char cSREG = SREG; cli(); clrbit(*(led.port.regport), led.port.pin); SREG = cSREG;}
#define LED_HIGH		{char cSREG = SREG; cli(); setbit(*(led.port.regport), led.port.pin); SREG = cSREG;}

LED::LED(uint8_t volatile *led_regddr, uint8_t volatile *led_regport, uint8_t pin)
{
	led.port.regddr = led_regddr;
	led.port.regport = led_regport;
	led.port.pin = pin;
	led.counter = 0;
	led.period = 100;
	led.duty = 50;
	led.setup = 0x00;
}

void LED::init(void)
{
	setbit(*led.port.regddr, led.port.pin);
	led.counter = 0;
}

void LED::update(void)
{
	if (testbit(led.setup, LED_STATE_On)) {
		if (testbit(led.setup, LED_STATE_Blink)) {
			led.counter++;
			if (led.counter >= led.period) led.counter = 0;
			if (led.duty > led.counter) {
				LED_HIGH;
			} else {
				LED_LOW;
			}
		} else {
			LED_HIGH;
		}
	} else {
		LED_LOW;
	}
}

void LED::clear(void)
{
	led.counter = 0;
}

void LED::setEnabled(bool enabled)
{
	if (enabled) {
		setbit(led.setup, LED_STATE_On);
	} else {
		clrbit(led.setup, LED_STATE_On);
	}
}

// Sets LED light blinking or solid
void LED::setBlink(bool blink)
{
	if (blink) {
		setbit(led.setup, LED_STATE_Blink);
	} else {
		clrbit(led.setup, LED_STATE_Blink);
	}
}

void LED::setLed(bool enabled, bool blink)
{
	setEnabled(enabled);
	setBlink(blink);
}

void LED::setTimers(uint8_t period, uint8_t duty) 
{
	led.period = period;
	led.duty = duty;
}


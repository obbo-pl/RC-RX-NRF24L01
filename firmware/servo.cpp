/*
 * servo.cpp
 *
 * Created: 2020-05-12 21:53:08
 * Atmel Studio 6 (Version: 6.2.1563 - Service Pack 2)
 * AVR/GNU C Compiler : 4.8.1
 *  Author: Krzysztof Markiewicz
 *  www.obbo.pl
 *
 * This program is distributed under the terms of the GNU General Public License
 */ 

#include <stdlib.h>
#include <util/delay.h>
#include "servo.h"
#include "macro.h"


#define SERVO_CHECK_PORT_DELAY_ms		(50)
#define SERVO_CHAIN_CHANNEL_DELAY		(SERVO_MAX_PULSE_PERIOD / SERVO_CHAIN_MAX_LENGTH)
#define SERVO_CHAIN_PULSE_CORRECTION		(9)
#define SERVO_CHAIN_MINIMUM_TIMER_VALUE		(4)


#ifndef SERVO_CHAIN_STROBE_BY_16BIT_0us5_TIMER
#if SERVO_MIN_PULSE_WIDTH < 1000
#error "For an 8-bit strobe timer, the minimum pulse length must not be less than 1000"
#endif
#if SERVO_MAX_PULSE_WIDTH > 2000
#error "For an 8-bit strobe timer, the maximum pulse length cannot be greater than 2000"
#endif
#if SERVO_CHAIN_MINIMUM_TIMER_VALUE > 5
#error "For an 8-bit strobe timer, the minimum timer value cannot be greater than 5"
#endif
#endif


// Class Servo
Servo::Servo(uint8_t volatile *regpin, uint8_t volatile *regddr, uint8_t volatile *regport, uint8_t pin)
{
	servo.out.regpin = regpin;
	servo.out.regddr = regddr;
	servo.out.regport = regport;
	servo.out.pin = pin;
}

void Servo::attach(void)
{
	clrbit(*(servo.out.regport), servo.out.pin);
	setbit(*(servo.out.regddr), servo.out.pin);
}

void Servo::detach(void)
{
	clrbit(*(servo.out.regddr), servo.out.pin);
}

void Servo::startPulse(void)
{
	setbit(*(servo.out.regport), servo.out.pin);
}

void Servo::stopPulse(void)
{
	clrbit(*(servo.out.regport), servo.out.pin);
}

bool Servo::checkPort(void)
{
	bool result = true;
	clrbit(*(servo.out.regddr), servo.out.pin);
	setbit(*(servo.out.regport), servo.out.pin);
	_delay_ms(SERVO_CHECK_PORT_DELAY_ms);
	if (!(testbit(*(servo.out.regpin), servo.out.pin))) result = false;
	return result;
}



// Class Servo_Chain
ServoChain::ServoChain(void)
{
	for (uint8_t i = 0; i < SERVO_CHAIN_MAX_LENGTH; i++) {
		servos[i] = NULL;
	}
	current_servo = 0;
	phase = SERVO_CHAIN_PULSE_BASE;
	pulses_enabled = false;
}

void ServoChain::bind(Servo *ch, uint8_t channel)
{
	channel--;
	if (channel < SERVO_CHAIN_MAX_LENGTH) {
		servos[channel] = ch;
	}
}

void ServoChain::unbind(uint8_t channel)
{
	channel--;
	if (channel < SERVO_CHAIN_MAX_LENGTH) {
		servos[channel] = NULL;
	}
}

void ServoChain::attach(void)
{
	for (uint8_t i = 0; i < SERVO_CHAIN_MAX_LENGTH; i++) {
		if (servos[i]) servos[i]->attach();
	}
}

void ServoChain::detach(void)
{
	for (uint8_t i = 0; i < SERVO_CHAIN_MAX_LENGTH; i++) {
		if (servos[i]) servos[i]->detach();
	}
}

uint8_t ServoChain::checkPort(void)
{
	uint8_t result = 0xFF;
	for (uint8_t i = 0; i < SERVO_CHAIN_MAX_LENGTH; i++) {
		if (servos[i]) {
			if (!(servos[i]->checkPort())) clrbit(result, i);
		}
	}
	return result;
}

void ServoChain::enablePulses(bool enable)
{
	pulses_enabled = enable;
}

void ServoChain::setValues(uint16_t *values)
{
	for (uint8_t i = 0; i < SERVO_CHAIN_MAX_LENGTH; i++) {
		uint16_t temp_value = values[i];
		if (temp_value < SERVO_MIN_PULSE_WIDTH) temp_value = SERVO_MIN_PULSE_WIDTH;
		else if (temp_value > SERVO_MAX_PULSE_WIDTH) temp_value = SERVO_MAX_PULSE_WIDTH;
#ifdef SERVO_CHAIN_STROBE_BY_16BIT_0us5_TIMER
		pulse_length[i] =  2 * (temp_value - SERVO_CHAIN_PULSE_CORRECTION);
#else
		pulse_length[i] = (uint8_t)((temp_value - SERVO_MIN_PULSE_WIDTH) / 4);
#endif
	}
}

#ifdef SERVO_CHAIN_STROBE_BY_16BIT_0us5_TIMER
void ServoChain::interruptHandler(void (*start_timer_0us5)(uint16_t))
{
	switch(phase) {
		case SERVO_CHAIN_PULSE_BASE:
			phase = SERVO_CHAIN_PULSE_PAUSE;
			if (servos[current_servo]) {
				if (pulses_enabled) servos[current_servo]->startPulse();
				pause = 2 * (SERVO_CHAIN_CHANNEL_DELAY - SERVO_CHAIN_PULSE_CORRECTION) - pulse_length[current_servo];
				start_timer_0us5(pulse_length[current_servo]);
			} else {
				pause = SERVO_CHAIN_CHANNEL_DELAY - SERVO_CHAIN_PULSE_CORRECTION;
				start_timer_0us5(SERVO_CHAIN_CHANNEL_DELAY - SERVO_CHAIN_PULSE_CORRECTION);
			}
			break;
		case SERVO_CHAIN_PULSE_PAUSE:
			if (servos[current_servo]) servos[current_servo]->stopPulse();
			phase = SERVO_CHAIN_PULSE_BASE;
			nextServo();
			start_timer_0us5(pause);
			break;
	}
}

#else
void ServoChain::interruptHandler(void (*start_timer_4us)(uint8_t))
{
	switch(phase) {
		case SERVO_CHAIN_PULSE_BASE:
			phase = SERVO_CHAIN_PULSE_EXTENSION;
			if (servos[current_servo]) {
				if (pulses_enabled) servos[current_servo]->startPulse();
			}
			start_timer_4us((SERVO_MIN_PULSE_WIDTH - SERVO_CHAIN_PULSE_CORRECTION) / 4 - SERVO_CHAIN_MINIMUM_TIMER_VALUE);
			break;
		case SERVO_CHAIN_PULSE_EXTENSION:
			phase = SERVO_CHAIN_PULSE_PAUSE;
			if (servos[current_servo]) {
				pause = ((SERVO_MIN_PULSE_WIDTH / 4) - pulse_length[current_servo]) + SERVO_CHAIN_MINIMUM_TIMER_VALUE;
				start_timer_4us(pulse_length[current_servo] + SERVO_CHAIN_MINIMUM_TIMER_VALUE);
			} else {
				pause = SERVO_MIN_PULSE_WIDTH / 8 + SERVO_CHAIN_MINIMUM_TIMER_VALUE;
				start_timer_4us(SERVO_MIN_PULSE_WIDTH / 8 + SERVO_CHAIN_MINIMUM_TIMER_VALUE);
			}
			break;
		case SERVO_CHAIN_PULSE_PAUSE:
			if (servos[current_servo]) servos[current_servo]->stopPulse();
			phase = SERVO_CHAIN_PAUSE_BASE;
			nextServo();
			start_timer_4us(pause);
			break;
		case SERVO_CHAIN_PAUSE_BASE:
			phase = SERVO_CHAIN_PULSE_BASE;
			start_timer_4us((SERVO_MIN_PULSE_WIDTH - 2 * SERVO_CHAIN_PULSE_CORRECTION) / 8 - SERVO_CHAIN_MINIMUM_TIMER_VALUE);
			break;
	}
}
#endif

void ServoChain::nextServo(void)
{
	current_servo++;
	if (current_servo >= SERVO_CHAIN_MAX_LENGTH) current_servo = 0;
}
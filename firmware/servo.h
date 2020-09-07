/*
 * servo.h
 *
 * Created: 2020-05-12 21:52:53
 * Atmel Studio 6 (Version: 6.2.1563 - Service Pack 2)
 * AVR/GNU C Compiler : 4.8.1
 *  Author: Krzysztof Markiewicz
 *  www.obbo.pl
 *
 * This program is distributed under the terms of the GNU General Public License
 */ 


#ifndef SERVO_H_
#define SERVO_H_

#include <avr/io.h>
#include <stdbool.h>
#include "config.h"

#define SERVO_MIN_PULSE_WIDTH			(CONFIG_CHANNEL_MIN_VALUE)
#define SERVO_MAX_PULSE_WIDTH			(CONFIG_CHANNEL_MAX_VALUE)
#define SERVO_MAX_PULSE_PERIOD			(20000)

#define SERVO_CHAIN_MAX_LENGTH			(8)

#define SERVO_CHAIN_STROBE_BY_16BIT_0us5_TIMER


class Servo
{
	public:
		Servo(uint8_t volatile *regpin, uint8_t volatile *regddr, uint8_t volatile *regport, uint8_t pin);

		void attach(void);
		void detach(void);
		void startPulse(void);
		void stopPulse(void);
		bool checkPort(void);
		
	private:
		typedef struct {
			uint8_t volatile *regpin;
			uint8_t volatile *regddr;
			uint8_t volatile *regport;
			uint8_t pin;
		} ServoOut_t   ;

		typedef struct {
			ServoOut_t out;
		} Servo_t;
		
		Servo_t servo;
};


class ServoChain
{
	public:
		ServoChain(void);
		
		void bind(Servo *ch, uint8_t channel);
		void unbind(uint8_t channel);
		void attach(void);
		void detach(void);
		uint8_t checkPort(void);
		void setValues(uint16_t *values);
		void enablePulses(bool enable);
#ifdef SERVO_CHAIN_STROBE_BY_16BIT_0us5_TIMER
		void interruptHandler(void (*start_timer_0us5)(uint16_t));
#else
		void interruptHandler(void (*start_timer_4us)(uint8_t));
#endif

	private:
		enum {
			SERVO_CHAIN_PULSE_BASE,
			SERVO_CHAIN_PULSE_EXTENSION,
			SERVO_CHAIN_PULSE_PAUSE,
			SERVO_CHAIN_PAUSE_BASE
		};
	
		Servo *servos[SERVO_CHAIN_MAX_LENGTH];
#ifdef SERVO_CHAIN_STROBE_BY_16BIT_0us5_TIMER
		uint16_t pulse_length[SERVO_CHAIN_MAX_LENGTH];
		uint16_t pause;
#else
		uint8_t pulse_length[SERVO_CHAIN_MAX_LENGTH];
		uint8_t pause;
#endif
		uint8_t current_servo;
		uint8_t phase;
		bool pulses_enabled;
		void nextServo(void);

};


#endif /* SERVO_H_ */
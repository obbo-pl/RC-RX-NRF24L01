/*
 * throttle.h
 *
 * Created: 2020-09-03 19:41:37
 * Atmel Studio 6 (Version: 6.2.1563 - Service Pack 2)
 * AVR/GNU C Compiler : 4.8.1
 *  Author: Krzysztof Markiewicz
 *  www.obbo.pl
 *
 * This program is distributed under the terms of the GNU General Public License
 */ 


#ifndef THROTTLE_H_
#define THROTTLE_H_

#include <avr/io.h>
#include <stdbool.h>
#include "config.h"
#include "uptime.h"
#include "cbuffer.h"

#define THROTTLE_MAX_CHANNELS			(CONFIG_CHANNELS_MAX_COUNT)
#define THROTTLE_MIN_VALUE			(CONFIG_CHANNEL_MIN_VALUE)
#define THROTTLE_MAX_VALUE			(CONFIG_CHANNEL_MAX_VALUE)
#define THROTTLE_DISARM_DEFAULT_VALUE		(CONFIG_CHANNEL_MIN_VALUE)
#define THROTTLE_DISARM_VALUE_LATITUDE		50
#define THROTTLE_DISARM_TIMEOUT			300000UL	// If no packet received in this time frame disarm the throttle.
#define THROTTLE_ARMING_DURATION		150


class Throttle
{
	public:
		Throttle();

		void init(uint8_t channel, uint16_t disarm_value, UpTime *uptime);
		void check(uint16_t *channels_value, uint32_t last_good_packet, bool initial_packet, bool failsafe);
		void saveDisarmValue(void);
		void saveDisarmValue(uint16_t disarm_value);
		void setDisarmValue(uint16_t disarm_value);
		void clearArmed(void);
		bool isArmed(void);
		uint16_t getDisarmValue(void);
		void bindTerminal(CircularBuffer_t *console);
				
	private:
		UpTime *uptime;
		CircularBuffer_t *console;
		bool armed;
		uint16_t throttle_disarm_value;
		uint8_t throttle_channel;
		uint16_t arming_duration;
		void setArmed(void);
		
};


#endif /* THROTTLE_H_ */
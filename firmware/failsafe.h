/*
 * failsafe.h
 *
 * Created: 2020-09-05 20:35:45
 * Atmel Studio 6 (Version: 6.2.1563 - Service Pack 2)
 * AVR/GNU C Compiler : 4.8.1
 *  Author: Krzysztof Markiewicz
 *  www.obbo.pl
 *
 * This program is distributed under the terms of the GNU General Public License
 */ 


#ifndef FAILSAFE_H_
#define FAILSAFE_H_

#include <avr/io.h>
#include <stdbool.h>
#include "config.h"
#include "uptime.h"


#define FAILSAFE_MAX_CHANNELS				(CONFIG_CHANNELS_MAX_COUNT)
#define FAILSAFE_CHANNEL_MIN_VALUE			(CONFIG_CHANNEL_MIN_VALUE)
#define FAILSAFE_CHANNEL_MAX_VALUE			(CONFIG_CHANNEL_MAX_VALUE)
#define FAILSAFE_CHANNEL_MID_VALUE			((CONFIG_CHANNEL_MIN_VALUE + CONFIG_CHANNEL_MAX_VALUE) / 2)

#define FAILSAFE_CONNECTION_TIMEOUT			10000		// If no packet received in this time frame apply failsafe settings.


enum FAILSAFE_STATE {
	FAILSAFE_STATE_FAILSAFE_MODE,
	FAILSAFE_STATE_MODE_PULSES,
	FAILSAFE_STATE_CONFIGURE_ENABLED
};


class Failsafe
{
	public:
		Failsafe();
	
		uint16_t servo_channels[FAILSAFE_MAX_CHANNELS];
		uint8_t state;
		void init(UpTime *uptime);
		void setConfigurationEnabled(void);
		void saveValues(void);
		void saveValues(uint16_t *values);
		void check(uint16_t *channels_value, bool *enable_pulses, uint32_t last_good_packet, bool initial_packet);
		void setDefaultValues(void);
		void eraseValues(void);
		void saveModePulses(void);
		void saveModePulses(bool pulses);
		void getState(uint8_t *state);

	private:
		UpTime *uptime;
		void loadValues(void);
		void setValue(uint8_t channel, uint16_t val);
		void readModePulses(void);

};




#endif /* FAILSAFE_H_ */
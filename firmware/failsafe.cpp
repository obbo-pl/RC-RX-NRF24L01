/*
 * failsafe.cpp
 *
 * Created: 2020-09-05 20:35:58
 * Atmel Studio 6 (Version: 6.2.1563 - Service Pack 2)
 * AVR/GNU C Compiler : 4.8.1
 *  Author: Krzysztof Markiewicz
 *  www.obbo.pl
 *
 * This program is distributed under the terms of the GNU General Public License
 */ 

#include "failsafe.h"
#include <avr/eeprom.h>
#include "macro.h"


uint16_t EEMEM eeprom_failsafe_values[FAILSAFE_MAX_CHANNELS];
#define FAILSAFE_CHANNEL_ERASED_VALUE		0xFFFF


Failsafe::Failsafe()
{
	this->state = (uint8_t)(1 << FAILSAFE_STATE_FAILSAFE_CONFIGURE_ENABLED);
}

void Failsafe::init(UpTime *uptime)
{
	this->uptime = uptime;
	uint16_t failsafe_value_ch1 = eeprom_read_word(&eeprom_failsafe_values[0]);
	if ((failsafe_value_ch1 < FAILSAFE_CHANNEL_MIN_VALUE) || (failsafe_value_ch1 > FAILSAFE_CHANNEL_MAX_VALUE)) {
		this->setDefaultValues();
	} else {
		this->loadValues();
	}
}

void Failsafe::setDefaultValues(void)
{
	for (uint8_t i = 0; i < FAILSAFE_MAX_CHANNELS; i++) {
		this->setValue(i, FAILSAFE_CHANNEL_MID_VALUE);
	}
	seveValues();
}

void Failsafe::setErasedValues(void)
{
	for (uint8_t i = 0; i < FAILSAFE_MAX_CHANNELS; i++) {
		this->setValue(i, FAILSAFE_CHANNEL_ERASED_VALUE);
	}
	seveValues();
}

void Failsafe::loadValues(void)
{
	for (uint8_t i = 0; i < FAILSAFE_MAX_CHANNELS; i++) {
		this->setValue(i, eeprom_read_word(&eeprom_failsafe_values[i]));
	}
}

void Failsafe::seveValues(void)
{
	for (uint8_t i = 0; i < FAILSAFE_MAX_CHANNELS; i++) {
		eeprom_write_word(&eeprom_failsafe_values[i], this->servo_channels[i]);
	}
}

void Failsafe::setValue(uint8_t channel, uint16_t val)
{
	if (val < FAILSAFE_CHANNEL_MIN_VALUE) val = (uint16_t)FAILSAFE_CHANNEL_MIN_VALUE;
	if (val > FAILSAFE_CHANNEL_MAX_VALUE) val = (uint16_t)FAILSAFE_CHANNEL_MAX_VALUE;
	this->servo_channels[channel] = val;
}

void Failsafe::setConfigurationEnabled(void)
{
	setbit(this->state, FAILSAFE_STATE_FAILSAFE_CONFIGURE_ENABLED);
}

void Failsafe::setNoPulses(void)
{
	setbit(this->state, FAILSAFE_STATE_FAILSAFE_NO_PULSES);
}

void Failsafe::seveValues(uint16_t *values)
{
	if (testbit(this->state, FAILSAFE_STATE_FAILSAFE_CONFIGURE_ENABLED)) {
		for (uint8_t i = 0; i < FAILSAFE_MAX_CHANNELS; i++) {
			this->setValue(i, values[i]);
		}
		this->seveValues();
		clrbit(this->state, FAILSAFE_STATE_FAILSAFE_CONFIGURE_ENABLED);
	}
}

void Failsafe::check(uint16_t *channels_value, bool *enable_pulses, uint32_t last_good_packet, bool initial_packet)
{
	if (((this->uptime->getTime() - last_good_packet) > (uint32_t)FAILSAFE_CONNECTION_TIMEOUT) || !(initial_packet)) {
		setbit(this->state, FAILSAFE_STATE_FAILSAFE_MODE);
		if (testbit(this->state, FAILSAFE_STATE_FAILSAFE_NO_PULSES)) *enable_pulses = false;
		else *enable_pulses = true;
		for (uint8_t i = 0; i < FAILSAFE_MAX_CHANNELS; i++) {
			channels_value[i] = this->servo_channels[i];
		}
	} else {
		clrbit(this->state, FAILSAFE_STATE_FAILSAFE_MODE);
		*enable_pulses = true;
	}
}

void Failsafe::getState(uint8_t *state)
{
	*state = this->state;
}

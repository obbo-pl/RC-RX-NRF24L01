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


#define FAILSAFE_CHANNEL_ERASED_VALUE		0xFFFF
#define FAILSAFE_PULSES_DISABLED		0x00
#define FAILSAFE_PULSES_ENABLED			0xFF

uint16_t EEMEM eeprom_failsafe_values[FAILSAFE_MAX_CHANNELS];
uint8_t EEMEM eeprom_failsafe_pulse_mode = FAILSAFE_PULSES_ENABLED;



Failsafe::Failsafe()
{
	this->uptime = NULL;
	this->state = (uint8_t)(1 << FAILSAFE_STATE_CONFIGURE_ENABLED);
	readPulseMode();
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
	saveValues();
}

void Failsafe::eraseValues(void)
{
	for (uint8_t i = 0; i < FAILSAFE_MAX_CHANNELS; i++) {
		this->setValue(i, FAILSAFE_CHANNEL_ERASED_VALUE);
	}
	saveValues();
	savePulseMode(true);
}

void Failsafe::loadValues(void)
{
	for (uint8_t i = 0; i < FAILSAFE_MAX_CHANNELS; i++) {
		this->setValue(i, eeprom_read_word(&eeprom_failsafe_values[i]));
	}
}

void Failsafe::saveValues(void)
{
	for (uint8_t i = 0; i < FAILSAFE_MAX_CHANNELS; i++) {
		eeprom_write_word(&eeprom_failsafe_values[i], this->servo_channels[i]);
	}
}

void Failsafe::saveValues(uint16_t *values)
{
	if (testbit(this->state, FAILSAFE_STATE_CONFIGURE_ENABLED)) {
		for (uint8_t i = 0; i < FAILSAFE_MAX_CHANNELS; i++) {
			this->setValue(i, values[i]);
		}
		this->saveValues();
		clrbit(this->state, FAILSAFE_STATE_CONFIGURE_ENABLED);
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
	setbit(this->state, FAILSAFE_STATE_CONFIGURE_ENABLED);
}

void Failsafe::savePulseMode(void)
{
	if (testbit(this->state, FAILSAFE_STATE_PULSE_ENABLED)) {
		eeprom_write_byte(&eeprom_failsafe_pulse_mode, FAILSAFE_PULSES_ENABLED);
	} else {
		eeprom_write_byte(&eeprom_failsafe_pulse_mode, FAILSAFE_PULSES_DISABLED);
	}
}

void Failsafe::savePulseMode(bool pulses_enabled)
{
	if (pulses_enabled) {
		setbit(this->state, FAILSAFE_STATE_PULSE_ENABLED);
	} else {
		clrbit(this->state, FAILSAFE_STATE_PULSE_ENABLED);
	}
	savePulseMode();
}

void Failsafe::readPulseMode(void)
{
	if (eeprom_read_byte(&eeprom_failsafe_pulse_mode) == FAILSAFE_PULSES_ENABLED) {
		setbit(this->state, FAILSAFE_STATE_PULSE_ENABLED);
	} else {
		clrbit(this->state, FAILSAFE_STATE_PULSE_ENABLED);
	}
}

bool Failsafe::isPulseEnabled(void)
{
	if (testbit(this->state, FAILSAFE_STATE_PULSE_ENABLED)) {
		return true;
	} else {
		return false;
	} 
}

void Failsafe::check(uint16_t *channels_value, bool *enable_pulses, uint32_t last_good_packet, bool initial_packet)
{
	if (this->uptime) {
		if (((this->uptime->getTime() - last_good_packet) > (uint32_t)FAILSAFE_CONNECTION_TIMEOUT) || !(initial_packet)) {
			setbit(this->state, FAILSAFE_STATE_ACTIVE);
			if (testbit(this->state, FAILSAFE_STATE_PULSE_ENABLED)) {
				*enable_pulses = true;
			} else {
				*enable_pulses = false;
			}
			for (uint8_t i = 0; i < FAILSAFE_MAX_CHANNELS; i++) {
				channels_value[i] = this->servo_channels[i];
			}
		} else {
			clrbit(this->state, FAILSAFE_STATE_ACTIVE);
			*enable_pulses = true;
		}
	}
}

void Failsafe::getState(uint8_t *state)
{
	*state = this->state;
}

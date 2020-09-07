/*
 * throttle.cpp
 *
 * Created: 2020-09-03 19:42:00
 * Atmel Studio 6 (Version: 6.2.1563 - Service Pack 2)
 * AVR/GNU C Compiler : 4.8.1
 *  Author: Krzysztof Markiewicz
 *  www.obbo.pl
 *
 * This program is distributed under the terms of the GNU General Public License
 */ 


#include "throttle.h"
#include <avr/eeprom.h>
#include "terminal.h"


#define THROTTLE_CHANNEL_NOT_CONFIGURED		(-1)

uint16_t EEMEM eeprom_throttle_disarm_value = THROTTLE_DISARM_DEFAULT_VALUE;



Throttle::Throttle()
{
	this->armed = false;
	this->throttle_channel = THROTTLE_CHANNEL_NOT_CONFIGURED;
	this->throttle_disarm_value = eeprom_read_word(&eeprom_throttle_disarm_value);
	this->arming_duration = 0;
}

void Throttle::init(uint8_t channel, uint16_t disarm_value, UpTime *uptime)
{
	this->uptime = uptime;
	if (channel < (uint8_t)THROTTLE_MAX_CHANNELS) this->throttle_channel = channel;
	this->setDisarmValue(disarm_value);
}

void Throttle::check(uint16_t *channels_value, uint32_t last_good_packet, bool initial_packet, bool failsafe)
{
	if (this->throttle_channel != THROTTLE_CHANNEL_NOT_CONFIGURED) {
		if (((this->uptime->getTime() - last_good_packet) > (uint32_t)THROTTLE_DISARM_TIMEOUT) || !(initial_packet)) {
			if (this->isArmed()) {
				if (this->console) terminal_Print(this->console, "Disarming throttle\n\r", false);
			}
			this->clearArmed();
		}
		if (!(this->isArmed())) {
			if ((channels_value[this->throttle_channel] > this->throttle_disarm_value - THROTTLE_DISARM_VALUE_LATITUDE) 
				&& ((channels_value[this->throttle_channel] < this->throttle_disarm_value + THROTTLE_DISARM_VALUE_LATITUDE))) {
				if (!(failsafe)) this->arming_duration++;
			} else {
				this->arming_duration = 0;
			}
			if (this->arming_duration >= THROTTLE_ARMING_DURATION) {
				if (this->console) terminal_Print(this->console, "Arming throttle\n\r", false);
				this->setArmed();
			}
			channels_value[this->throttle_channel] = this->throttle_disarm_value;
		}
	}
}

void Throttle::saveDisarmValue(void)
{
	eeprom_write_word(&eeprom_throttle_disarm_value, this->throttle_disarm_value);
}

void Throttle::saveDisarmValue(uint16_t disarm_value)
{
	this->setDisarmValue(disarm_value);
	this->saveDisarmValue();
}

void Throttle::setDisarmValue(uint16_t disarm_value)
{
	this->throttle_disarm_value = disarm_value;
	if (this->throttle_disarm_value < THROTTLE_MIN_VALUE) this->throttle_disarm_value = THROTTLE_MIN_VALUE;
	if (this->throttle_disarm_value > THROTTLE_MAX_VALUE) this->throttle_disarm_value = THROTTLE_MAX_VALUE;
}

uint16_t Throttle::getDisarmValue(void)
{
	return this->throttle_disarm_value;
}

void Throttle::clearArmed(void)
{
	this->armed = false;
	this->arming_duration = 0;
}

void Throttle::setArmed(void)
{
	this->armed = true;
}

bool Throttle::isArmed(void)
{
	return this->armed;
}

void Throttle::bindTerminal(CircularBuffer_t *console)
{
	this->console = console;
}

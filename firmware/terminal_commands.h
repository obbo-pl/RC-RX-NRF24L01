/*
 * terminal_commands.h
 *
 * Created: 2020-06-15 19:32:51
 * Atmel Studio 6 (Version: 6.2.1563 - Service Pack 2)
 * AVR/GNU C Compiler : 4.8.1
 *  Author: Krzysztof Markiewicz
 *  www.obbo.pl
 *
 * This program is distributed under the terms of the GNU General Public License
 */ 


#ifndef TERMINAL_COMMANDS_H_
#define TERMINAL_COMMANDS_H_

#include <avr/io.h>
#include "terminal.h"
#include "CABELL_nrf224l01.h"
#include "nrf24l01_spi.h"
#include "throttle.h"
#include "failsafe.h"



void terminal_commands_receiver_Init(CABELL *radio_protocol, 
				     uint16_t *servo_channels,
				     const char *device_info, 
				     uint8_t *battery_voltage, 
				     uint8_t *battery_divider,
				     NRF24L01 *radio);

void terminal_commands_throttle_Init(Throttle *throttle); 
void terminal_commands_failsafe_Init(Failsafe *failsafe);



#endif /* TERMINAL_COMMANDS_H_ */
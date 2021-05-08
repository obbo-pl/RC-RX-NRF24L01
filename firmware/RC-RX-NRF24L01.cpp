/*
 * RC_RX_NRF24L01.cpp
 *
 * Created: 2020-05-10 00:05:03
 * Atmel Studio 6 (Version: 6.2.1563 - Service Pack 2)
 * AVR/GNU C Compiler : 4.8.1
 *  Author: Krzysztof Markiewicz
 *  www.obbo.pl
 *
 * This program is distributed under the terms of the GNU General Public License
 */ 


#include <avr/io.h>
#include <avr/eeprom.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include "macro.h"
#include "config.h"
#include "tx_def.h"
#include "led.h"
#include "usart.h"
#include "cbuffer.h"
#include "adconversion.h"
#include "servo.h"
#include "spi.h"
#include "nrf24l01_spi.h"
#include "cabell_nrf224l01.h"
#include "uptime.h"
#include "terminal.h"
#include "terminal_commands.h"
#include "lowpass_filter.h"
#include "throttle.h"
#include "failsafe.h"


#define VERSION_MAJOR				"1"
#define VERSION_MINOR				"1"
#define DEVICE_INFO_SIZE			64
const char DEVICE_INFO[DEVICE_INFO_SIZE]	PROGMEM = "RC-RX-NRF24L01 " VERSION_MAJOR "." VERSION_MINOR " (build: " __DATE__ " " __TIME__ ")\n\r";

#define MAIN_STATUS_UPDATE_DELAY		1000		// update every 0.1sec
#define PACKET_LIFETIME				28

// check service mode
#define MAIN_TWI_SDA				C,4
#define MAIN_TWI_SCL				C,5
//#define MAIN_CHECK_SERVO_PORT 

#define MAIN_THROTTLE_PROTECTION
#define MAIN_THROTTLE_CHANNEL			THROTTLE

uint8_t EEMEM EEMEM_battery_divider = 40;

char EEMEM main_message_001[] = "Initializing\n\r";
char EEMEM main_message_002[] = "NRF24L01 detected\n\r";
char EEMEM main_message_003[] = "NRF24L01 not detected\n\r";

uint16_t servo_channels_value[CONFIG_CHANNELS_MAX_COUNT];
CircularBuffer_t cbuffer;
DCTERMINAL_t terminal;
const char *device_info;
uint8_t battery_voltage;
uint8_t battery_divider;
UpTime uptime;
ServoChain servos;
LPFu16_t filter_adc;


void main_ReadBatteryVoltage(uint8_t *voltage);
void main_SaveSetup(void);
void main_ReadSetup(void);
void main_InitServos(ServoChain *servos);
void main_InitRadioTimer(void);
void main_InitServoTimer(void);
void main_SetStatusLed(LED *led, uint8_t *protocol, uint8_t *failsafe);
#ifdef SERVO_CHAIN_STROBE_BY_16BIT_0us5_TIMER
void main_StartServoTimer(uint16_t length);
#else
void main_StartServoTimer(uint8_t length);
#endif

enum MAIN_PHASE {
	PHASE_EXCHANGE_PACKET,
	PHASE_UPDATE_SERVO_CHANNEL_PULSE,
	PHASE_EMIT_TELEMETRY,
	PHASE_CHANNEL_SWITCH,
	PHASE_UPDATE_STATE,
	PHASE_FINISH
};



int main(void)
{
 	LED led_status(PORT_REG_O(CONFIG_STATUS_LED));
#ifdef CONFIG_CH1
	Servo servo_ch1(PORT_REG_IO(CONFIG_CH1));
	servos.bind(&servo_ch1, 1);
#endif
#ifdef CONFIG_CH2
	Servo servo_ch2(PORT_REG_IO(CONFIG_CH2));
	servos.bind(&servo_ch2, 2);
#endif
#ifdef CONFIG_CH3
	Servo servo_ch3(PORT_REG_IO(CONFIG_CH3));
	servos.bind(&servo_ch3, 3);
#endif
#ifdef CONFIG_CH4
	Servo servo_ch4(PORT_REG_IO(CONFIG_CH4));
	servos.bind(&servo_ch4, 4);
#endif
#ifdef CONFIG_CH5
	Servo servo_ch5(PORT_REG_IO(CONFIG_CH5));
	servos.bind(&servo_ch5, 5);
#endif
#ifdef CONFIG_CH6
	Servo servo_ch6(PORT_REG_IO(CONFIG_CH6));
	servos.bind(&servo_ch6, 6);
#endif
#ifdef CONFIG_CH7
	Servo servo_ch7(PORT_REG_IO(CONFIG_CH7));
	servos.bind(&servo_ch7, 7);
#endif
#ifdef CONFIG_CH8
	Servo servo_ch8(PORT_REG_IO(CONFIG_CH8));
	servos.bind(&servo_ch8, 8);
#endif
	bool service_mode = false;
	
	SET_PIN_AS_IN(MAIN_TWI_SDA);
	SET_PIN_PULLUP(MAIN_TWI_SDA);
	_delay_ms(50);
	if (!(READ_PIN(MAIN_TWI_SDA))) service_mode = true;
	SET_PIN_LEVEL_LOW(MAIN_TWI_SDA);
	
	SET_PIN_AS_IN(MAIN_TWI_SCL);
	SET_PIN_PULLUP(MAIN_TWI_SCL);
	_delay_ms(50);
	if (!(READ_PIN(MAIN_TWI_SCL))) service_mode = true;
	SET_PIN_LEVEL_LOW(MAIN_TWI_SDA);

#ifdef MAIN_CHECK_SERVO_PORT
	uint8_t servo_port_check = servos.checkPort();
	if (servo_port_check != 0xFF) service_mode = true;
#endif
	if (service_mode) {
		servos.unbind(2);
		servos.unbind(3);
		servos.unbind(4);
		servos.unbind(7);
		servos.unbind(8);
#ifdef MAIN_CHECK_SERVO_PORT
		for (uint8_t i = 0; i < SERVO_CHAIN_MAX_LENGTH; i++) {
			if (!(testbit(servo_port_check, i))) servos.unbind(i + 1);
		}
#endif
		SET_PIN_AS_IN(CONFIG_BIND_BUTTON);
		SET_PIN_PULLUP(CONFIG_BIND_BUTTON);		
		led_status.init();
		led_status.setLed(true, false);
	}
	servos.attach();
	
	device_info = DEVICE_INFO;
	usart_Init();
	cbuffer_Init(&cbuffer);
	terminal_Init(&terminal, &cbuffer);
		
 	terminal_SendNL(&cbuffer, true);
 	terminal_PrintPM(&cbuffer, device_info, true);
 	terminal_PrintEE(&cbuffer, main_message_001, true);
	main_InitRadioTimer();
	main_InitServoTimer();

	main_ReadSetup();
	lpfilter_Set(&filter_adc, 3);
	lpfilter_Fill(&filter_adc, 0xFF);
	adc_Init(glue2(ADCONVERSION_MUX_ADC,CONFIG_VBAT_MUX_IN));
	
	Throttle throttle(MAIN_THROTTLE_CHANNEL, &uptime);
	throttle.bindTerminal(&cbuffer);
	Failsafe failsafe;
	failsafe.init(&uptime);
	SPI spi;
	NRF24L01 radio(&spi, PORT_REG_O(CONFIG_NRF_CE), PORT_REG_O(CONFIG_NRF_CSN));
	CABELL protocol(&radio, &uptime, service_mode, servo_channels_value);
	protocol.bindTerminal(&cbuffer);
	terminal_commands_receiver_Init(&protocol, servo_channels_value, device_info, &battery_voltage, &battery_divider, &radio);
	terminal_commands_throttle_Init(&throttle);
	terminal_commands_failsafe_Init(&failsafe);
	bool enable_servo_pulses = false;

	protocol.readSetup();
	
	uint8_t bind_button = 1;
	if (service_mode) bind_button = READ_PIN(CONFIG_BIND_BUTTON);
	protocol.checkBindRequest(bind_button);
	failsafe.check(servo_channels_value,
		       &enable_servo_pulses,
		       protocol.cabell.timer.last_good_packet,
		       testbit(protocol.cabell.state.protocol, CABELL_STATE_INITIAL_GOOD_PACKET));
	servos.setValues(servo_channels_value);
	servos.enablePulses(enable_servo_pulses);

	sei();
	servos.interruptHandler(main_StartServoTimer);
	_delay_ms(100);

	if (protocol.init()) {
		terminal_PrintEE(&cbuffer, main_message_002, true);
		protocol.initRadio();
	} else {
		terminal_PrintEE(&cbuffer, main_message_003, true);
	}
	
	uint8_t command_length;
	uint8_t main_phase = 0;

	uint32_t telemetry_emit_delay = 0;
	uint32_t packet_lifetime = 0;
	
 	uint32_t led_update_time = uptime.getTime() + MAIN_STATUS_UPDATE_DELAY;
	uint8_t state_protocol;
	uint8_t state_failsafe;
	uint8_t state_errors;
	protocol.getState(&state_protocol, &state_errors);
	failsafe.getState(&state_failsafe);
	if (service_mode) main_SetStatusLed(&led_status, &state_protocol, &state_failsafe);
	
	while (true) {
		switch (main_phase) {
			case PHASE_EXCHANGE_PACKET:
				packet_lifetime = uptime.getTime();
				if (protocol.exchangePacket(&telemetry_emit_delay)) {
					packet_lifetime += PACKET_LIFETIME;
					if (telemetry_emit_delay) packet_lifetime += CABELL_TELEMETRY_EXTEND_INTERVAL;
					main_phase = PHASE_UPDATE_SERVO_CHANNEL_PULSE;
				} else {
					main_phase = PHASE_FINISH;
				}
				break;
			case PHASE_UPDATE_SERVO_CHANNEL_PULSE:
				failsafe.check(servo_channels_value, 
					       &enable_servo_pulses, 
					       protocol.cabell.timer.last_good_packet, 
					       testbit(protocol.cabell.state.protocol, CABELL_STATE_INITIAL_GOOD_PACKET));
#ifdef MAIN_THROTTLE_PROTECTION
				throttle.check(servo_channels_value, 
					       protocol.cabell.timer.last_good_packet, 
					       testbit(protocol.cabell.state.protocol, CABELL_STATE_INITIAL_GOOD_PACKET),
					       testbit(failsafe.state, FAILSAFE_STATE_ACTIVE));
#endif
				servos.setValues(servo_channels_value);				
				servos.enablePulses(enable_servo_pulses);
				if (service_mode) {
					if (!READ_PIN(CONFIG_BIND_BUTTON)) failsafe.saveValues(servo_channels_value);					
				}
				main_phase++;
				break;
			case PHASE_EMIT_TELEMETRY:
				if (uptime.getTime() >= telemetry_emit_delay) main_phase++;
				break;
			case PHASE_CHANNEL_SWITCH:
				protocol.setNextRadioChannel();
				main_phase++; 
				break;
			case PHASE_UPDATE_STATE:
				main_ReadBatteryVoltage(&battery_voltage);
				protocol.setTelemetryFrame(battery_voltage, 0);
				main_phase++;
				break;
			case PHASE_FINISH:  
				if (protocol.cabell.state.request) {
					if (testbit(protocol.cabell.state.request, CABELL_STATE_REQUEST_FAILSAFE_DEFAULT)) {
						clrbit(protocol.cabell.state.request, CABELL_STATE_REQUEST_FAILSAFE_DEFAULT);
						failsafe.setDefaultValues();
					}
					if (testbit(protocol.cabell.state.request, CABELL_STATE_REQUEST_FAILSAFE_NO_PULSES)) {
						clrbit(protocol.cabell.state.request, CABELL_STATE_REQUEST_FAILSAFE_NO_PULSES);
						failsafe.savePulseMode(false);
					}
					if (testbit(protocol.cabell.state.request, CABELL_STATE_REQUEST_FAILSAFE_ERASE)) {
						clrbit(protocol.cabell.state.request, CABELL_STATE_REQUEST_FAILSAFE_ERASE);
						failsafe.eraseValues();
					}
					if (testbit(protocol.cabell.state.request, CABELL_STATE_REQUEST_FAILSAFE_SAVE)) {
						clrbit(protocol.cabell.state.request, CABELL_STATE_REQUEST_FAILSAFE_SAVE);
						failsafe.saveValues(servo_channels_value);
					}
				} else failsafe.setConfigurationEnabled();
				if (service_mode) {
					protocol.getState(&state_protocol, &state_errors);
					failsafe.getState(&state_failsafe);
					if (led_update_time < uptime.getTime()) {
						main_SetStatusLed(&led_status, &state_protocol, &state_failsafe);
						led_status.update();
						led_update_time = uptime.getTime() + MAIN_STATUS_UPDATE_DELAY;
					}
				}
				terminal_SendOutBuffer(&cbuffer);
				if (terminal.change_to_write) {
					terminal.change_to_write = false;
					main_SaveSetup();
				}
  				if (terminal.input_is_full) terminal_DropInputBuffer(&terminal);
 				if (terminal_FindNewLine(&terminal, &command_length)) terminal_ParseCommand(&terminal, command_length);
				
				if (uptime.getTime() >= packet_lifetime) main_phase = PHASE_EXCHANGE_PACKET;
				break;
		}
	}
}

ISR(USART_RX_vect)
{
	terminal_InterruptHandler(&terminal);
}

ISR(TIMER0_COMPA_vect)
{
	// tic every 100us
	// counter overload occurs after 4,97 days
	uptime.interruptHandler();
}

#ifdef SERVO_CHAIN_STROBE_BY_16BIT_0us5_TIMER
ISR(TIMER1_COMPA_vect)
{
	// stop timer
	TCCR1B &= ~((1 << CS12) | (1 << CS11) | (1 << CS10));
	servos.interruptHandler(main_StartServoTimer);
}

#else
ISR(TIMER2_COMPA_vect)
{
	// stop timer
	TCCR2B &= ~((1 << CS22) | (1 << CS21) | (1 << CS20));
	servos.interruptHandler(main_StartServoTimer);
}
#endif

void main_InitRadioTimer(void)
{
	TCCR0A = (1 << WGM01);
	TCCR0B = (1 << FOC0A) | (1 << CS01);
	OCR0A = 199;
	TIMSK0 = (1 << OCIE0A);
}

#ifdef SERVO_CHAIN_STROBE_BY_16BIT_0us5_TIMER
void main_InitServoTimer(void)
{
	TCCR1A = 0x00;
	TCCR1B = (1 << WGM12);
	TCCR1C = (1 << FOC1A);
	TIMSK1 = (1 << OCIE1A);
}

void main_StartServoTimer(uint16_t length)
{
	OCR1A = length;
	TCCR1B |= (1 << CS11);
}

#else
void main_InitServoTimer(void)
{
	TCCR2A = (1 << WGM21);
	TCCR2B = (1 << FOC2A);
	TIMSK2 = (1 << OCIE2A);
}

void main_StartServoTimer(uint8_t length)
{
	OCR2A = length;
	TCCR2B |= (1 << CS22);
}
#endif

void main_ReadBatteryVoltage(uint8_t *voltage)
{
	uint8_t bat;
	if (adc_Read(&bat)) {
		*voltage = lpfilter_Filter(&filter_adc, bat);
	}
}

void main_SetStatusLed(LED *led, uint8_t *protocol, uint8_t *failsafe)
{
	if (testbit(*failsafe, FAILSAFE_STATE_ACTIVE)) {
		led->setLed(true, false);
	} else if (testbit(*protocol, CABELL_STATE_REBOOT_PENDING)) {
		led->setTimers(20, 10);
		led->setLed(true, true);
	} else if (testbit(*protocol, CABELL_STATE_BIND_MODE)) {
		led->setTimers(20, 2);
		led->setLed(true, true);
	} else if (!(testbit(*protocol, CABELL_STATE_IN_SYNC))) {
		led->setTimers(3, 1);
		led->setLed(true, true);
	} else {
		led->setLed(false, false);
	}
}

void main_SaveSetup(void)
{
	eeprom_write_byte(&EEMEM_battery_divider, battery_divider);
}

void main_ReadSetup(void)
{
	battery_divider = eeprom_read_byte(&EEMEM_battery_divider);
}


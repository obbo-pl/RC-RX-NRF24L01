/*
 * terminal_commands.cpp
 *
 * Created: 2020-06-15 19:33:07
 * Atmel Studio 6 (Version: 6.2.1563 - Service Pack 2)
 * AVR/GNU C Compiler : 4.8.1
 *  Author: Krzysztof Markiewicz
 *  www.obbo.pl
 *
 * This program is distributed under the terms of the GNU General Public License
 */ 

#include <stdlib.h>
#include <avr/eeprom.h>
#include "terminal_commands.h"
#include "macro.h"



void ftoa(float f, char *buffer, uint8_t length);
void command_SystemVersion(DCTERMINAL_t *terminal);
void command_BatteryVoltageValue(DCTERMINAL_t *terminal);
void command_BatteryVoltageDivider(DCTERMINAL_t *terminal);
void command_RadioPowerDetector(DCTERMINAL_t *terminal);
void command_RadioPacketsCount(DCTERMINAL_t *terminal);
void command_RadioErrorsStatus(DCTERMINAL_t *terminal);
void command_RadioErrorsClear(DCTERMINAL_t *terminal);
void command_RadioUnBind(DCTERMINAL_t *terminal);
void command_RadioProtocolStatus(DCTERMINAL_t *terminal);
void command_RadioFailsafeStatus(DCTERMINAL_t *terminal);
void command_RadioSetFailsafe(DCTERMINAL_t *terminal);
void command_RadioServoValue(DCTERMINAL_t *terminal);
void command_RadioFailsafeValue(DCTERMINAL_t *terminal);
void command_ThrottleDisarmValue(DCTERMINAL_t *terminal);
void command_RadioBindModel(DCTERMINAL_t *terminal);
#ifdef TERMINAL_CABELL_FULL_STATISTICS
void command_RadioCountSequential(DCTERMINAL_t *terminal);
void command_RadioCurrentChannel(DCTERMINAL_t *terminal);
void command_RadioPacketTimers(DCTERMINAL_t *terminal);
void command_RadioPacketInterval(DCTERMINAL_t *terminal);
void command_RadioScanInterference(DCTERMINAL_t *terminal);
#endif
#ifdef TERMINAL_NRF24L01_RADIO_STATE
void command_RadioNRF24L01Config(DCTERMINAL_t *terminal);
void command_RadioNRF24L01State(DCTERMINAL_t *terminal);
void command_RadioNRF24L01Feature(DCTERMINAL_t *terminal);
void command_RadioNRF24L01PipeAddress(DCTERMINAL_t *terminal);
void command_RadioNRF24L01PipePayload(DCTERMINAL_t *terminal);
#endif

#define TERMINAL_BASE_COMMANDS_COUNT		15
#ifdef TERMINAL_CABELL_FULL_STATISTICS
#define TERMINAL_STATISTICS_COMMANDS_COUNT	5
#else
#define TERMINAL_STATISTICS_COMMANDS_COUNT	0
#endif
#ifdef TERMINAL_NRF24L01_RADIO_STATE
#define TERMINAL_RADIO_COMMANDS_COUNT		5
#else
#define TERMINAL_RADIO_COMMANDS_COUNT		0
#endif

#define TERMINAL_COMMANDS_COUNT			(TERMINAL_BASE_COMMANDS_COUNT + TERMINAL_STATISTICS_COMMANDS_COUNT + \
						 TERMINAL_RADIO_COMMANDS_COUNT)

TERMINAL_COMMAND_t cabell_commands[TERMINAL_COMMANDS_COUNT] = {
	{ .pattern = "@BVD", .callback = command_BatteryVoltageDivider,},
	{ .pattern = "@BVV", .callback = command_BatteryVoltageValue,},
	{ .pattern = "@RBM", .callback = command_RadioBindModel,},
	{ .pattern = "@REC", .callback = command_RadioErrorsClear,},
	{ .pattern = "@RER", .callback = command_RadioErrorsStatus,},
	{ .pattern = "@RFS", .callback = command_RadioFailsafeStatus,},
	{ .pattern = "@RFV", .callback = command_RadioFailsafeValue,},
	{ .pattern = "@RPC", .callback = command_RadioPacketsCount,},
	{ .pattern = "@RPD", .callback = command_RadioPowerDetector,},
	{ .pattern = "@RSF", .callback = command_RadioSetFailsafe,},
	{ .pattern = "@RSS", .callback = command_RadioProtocolStatus,},
	{ .pattern = "@RSV", .callback = command_RadioServoValue,},
	{ .pattern = "@RUB", .callback = command_RadioUnBind,},
	{ .pattern = "@TDV", .callback = command_ThrottleDisarmValue,},
	{ .pattern = "@VER", .callback = command_SystemVersion,},
#ifdef TERMINAL_CABELL_FULL_STATISTICS
	{ .pattern = "@RCS", .callback = command_RadioCountSequential,},
	{ .pattern = "@RCC", .callback = command_RadioCurrentChannel,},
	{ .pattern = "@RPT", .callback = command_RadioPacketTimers,},
	{ .pattern = "@RPI", .callback = command_RadioPacketInterval,},
	{ .pattern = "@RSI", .callback = command_RadioScanInterference,},
#endif
#ifdef TERMINAL_NRF24L01_RADIO_STATE
	{ .pattern = "@RLC", .callback = command_RadioNRF24L01Config,},
	{ .pattern = "@RLS", .callback = command_RadioNRF24L01State,},
	{ .pattern = "@RLF", .callback = command_RadioNRF24L01Feature,},
	{ .pattern = "@RLA", .callback = command_RadioNRF24L01PipeAddress,},
	{ .pattern = "@RLP", .callback = command_RadioNRF24L01PipePayload,},
#endif
};


const char *receiver_device_info;
uint8_t *receiver_battery_voltage;
uint8_t *receiver_battery_divider;
CABELL *receiver_protocol;
NRF24L01 *receiver_radio;
Throttle *receiver_throttle;
Failsafe *receiver_failsafe;
uint16_t *receiver_servo_channels;


const char EEMEM eeprom_terminal_message_001[] = "Battery voltage [A/D, volt]: ";
const char EEMEM eeprom_terminal_message_002[] = "Battery voltage divider: ";
const char EEMEM eeprom_terminal_message_003[] = "Radio Power Detector (-64 dBm): ";
const char EEMEM eeprom_terminal_message_004[] = "Radio packets:\n\r";
const char EEMEM eeprom_terminal_message_005[] = "  Rx/Tx:        ";
const char EEMEM eeprom_terminal_message_006[] = "  Normal:       ";
const char EEMEM eeprom_terminal_message_007[] = "  Bind:         ";
const char EEMEM eeprom_terminal_message_008[] = "  Set Failsafe: ";
const char EEMEM eeprom_terminal_message_009[] = "  Err/Checksum: ";
const char EEMEM eeprom_terminal_message_010[] = "  Lost/Sync:    ";
const char EEMEM eeprom_terminal_message_011[] = "Protocol errors: ";
const char EEMEM eeprom_terminal_message_012[] = "Servo failsafe value:\n\r";
const char EEMEM eeprom_terminal_message_013[] = "Protocol state:  ";
const char EEMEM eeprom_terminal_message_014[] = "Failsafe state:  ";
const char EEMEM eeprom_terminal_message_015[] = "Throttle disarm value: ";
#ifdef TERMINAL_CABELL_FULL_STATISTICS
const char EEMEM eeprom_terminal_message_016[] = "Sequential:\n\r";
const char EEMEM eeprom_terminal_message_017[] = "  Hit:  ";
const char EEMEM eeprom_terminal_message_018[] = "  Miss: ";
const char EEMEM eeprom_terminal_message_019[] = "Radio timers [0.1ms]:\n\r";
const char EEMEM eeprom_terminal_message_020[] = "  Next channel switch: ";
const char EEMEM eeprom_terminal_message_021[] = "  Receive last packet: ";
const char EEMEM eeprom_terminal_message_022[] = "  Receive good packet: ";
const char EEMEM eeprom_terminal_message_023[] = "  Packet interval:     ";
const char EEMEM eeprom_terminal_message_024[] = "Packet interval [ms]:\n\r";
const char EEMEM eeprom_terminal_message_025[] = "  less:    ";
const char EEMEM eeprom_terminal_message_026[] = "  2.6-2.7: ";
const char EEMEM eeprom_terminal_message_027[] = "  2.8-2.9: ";
const char EEMEM eeprom_terminal_message_028[] = "  3.0-3.1: ";
const char EEMEM eeprom_terminal_message_029[] = "  3.2-3.3: ";
const char EEMEM eeprom_terminal_message_030[] = "  3.4-3.5: ";
const char EEMEM eeprom_terminal_message_031[] = "  3.6-3.7: ";
const char EEMEM eeprom_terminal_message_032[] = "  3.8-3.9: ";
const char EEMEM eeprom_terminal_message_033[] = "  4.0-4.1: ";
#endif
const char EEMEM eeprom_terminal_message_034[] = "Servo value:\n\r";



void terminal_commands_receiver_Init(CABELL *radio_protocol,
				     uint16_t *servo_channels,
				     const char *device_info,
				     uint8_t *battery_voltage,
				     uint8_t *battery_divider,
				     NRF24L01 *radio)
{
	terminal_BindCommands(cabell_commands, TERMINAL_COMMANDS_COUNT);
	receiver_device_info = const_cast<char *>(device_info);
	receiver_battery_voltage = battery_voltage;
	receiver_battery_divider = battery_divider;
	receiver_protocol = radio_protocol;
	receiver_servo_channels = servo_channels;
	receiver_radio = radio;
}

void terminal_commands_throttle_Init(Throttle *throttle)
{
	receiver_throttle = throttle;
}

void terminal_commands_failsafe_Init(Failsafe *failsafe)
{
	receiver_failsafe = failsafe;
}

// in special cases an alternative to sprintf
void ftoa(float f, char *buffer, uint8_t length)
{
	char *start = buffer;
	f += 0.05;
	int major = (int)f;
	itoa(major, start, 10);
	for (uint8_t i = 0; i < length; i++) {
		start++;
		if (*start == 0x00) break;
	}
	*(start++) = '.';
	int minor = (int)((f - major) * 10);
	itoa(minor, start, 10);
}

void command_SystemVersion(DCTERMINAL_t *terminal)
{
	cbuffer_AppendPMString(terminal->output_buffer, receiver_device_info);
}

void command_BatteryVoltageValue(DCTERMINAL_t *terminal)
{
	cbuffer_AppendEEString(terminal->output_buffer, eeprom_terminal_message_001);
	char buffer[8];
	itoa(*receiver_battery_voltage, buffer, 10);
	cbuffer_AppendString(terminal->output_buffer, buffer);
	cbuffer_AppendString(terminal->output_buffer, ", ");
	float volt_f = *receiver_battery_voltage;
	volt_f /= *receiver_battery_divider;
	ftoa(volt_f, buffer, 8);
	cbuffer_AppendString(terminal->output_buffer, buffer);
	cbuffer_AppendString(terminal->output_buffer, "V");
	terminal_SendNL(terminal->output_buffer);
}

void command_BatteryVoltageDivider(DCTERMINAL_t *terminal)
{
	if (terminal->command_option[0] == TERMINAL_SPACE) {
		int temp = atoi(terminal->command_option);
		if ((temp > 0) && (temp <= 100)) {
			*receiver_battery_divider = temp;
			terminal->change_to_write = true;
			} else {
			terminal_SendBadArgument(terminal->output_buffer);
		}
	}
	cbuffer_AppendEEString(terminal->output_buffer, eeprom_terminal_message_002);
	char buffer [4];
	itoa(*receiver_battery_divider, buffer, 10);
	cbuffer_AppendString(terminal->output_buffer, buffer);
	terminal_SendNL(terminal->output_buffer);
}

void command_RadioPowerDetector(DCTERMINAL_t *terminal)
{
	cbuffer_AppendEEString(terminal->output_buffer, eeprom_terminal_message_003);
	char buffer[8];
	ultoa(receiver_protocol->cabell.signal_power.good, buffer, 10);
	cbuffer_AppendString(terminal->output_buffer, buffer);
	cbuffer_AppendString(terminal->output_buffer, "/");
	ultoa(receiver_protocol->cabell.signal_power.probe, buffer, 10);
	cbuffer_AppendString(terminal->output_buffer, buffer);
	terminal_SendNL(terminal->output_buffer);
}

void command_RadioPacketsCount(DCTERMINAL_t *terminal)
{
	cbuffer_AppendEEString(terminal->output_buffer, eeprom_terminal_message_004);
	char buffer[12];
	cbuffer_AppendEEString(terminal->output_buffer, eeprom_terminal_message_005);
	ultoa(receiver_protocol->cabell.counter.rx, buffer, 10);
	cbuffer_AppendString(terminal->output_buffer, buffer);
	cbuffer_AppendString(terminal->output_buffer, "/");
	ultoa(receiver_protocol->cabell.counter.tx, buffer, 10);
	cbuffer_AppendString(terminal->output_buffer, buffer);
	terminal_SendNL(terminal->output_buffer);
	cbuffer_AppendEEString(terminal->output_buffer, eeprom_terminal_message_006);
	ultoa(receiver_protocol->cabell.counter.normal, buffer, 10);
	cbuffer_AppendString(terminal->output_buffer, buffer);
	terminal_SendNL(terminal->output_buffer);
	cbuffer_AppendEEString(terminal->output_buffer, eeprom_terminal_message_007);
	ultoa(receiver_protocol->cabell.counter.bind, buffer, 10);
	cbuffer_AppendString(terminal->output_buffer, buffer);
	terminal_SendNL(terminal->output_buffer);
	cbuffer_AppendEEString(terminal->output_buffer, eeprom_terminal_message_008);
	ultoa(receiver_protocol->cabell.counter.set_failsafe, buffer, 10);
	cbuffer_AppendString(terminal->output_buffer, buffer);
	terminal_SendNL(terminal->output_buffer);
	cbuffer_AppendEEString(terminal->output_buffer, eeprom_terminal_message_009);
	ultoa(receiver_protocol->cabell.counter.error_total, buffer, 10);
	cbuffer_AppendString(terminal->output_buffer, buffer);
	cbuffer_AppendString(terminal->output_buffer, "/");
	ultoa(receiver_protocol->cabell.counter.error_checksum, buffer, 10);
	cbuffer_AppendString(terminal->output_buffer, buffer);
	terminal_SendNL(terminal->output_buffer);
	cbuffer_AppendEEString(terminal->output_buffer, eeprom_terminal_message_010);
	ultoa(receiver_protocol->cabell.counter.lost, buffer, 10);
	cbuffer_AppendString(terminal->output_buffer, buffer);
	cbuffer_AppendString(terminal->output_buffer, "/");
	ultoa(receiver_protocol->cabell.counter.resync, buffer, 10);
	cbuffer_AppendString(terminal->output_buffer, buffer);
	terminal_SendNL(terminal->output_buffer);
}

void command_RadioErrorsStatus(DCTERMINAL_t *terminal)
{
	cbuffer_AppendEEString(terminal->output_buffer, eeprom_terminal_message_011);
	terminal_PrintBin(terminal->output_buffer, receiver_protocol->cabell.state.errors);
	terminal_SendNL(terminal->output_buffer);
}

void command_RadioErrorsClear(DCTERMINAL_t *terminal)
{
	receiver_protocol->cabell.state.errors = 0x00;
	terminal_SendOK(terminal->output_buffer);
}

void command_RadioBindModel(DCTERMINAL_t *terminal)
{
	if (terminal->command_option[0] == TERMINAL_SPACE) {
		int temp = atoi(terminal->command_option);
		if ((temp >= 0) && (temp < 255)) {
			receiver_protocol->saveModel(temp);
		} else {
			terminal_SendBadArgument(terminal->output_buffer);
		}
	}
	cbuffer_AppendString(terminal->output_buffer, "Saved model: ");
	char buffer [4];
	itoa(receiver_protocol->cabell.current_model, buffer, 10);
	cbuffer_AppendString(terminal->output_buffer, buffer);
	terminal_SendNL(terminal->output_buffer);
}

void command_RadioUnBind(DCTERMINAL_t *terminal)
{
	receiver_protocol->unbindReciever();
}

void command_RadioProtocolStatus(DCTERMINAL_t *terminal)
{
	cbuffer_AppendEEString(terminal->output_buffer, eeprom_terminal_message_013);
	terminal_PrintBin(terminal->output_buffer, receiver_protocol->cabell.state.protocol);
	terminal_SendNL(terminal->output_buffer);
}

void command_RadioFailsafeStatus(DCTERMINAL_t *terminal)
{
	cbuffer_AppendEEString(terminal->output_buffer, eeprom_terminal_message_014);
	terminal_PrintBin(terminal->output_buffer, receiver_failsafe->state);
	terminal_SendNL(terminal->output_buffer);
}

void command_RadioSetFailsafe(DCTERMINAL_t *terminal)
{
	receiver_failsafe->setConfigurationEnabled();
	receiver_failsafe->seveValues(receiver_servo_channels);
	terminal_SendOK(terminal->output_buffer);
}

void command_RadioServoValue(DCTERMINAL_t *terminal)
{
	cbuffer_AppendEEString(terminal->output_buffer, eeprom_terminal_message_034);
	for (uint8_t i = 0; i < (CABELL_MAX_CHANNELS / 2); i++) {
		cbuffer_AppendString(terminal->output_buffer, "  CH");
		terminal_PrintInt(terminal->output_buffer, i + 1);
		cbuffer_AppendString(terminal->output_buffer, ": ");
		terminal_PrintInt(terminal->output_buffer, receiver_servo_channels[i]);
		terminal_SendNL(terminal->output_buffer);
	}
}

void command_RadioFailsafeValue(DCTERMINAL_t *terminal)
{
	cbuffer_AppendEEString(terminal->output_buffer, eeprom_terminal_message_012);
	for (uint8_t i = 0; i < (CABELL_MAX_CHANNELS / 2); i++) {
		cbuffer_AppendString(terminal->output_buffer, "  CH");
		terminal_PrintInt(terminal->output_buffer, i + 1);
		cbuffer_AppendString(terminal->output_buffer, ": ");
		terminal_PrintInt(terminal->output_buffer, receiver_failsafe->servo_channels[i]);
		terminal_SendNL(terminal->output_buffer);
	}
}

void command_ThrottleDisarmValue(DCTERMINAL_t *terminal)
{
	if (terminal->command_option[0] == TERMINAL_SPACE) {
		int temp = atoi(terminal->command_option);
		if ((temp >= THROTTLE_MIN_VALUE) && (temp <= THROTTLE_MAX_VALUE)) {
			receiver_throttle->saveDisarmValue(temp);
		} else {
			terminal_SendBadArgument(terminal->output_buffer);
		}
	}
	cbuffer_AppendEEString(terminal->output_buffer, eeprom_terminal_message_015);
	char buffer [6];
	itoa(receiver_throttle->getDisarmValue(), buffer, 10);
	cbuffer_AppendString(terminal->output_buffer, buffer);
	terminal_SendNL(terminal->output_buffer);
}

#ifdef TERMINAL_CABELL_FULL_STATISTICS
void command_RadioCountSequential(DCTERMINAL_t *terminal)
{
	cbuffer_AppendEEString(terminal->output_buffer, eeprom_terminal_message_016);
	char buffer[12];
	cbuffer_AppendEEString(terminal->output_buffer, eeprom_terminal_message_017);
	ultoa(receiver_protocol->cabell.counter.sequential_hit, buffer, 10);
	cbuffer_AppendString(terminal->output_buffer, buffer);
	terminal_SendNL(terminal->output_buffer);
	cbuffer_AppendEEString(terminal->output_buffer, eeprom_terminal_message_018);
	ultoa(receiver_protocol->cabell.counter.sequential_miss, buffer, 10);
	cbuffer_AppendString(terminal->output_buffer, buffer);
	terminal_SendNL(terminal->output_buffer);
}

void command_RadioCurrentChannel(DCTERMINAL_t *terminal)
{
	cbuffer_AppendString(terminal->output_buffer, "Radio channel: ");
	char buffer[6];
	ultoa(receiver_protocol->cabell.current_radio_channel, buffer, 10);
	cbuffer_AppendString(terminal->output_buffer, buffer);
	terminal_SendNL(terminal->output_buffer);
}

void command_RadioPacketTimers(DCTERMINAL_t *terminal)
{
	cbuffer_AppendEEString(terminal->output_buffer, eeprom_terminal_message_019);
	char buffer[12];
	cbuffer_AppendEEString(terminal->output_buffer, eeprom_terminal_message_020);
	ultoa(receiver_protocol->cabell.timer.next_channel_switch, buffer, 10);
	cbuffer_AppendString(terminal->output_buffer, buffer);
	terminal_SendNL(terminal->output_buffer);
	cbuffer_AppendEEString(terminal->output_buffer, eeprom_terminal_message_021);
	ultoa(receiver_protocol->cabell.timer.last_packet_received, buffer, 10);
	cbuffer_AppendString(terminal->output_buffer, buffer);
	terminal_SendNL(terminal->output_buffer);
	cbuffer_AppendEEString(terminal->output_buffer, eeprom_terminal_message_022);
	ultoa(receiver_protocol->cabell.timer.last_good_packet, buffer, 10);
	cbuffer_AppendString(terminal->output_buffer, buffer);
	terminal_SendNL(terminal->output_buffer);
	cbuffer_AppendEEString(terminal->output_buffer, eeprom_terminal_message_023);
	ultoa(receiver_protocol->cabell.timer.packet_interval, buffer, 10);
	cbuffer_AppendString(terminal->output_buffer, buffer);
	terminal_SendNL(terminal->output_buffer);
	cbuffer_AppendString(terminal->output_buffer, "  Sync recovery:       ");
	ultoa(receiver_protocol->cabell.timer.sync_recovery, buffer, 10);
	cbuffer_AppendString(terminal->output_buffer, buffer);
	terminal_SendNL(terminal->output_buffer);
}

void command_RadioPacketInterval(DCTERMINAL_t *terminal)
{
	cbuffer_AppendEEString(terminal->output_buffer, eeprom_terminal_message_024);
	char buffer[10];
	cbuffer_AppendEEString(terminal->output_buffer, eeprom_terminal_message_025);
	ultoa(receiver_protocol->cabell.len_less, buffer, 10);
	cbuffer_AppendString(terminal->output_buffer, buffer);
	terminal_SendNL(terminal->output_buffer);
	cbuffer_AppendEEString(terminal->output_buffer, eeprom_terminal_message_026);
	ultoa(receiver_protocol->cabell.len_2700, buffer, 10);
	cbuffer_AppendString(terminal->output_buffer, buffer);
	terminal_SendNL(terminal->output_buffer);
	cbuffer_AppendEEString(terminal->output_buffer, eeprom_terminal_message_027);
	ultoa(receiver_protocol->cabell.len_2900, buffer, 10);
	cbuffer_AppendString(terminal->output_buffer, buffer);
	terminal_SendNL(terminal->output_buffer);
	cbuffer_AppendEEString(terminal->output_buffer, eeprom_terminal_message_028);
	ultoa(receiver_protocol->cabell.len_3100, buffer, 10);
	cbuffer_AppendString(terminal->output_buffer, buffer);
	terminal_SendNL(terminal->output_buffer);
	cbuffer_AppendEEString(terminal->output_buffer, eeprom_terminal_message_029);
	ultoa(receiver_protocol->cabell.len_3300, buffer, 10);
	cbuffer_AppendString(terminal->output_buffer, buffer);
	terminal_SendNL(terminal->output_buffer);
	cbuffer_AppendEEString(terminal->output_buffer, eeprom_terminal_message_030);
	ultoa(receiver_protocol->cabell.len_3500, buffer, 10);
	cbuffer_AppendString(terminal->output_buffer, buffer);
	terminal_SendNL(terminal->output_buffer);
	cbuffer_AppendEEString(terminal->output_buffer, eeprom_terminal_message_031);
	ultoa(receiver_protocol->cabell.len_3700, buffer, 10);
	cbuffer_AppendString(terminal->output_buffer, buffer);
	terminal_SendNL(terminal->output_buffer);
	cbuffer_AppendEEString(terminal->output_buffer, eeprom_terminal_message_032);
	ultoa(receiver_protocol->cabell.len_3900, buffer, 10);
	cbuffer_AppendString(terminal->output_buffer, buffer);
	terminal_SendNL(terminal->output_buffer);
	cbuffer_AppendEEString(terminal->output_buffer, eeprom_terminal_message_033);
	ultoa(receiver_protocol->cabell.len_4100, buffer, 10);
	cbuffer_AppendString(terminal->output_buffer, buffer);
	terminal_SendNL(terminal->output_buffer);
	cbuffer_AppendString(terminal->output_buffer, "  more:    ");
	ultoa(receiver_protocol->cabell.len_more, buffer, 10);
	cbuffer_AppendString(terminal->output_buffer, buffer);
	terminal_SendNL(terminal->output_buffer);
}

void command_RadioScanInterference(DCTERMINAL_t *terminal)
{
	cbuffer_AppendString(terminal->output_buffer, "Interference scan (");
	terminal_PrintInt(terminal->output_buffer, CABELL_RADIO_MIN_CHANNEL_NUM);
	cbuffer_AppendString(terminal->output_buffer, "-");
	terminal_PrintInt(terminal->output_buffer, CABELL_RADIO_CHANNELS * CABELL_RADIO_CHANNELS_BANDS + CABELL_RADIO_MIN_CHANNEL_NUM - 1);
	cbuffer_AppendString(terminal->output_buffer, "):\n\r");
	uint16_t max = 4;
	uint32_t sum = 0;
	for (uint8_t i = CABELL_RADIO_MIN_CHANNEL_NUM; i < CABELL_RADIO_CHANNELS * CABELL_RADIO_CHANNELS_BANDS + CABELL_RADIO_MIN_CHANNEL_NUM; i++) {
		if (receiver_protocol->cabell.radio_scan[i] > max) max = receiver_protocol->cabell.radio_scan[i];
		sum += receiver_protocol->cabell.radio_scan[i];
	}
	uint32_t level = 2 * sum / (CABELL_RADIO_CHANNELS * CABELL_RADIO_CHANNELS_BANDS);
	if (level < max) level = max;
	level = level / 4;
	for (uint8_t i = CABELL_RADIO_MIN_CHANNEL_NUM; i < CABELL_RADIO_CHANNELS * CABELL_RADIO_CHANNELS_BANDS + CABELL_RADIO_MIN_CHANNEL_NUM; i++) {
		if (receiver_protocol->cabell.radio_scan[i] >= 3 * level) cbuffer_AppendChar(terminal->output_buffer, 0xDB);
		else if (receiver_protocol->cabell.radio_scan[i] >= 2 * level) cbuffer_AppendChar(terminal->output_buffer, 0xB2);
		else if (receiver_protocol->cabell.radio_scan[i] >= level) cbuffer_AppendChar(terminal->output_buffer, 0xB0);
		else cbuffer_AppendChar(terminal->output_buffer, 0x5F);
	}
	terminal_SendNL(terminal->output_buffer);
}
#endif

#ifdef TERMINAL_NRF24L01_RADIO_STATE
void command_RadioNRF24L01Config(DCTERMINAL_t *terminal)
{
	cbuffer_AppendString(terminal->output_buffer, "NRF24L01 Config:\n\r");
	// 0x00
	cbuffer_AppendString(terminal->output_buffer, "  CONFIG:      ");
	terminal_PrintHexAndBin(terminal->output_buffer, receiver_radio->readRegister(0x00));
	// 0x01
	cbuffer_AppendString(terminal->output_buffer, "  EN_AA:       ");
	terminal_PrintHexAndBin(terminal->output_buffer, receiver_radio->readRegister(0x01));
	// 0x02
	cbuffer_AppendString(terminal->output_buffer, "  EN_RXADDR:   ");
	terminal_PrintHexAndBin(terminal->output_buffer, receiver_radio->readRegister(0x02));
	// 0x03
	cbuffer_AppendString(terminal->output_buffer, "  SETUP_AW:    ");
	terminal_PrintHexAndBin(terminal->output_buffer, receiver_radio->readRegister(0x03));
	// 0x04
	cbuffer_AppendString(terminal->output_buffer, "  SETUP_RETR:  ");
	terminal_PrintHexAndBin(terminal->output_buffer, receiver_radio->readRegister(0x04));
	// 0x05
	cbuffer_AppendString(terminal->output_buffer, "  RF_CH:       ");
	terminal_PrintInt(terminal->output_buffer, receiver_radio->readRegister(0x05));
	terminal_SendNL(terminal->output_buffer);
	// 0x06
	cbuffer_AppendString(terminal->output_buffer, "  RF_SETUP:    ");
	terminal_PrintHexAndBin(terminal->output_buffer, receiver_radio->readRegister(0x06));
}

void command_RadioNRF24L01State(DCTERMINAL_t *terminal) 
{
	cbuffer_AppendString(terminal->output_buffer, "NRF24L01 State:\n\r");
	// 0x07
	cbuffer_AppendString(terminal->output_buffer, "  STATUS:      ");
	terminal_PrintHexAndBin(terminal->output_buffer, receiver_radio->readRegister(0x07));
	// 0x08
	cbuffer_AppendString(terminal->output_buffer, "  OBSERVE_TX:  ");
	terminal_PrintHexAndBin(terminal->output_buffer, receiver_radio->readRegister(0x08));
	// 0x09
	cbuffer_AppendString(terminal->output_buffer, "  RPD:         ");
	terminal_PrintHexAndBin(terminal->output_buffer, receiver_radio->readRegister(0x09));
	// 0x17
	cbuffer_AppendString(terminal->output_buffer, "  FIFO_STATUS: ");
	terminal_PrintHexAndBin(terminal->output_buffer, receiver_radio->readRegister(0x17));
}

void command_RadioNRF24L01Feature(DCTERMINAL_t *terminal)
{
	cbuffer_AppendString(terminal->output_buffer, "NRF24L01 Feature:\n\r");
	// 0x1C
	cbuffer_AppendString(terminal->output_buffer, "  DYNPD:       ");
	terminal_PrintHexAndBin(terminal->output_buffer, receiver_radio->readRegister(0x1C));
	// 0x1D
	cbuffer_AppendString(terminal->output_buffer, "  FEATURE:     ");
	terminal_PrintHexAndBin(terminal->output_buffer, receiver_radio->readRegister(0x1D));
}

void command_RadioNRF24L01PipeAddress(DCTERMINAL_t *terminal)
{
	uint8_t width = receiver_radio->getAddressWidth();
	cbuffer_AppendString(terminal->output_buffer, "NRF24L01 Pipe address:\n\r");
	cbuffer_AppendString(terminal->output_buffer, "  Length:      ");
	terminal_PrintInt(terminal->output_buffer, width);
	terminal_SendNL(terminal->output_buffer);
	// 0x0A
	uint64_t address;
	cbuffer_AppendString(terminal->output_buffer, "  RX_ADDR_P0:  ");
	receiver_radio->readRegister(0x0A, reinterpret_cast<uint8_t *>(&address), width);
	cbuffer_AppendString(terminal->output_buffer, "0x");
	terminal_PrintBuf(terminal->output_buffer, reinterpret_cast<const uint8_t *>(&address), width);
	terminal_SendNL(terminal->output_buffer);
	// 0x0B
	cbuffer_AppendString(terminal->output_buffer, "  RX_ADDR_P1:  ");
	receiver_radio->readRegister(0x0B, reinterpret_cast<uint8_t *>(&address), width);
	cbuffer_AppendString(terminal->output_buffer, "0x");
	terminal_PrintBuf(terminal->output_buffer, reinterpret_cast<const uint8_t *>(&address), width);
	terminal_SendNL(terminal->output_buffer);
	// 0x0C
	cbuffer_AppendString(terminal->output_buffer, "  RX_ADDR_P2:  ");
	cbuffer_AppendString(terminal->output_buffer, "0x");
	terminal_PrintHex(terminal->output_buffer, receiver_radio->readRegister(0x0C));
	terminal_SendNL(terminal->output_buffer);
	// 0x0D
	cbuffer_AppendString(terminal->output_buffer, "  RX_ADDR_P3:  ");
	cbuffer_AppendString(terminal->output_buffer, "0x");
	terminal_PrintHex(terminal->output_buffer, receiver_radio->readRegister(0x0D));
	terminal_SendNL(terminal->output_buffer);
	// 0x0E
	cbuffer_AppendString(terminal->output_buffer, "  RX_ADDR_P4:  ");
	cbuffer_AppendString(terminal->output_buffer, "0x");
	terminal_PrintHex(terminal->output_buffer, receiver_radio->readRegister(0x0E));
	terminal_SendNL(terminal->output_buffer);
	// 0x0F
	cbuffer_AppendString(terminal->output_buffer, "  RX_ADDR_P5:  ");
	cbuffer_AppendString(terminal->output_buffer, "0x");
	terminal_PrintHex(terminal->output_buffer, receiver_radio->readRegister(0x0F));
	terminal_SendNL(terminal->output_buffer);
	// 0x10
	cbuffer_AppendString(terminal->output_buffer, "  TX_ADDR:     ");
	receiver_radio->readRegister(0x10, reinterpret_cast<uint8_t *>(&address), width);
	cbuffer_AppendString(terminal->output_buffer, "0x");
	terminal_PrintBuf(terminal->output_buffer, reinterpret_cast<const uint8_t *>(&address), width);
	terminal_SendNL(terminal->output_buffer);
}

void command_RadioNRF24L01PipePayload(DCTERMINAL_t *terminal)
{
	cbuffer_AppendString(terminal->output_buffer, "NRF24L01 Pipe payload:\n\r");
	// 0x11
	cbuffer_AppendString(terminal->output_buffer, "  RX_PW_P0:    ");
	terminal_PrintInt(terminal->output_buffer, receiver_radio->readRegister(0x11));
	terminal_SendNL(terminal->output_buffer);
	// 0x12
	cbuffer_AppendString(terminal->output_buffer, "  RX_PW_P1:    ");
	terminal_PrintInt(terminal->output_buffer, receiver_radio->readRegister(0x12));
	terminal_SendNL(terminal->output_buffer);
	// 0x13
	cbuffer_AppendString(terminal->output_buffer, "  RX_PW_P2:    ");
	terminal_PrintInt(terminal->output_buffer, receiver_radio->readRegister(0x13));
	terminal_SendNL(terminal->output_buffer);
	// 0x14
	cbuffer_AppendString(terminal->output_buffer, "  RX_PW_P3:    ");
	terminal_PrintInt(terminal->output_buffer, receiver_radio->readRegister(0x14));
	terminal_SendNL(terminal->output_buffer);
	// 0x15
	cbuffer_AppendString(terminal->output_buffer, "  RX_PW_P4:    ");
	terminal_PrintInt(terminal->output_buffer, receiver_radio->readRegister(0x15));
	terminal_SendNL(terminal->output_buffer);
	// 0x16
	cbuffer_AppendString(terminal->output_buffer, "  RX_PW_P5:    ");
	terminal_PrintInt(terminal->output_buffer, receiver_radio->readRegister(0x16));
	terminal_SendNL(terminal->output_buffer);
}
#endif

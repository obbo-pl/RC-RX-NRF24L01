/*
 * CABELL_nrf224l01.h
 *
 * Created: 2020-05-10 23:48:11
 * Atmel Studio 6 (Version: 6.2.1563 - Service Pack 2)
 * AVR/GNU C Compiler : 4.8.1
 *  Author: Krzysztof Markiewicz
 *  www.obbo.pl
 *
 * This program is distributed under the terms of the GNU General Public License
 */ 


#include <util/delay.h>
#include <stdlib.h>
#include "cabell_nrf224l01.h"
#include "macro.h"
#include "tx_def.h"
#include "Rx_Tx_Util.h"
#include <avr/eeprom.h>


uint8_t EEMEM eeprom_soft_bind_flag;
uint8_t EEMEM eeprom_current_model;
uint64_t EEMEM eeprom_radio_pipe_address;

char EEMEM cabell_message_000[] = "Entering Bind Mode\n\r";
char EEMEM cabell_message_001[] = "Bound to new TX address for model: ";
char EEMEM cabell_message_002[] = "New Rx pipe address: ";
char EEMEM cabell_message_003[] = "Receiver bound. Reboot to enter normal mode\n\r";
char EEMEM cabell_message_004[] = "Receiver un-bound. Reboot to enter bind mode\n\r";
char EEMEM cabell_message_005[] = "Re-sync Attempt\n\r";
char EEMEM cabell_message_006[] = "Initial Signal Locked\n\r";
char EEMEM cabell_message_007[] = "Signal Locked\n\r";
char EEMEM cabell_message_008[] = "Rx pipe address: ";



CABELL::CABELL(NRF24L01 *radio, UpTime *uptime, bool service_mode, uint16_t *servo_channels)
{
	this->servo_channels_value = servo_channels;
	this->radio = radio;	
	this->uptime = uptime;
	this->console = NULL;
	this->cabell.current_radio_channel = CABELL_RADIO_MIN_CHANNEL_NUM;
	this->cabell.counter.sequential_hit = 0;
	this->cabell.counter.sequential_miss = 0;
	this->initial_telemetry_skip_packets = 0;
	this->cabell.power_mode = 0;
	this->clearTimer();
	this->clearState();
	this->clearCouters();
	this->clearSignalPowerCounters();
#ifdef TERMINAL_CABELL_FULL_STATISTICS
	this->clearRadioScan();
#endif
	if (service_mode) setbit(cabell.state.protocol, CABELL_STATE_SERVICE_MODE);
}

void CABELL::clearCouters(void)
{
	this->cabell.counter.rx = 0;
	this->cabell.counter.tx = 0;
	this->cabell.counter.normal = 0;
	this->cabell.counter.bind = 0;
	this->cabell.counter.set_failsafe = 0;
	this->cabell.counter.error_checksum = 0;
	this->cabell.counter.error_total = 0;
	this->cabell.counter.resync = 0;
	this->cabell.counter.lost = 0;
	this->cabell.counter.sequential_hit = 0;
	this->cabell.counter.sequential_miss = 0;
}

void CABELL::clearTimer(void)
{
	this->cabell.timer.next_channel_switch = this->uptime->getTime() + (uint32_t)CABELL_RESYNC_WAIT;
	this->cabell.timer.packet_interval = CABELL_DEFAULT_PACKET_INTERVAL;
	this->cabell.timer.last_good_packet = 0;
	this->cabell.timer.last_packet_received = 0;
	this->cabell.timer.sync_recovery = 0;	
	this->cabell.timer.lost_sync = 0;
}

void CABELL::clearSignalPowerCounters(void)
{
	this->cabell.signal_power.good = 0;
	this->cabell.signal_power.probe = 0;
}

void CABELL::clearState(void)
{
	this->cabell.state.protocol = 0x00;
	this->cabell.state.errors = 0x00;
	this->cabell.state.request = 0x00;
}

#ifdef TERMINAL_CABELL_FULL_STATISTICS
void CABELL::clearRadioScan(void)
{
	for (uint8_t i = 0; i < (CABELL_RADIO_CHANNELS * CABELL_RADIO_CHANNELS_BANDS + CABELL_RADIO_MIN_CHANNEL_NUM); i++) {
		this->cabell.radio_scan[i] = 0;	
	}
	this->cabell.len_less = 0;
	this->cabell.len_2700 = 0;
	this->cabell.len_2900 = 0;
	this->cabell.len_3100 = 0;
	this->cabell.len_3300 = 0;
	this->cabell.len_3500 = 0;
	this->cabell.len_3700 = 0;
	this->cabell.len_3900 = 0;
	this->cabell.len_4100 = 0;
	this->cabell.len_more = 0;
}
#endif

bool CABELL::init(void)
{
	this->radio->powerUp();
	_delay_ms(5);
	return this->radio->isChipConnected();
}

void CABELL::initRadio(void) {
	setTelemetryPowerMode(CABELL_MASK_MAX_POWER_OVERRIDE);
	this->radio->setChipEnable(true);
	this->radio->setCRC(2);
	this->radio->setAddressWidth(5);
	this->radio->setBitrate(NRF24L01_BITRATE_250k);
	this->radio->activateFeatures();
	this->radio->setAutoAck(false);
	this->radio->enableDynamicPayloads(true);
	this->radio->setRetries(NRF24L01_RETRANSMIT_DELAY_250us, 0);

 	this->radio->setPayloadLength(0, 4);
 	this->radio->setPayloadLength(1, 30);
	this->radio->openWritingPipe(~(this->cabell.radio_pipe_id), false);
	this->radio->openReadingPipe(1, this->cabell.radio_pipe_id);

	this->radio->setTxMode();
	this->radio->maskInterrupt(true, true, true);
 	template_radio_config_tx = this->radio->getConfig();

	this->radio->setRxMode();
	this->radio->maskInterrupt(false, true, true);
	template_radio_config_rx = this->radio->getConfig();

	this->radio->flushTx();
	this->radio->flushRx();
	this->radio->clearInterrupt();
	initResync();
}

void CABELL::bindTerminal(CircularBuffer_t *console)
{
	this->console = console;
}

void CABELL::getState(uint8_t *protocol, uint8_t *errors)
{
	*protocol = this->cabell.state.protocol;
	*errors = this->cabell.state.errors;
}

void CABELL::setTelemetryPowerMode(uint8_t option) 
{
	// Set transmit power to max or high based on the option byte in the incoming packet.
	// This should set the power the same as the transmitter module
	NRF24L01_TX_POWER power_mode;
	if ((option & CABELL_MASK_MAX_POWER_OVERRIDE) == 0) {
		power_mode = NRF24L01_TX_POWER_HIGH;
	} else {
		power_mode = NRF24L01_TX_POWER_MAX;
	}
	if (power_mode != this->cabell.power_mode) {
		radio->setTxPower(power_mode);
		this->cabell.power_mode = power_mode;
	}
}

void CABELL::setTelemetryFrame(uint8_t lipo1, uint8_t lipo2)
{
	telemetry_packet[2] = lipo1;
	telemetry_packet[3] = lipo2;
}

uint32_t CABELL::sendTelemetry(void)
{
	uint32_t expected_emit_time = 0;
	if (testbit(this->cabell.state.protocol, CABELL_STATE_TELEMETRY_ENABLED)) {
		if (initial_telemetry_skip_packets >= CABELL_TELEMETRY_PACKETS_TO_SKIP) {
			this->radio->setChipEnable(false);
			this->radio->setConfig(template_radio_config_tx);
			this->radio->flushTx();
			telemetry_packet[0] = CABELL_RXMODE_telemetryResponse | ((this->cabell.counter.tx << 7) & 0x80);
			telemetry_packet[1] = rssi.getRSSI();
			this->radio->writePayload(telemetry_packet, CABELL_TELEMETRY_PACKET_LENGTH);
			expected_emit_time = this->uptime->getTime() + CABELL_TELEMETRY_EMIT_INTERVAL;
			this->cabell.counter.tx++;
		} else {
			initial_telemetry_skip_packets++;
		}
	}
	this->radio->setChipEnable(true);
	return expected_emit_time;
}

void CABELL::setNextRadioChannel(void) { 
	this->radio->setConfig(template_radio_config_rx);
	this->cabell.current_radio_channel = getNextChannel(radio_channels, CABELL_RADIO_CHANNELS, this->cabell.current_radio_channel);
	this->radio->setChannel(this->cabell.current_radio_channel);
	this->radio->flushRx();
	this->radio->clearInterrupt();
}

// receive control packet and send telemetry
bool CABELL::exchangePacket(uint32_t *telemetry_emit_time)
{
	bool channel_switch = false;
	bool good_packet = false;
	bool strong_signal = false;
	
	if (this->radio->isRxPacektAvailable()) {	
#ifdef TERMINAL_CABELL_FULL_STATISTICS
		uint32_t delay = this->uptime->getTime() - this->cabell.timer.last_packet_received;
		if (delay < 26) this->cabell.len_less++;
		else if (delay < 28) this->cabell.len_2700++;
		else if (delay < 30) this->cabell.len_2900++;
		else if (delay < 32) this->cabell.len_3100++;
		else if (delay < 34) this->cabell.len_3300++;
		else if (delay < 36) this->cabell.len_3500++;
		else if (delay < 38) this->cabell.len_3700++;
		else if (delay < 40) this->cabell.len_3900++;
		else if (delay < 42) this->cabell.len_4100++;
		else if (delay < 50) this->cabell.len_more++;
#endif
		this->cabell.timer.last_packet_received = this->uptime->getTime();
		strong_signal = (this->radio->readPowerDetector());
		if (strong_signal) this->cabell.signal_power.good++;
		good_packet = processPackets(telemetry_emit_time);
		this->cabell.signal_power.probe++;
		if (!(testbit(this->cabell.state.protocol, CABELL_STATE_POWER_ON_LOCK))) {
			if(!(strong_signal)) good_packet = false;
		}	
		this->cabell.timer.next_channel_switch = this->cabell.timer.last_packet_received + this->cabell.timer.packet_interval + CABELL_PACKET_TIMEOUT_OFFSET;
		if (good_packet) {
			updateSequentialHit();
			setbit(this->cabell.state.protocol, CABELL_STATE_INITIAL_GOOD_PACKET);
			this->cabell.timer.last_good_packet = this->uptime->getTime();
			rssi.hit();
		} else {
			updateSequentialMiss();
			rssi.badPacket();
		}
		channel_switch = true;
	} else if (this->uptime->getTime() > this->cabell.timer.next_channel_switch) {
		updateSequentialMiss();
		rssi.miss();
		this->cabell.counter.lost++;
		this->cabell.timer.next_channel_switch += this->cabell.timer.packet_interval;
		*telemetry_emit_time = sendTelemetry();
		channel_switch = true;
#ifdef TERMINAL_CABELL_FULL_STATISTICS
		this->cabell.radio_scan[this->cabell.current_radio_channel]++;
#endif
	}
	checkSyncState();
	return channel_switch;
}

void CABELL::checkSyncState(void)
{
	if (testbit(this->cabell.state.protocol, CABELL_STATE_IN_SYNC)) {
		if ((this->cabell.timer.next_channel_switch - this->cabell.timer.last_packet_received) > ((uint32_t)CABELL_RESYNC_TIME_OUT)) initResync();		 
	} else {
		if (!(testbit(this->cabell.state.protocol, CABELL_STATE_POWER_ON_LOCK))) { 
			if (this->cabell.counter.sequential_hit > (CABELL_RADIO_CHANNELS * CABELL_RADIO_CHANNELS_BANDS)) {
				setbit(this->cabell.state.protocol, CABELL_STATE_POWER_ON_LOCK);
				setbit(this->cabell.state.protocol, CABELL_STATE_IN_SYNC);
				this->cabell.timer.sync_recovery = this->uptime->getTime() - this->cabell.timer.lost_sync;
				if (this->console) terminal_PrintEE(this->console, cabell_message_006);
			}
		} else {
			if (this->cabell.counter.sequential_hit > CABELL_SEQUENTIAL_HIT_TO_SYNC) {
				setbit(this->cabell.state.protocol, CABELL_STATE_IN_SYNC);
				this->cabell.timer.sync_recovery = this->uptime->getTime() - this->cabell.timer.lost_sync;
				if (this->console) terminal_PrintEE(this->console, cabell_message_007);
			}
		}
		if (this->cabell.counter.sequential_miss > CABELL_SEQUENTIAL_MISS_TO_RESYNC) initResync();
	}
}

void CABELL::updateSequentialHit(void)
{
	this->cabell.counter.sequential_hit++;
	this->cabell.counter.sequential_miss = 0;
}

void CABELL::updateSequentialMiss(void)
{
	this->cabell.counter.sequential_hit = 0;
	this->cabell.counter.sequential_miss++;
}

bool CABELL::processPackets(uint32_t *telemetry_emit_time)
{
	bool result = true;
	CABELL_RxPacket_t packet;
		
	this->cabell.counter.rx++;
	this->radio->readPayload(reinterpret_cast<uint8_t *>(&packet), sizeof(packet));

	*telemetry_emit_time = sendTelemetry();
	
	uint8_t tx_channel = packet.reserved & CABELL_RESERVED_CHANNEL_MASK;
	if (tx_channel) {
		if (tx_channel != this->cabell.current_radio_channel) {
			setbit(this->cabell.state.errors, CABELL_ERROR_CHANNEL_MISMATCH);
			this->cabell.current_radio_channel = tx_channel;
		}
	}
	
 	uint8_t *p = reinterpret_cast<uint8_t *>(&packet.rx_mode);
 	*p &= 0x7F;

	if (packet.rx_mode == CABELL_RXMODE_normalWithTelemetry) {
		setTelemetryPowerMode(packet.option);
		enableTelemetry(true);
	} else enableTelemetry(false);	
	
	uint16_t temp_rc_channels_value[CABELL_MAX_CHANNELS];
	uint8_t channel_reduction = packet.option & CABELL_CHANNEL_REDUCTION_MASK;
	if (channel_reduction > CABELL_CHANNEL_REDUCTION_MAX) result = false;  
	uint8_t max_payload_index = sizeof(packet.payload_value) - (((channel_reduction - (channel_reduction % 2)) / 2) * 3);  
	uint8_t channels_received = CABELL_MAX_CHANNELS - channel_reduction;
	
	if (result) {
		result = validateChecksum(&packet, max_payload_index);
		if (!(result)) {
			setbit(this->cabell.state.errors, CABELL_ERROR_CHECKSUM);
			this->cabell.counter.error_checksum++;
		}
	}
	if (result) result = decodeChannelValues(&packet, channels_received, temp_rc_channels_value);
	if (result) {
		updateRxCouters(packet.rx_mode);
		if (!(testbit(this->cabell.state.protocol, CABELL_STATE_REBOOT_PENDING))) {
			result = processRxMode(packet.rx_mode, packet.model_num, temp_rc_channels_value, channels_received);
		}
	}
	if (!(result)) this->cabell.counter.error_total++;
	return result;
}

void CABELL::updateRxCouters(uint8_t rx_mode)
{
	switch (rx_mode) {
		case CABELL_RXMODE_bindFalesafeNoPulse:
		case CABELL_RXMODE_bind:
		case CABELL_RXMODE_unBind:
			this->cabell.counter.bind++;
			break;
		case CABELL_RXMODE_normal:
		case CABELL_RXMODE_normalWithTelemetry:
			this->cabell.counter.normal++;
			break;
		case CABELL_RXMODE_setFailSafe:
			this->cabell.counter.set_failsafe++;
			break;
	}
}

bool CABELL::processRxMode(uint8_t rx_mode, uint8_t model_num, uint16_t *channels_value, uint8_t channels_count) 
{
	bool result = true;

	switch (rx_mode) {
		case CABELL_RXMODE_bindFalesafeNoPulse:
		case CABELL_RXMODE_bind: 
			if (testbit(this->cabell.state.protocol, CABELL_STATE_BIND_MODE)) {
				bindReciever(rx_mode, model_num, channels_value);
			} else {
				setbit(this->cabell.state.errors, CABELL_ERROR_BIND_IN_NORMAL_MODE);
				result = false;
			}
			break;	
		case CABELL_RXMODE_setFailSafe:  
			if (model_num == this->cabell.current_model) {
				setbit(this->cabell.state.request, CABELL_STATE_REQUEST_FAILSAFE_SAVE);
			} else {
				result = false;
				setbit(this->cabell.state.errors, CABELL_ERROR_WRONG_MODEL_NUMBER);
			}
			break;		
		case CABELL_RXMODE_normalWithTelemetry:
		case CABELL_RXMODE_normal:  
			if (model_num == this->cabell.current_model) {
				setServoChannelsValue(channels_value, channels_count);
			} else {
				result = false;
				setbit(this->cabell.state.errors, CABELL_ERROR_WRONG_MODEL_NUMBER);
			}
			break;
		case CABELL_RXMODE_unBind :  
			if (model_num == this->cabell.current_model) {
				unbindReciever();
			} else {
				result = false;
				setbit(this->cabell.state.errors, CABELL_ERROR_BIND_IN_NORMAL_MODE);
			}
			break;
		default : 
			setbit(this->cabell.state.errors, CABELL_ERROR_UNKNOWN_MODE);
			result = false;
			break;
	}
	return result;
}

void CABELL::unbindReciever(void)
{
	eeprom_write_byte(&eeprom_current_model, 0xFF);
	eeprom_write_byte(&eeprom_soft_bind_flag, 0xFF);
	uint64_t radio_id = ~(uint64_t)(0x00);
	eeprom_write_block(&radio_id, &eeprom_radio_pipe_address, 8);
	setbit(this->cabell.state.request, CABELL_STATE_REQUEST_FAILSAFE_ERASE);	
	if (this->console) terminal_PrintEE(this->console, cabell_message_004, true);
	setbit(this->cabell.state.protocol, CABELL_STATE_REBOOT_PENDING);
}

void CABELL::bindReciever(uint8_t mode, uint8_t model, uint16_t *channels_value) 
{
	// new radio address is in channels 11 to 15
	uint64_t new_radio_id = (((uint64_t)(channels_value[11] - 1000)) << 32) +
  		  	         (((uint64_t)(channels_value[12] - 1000)) << 24) +
				 (((uint64_t)(channels_value[13] - 1000)) << 16) +
				 (((uint64_t)(channels_value[14] - 1000)) << 8)  +
				 (((uint64_t)(channels_value[15] - 1000)));
	
	eeprom_write_byte(&eeprom_current_model, model);
	eeprom_write_block(&new_radio_id, &eeprom_radio_pipe_address, 8);
	if (mode == CABELL_RXMODE_bindFalesafeNoPulse) {
		eeprom_write_byte(&eeprom_soft_bind_flag, (uint8_t)CABELL_FAILSAFE_NO_PULSES);
	} else {
		eeprom_write_byte(&eeprom_soft_bind_flag, (uint8_t)CABELL_DO_NOT_BIND_ON_BOOT);
	}
	setbit(this->cabell.state.request, CABELL_STATE_REQUEST_FAILSAFE_DEFAULT);
	if (this->console) {
		terminal_PrintEE(this->console, cabell_message_001, true);
		terminal_PrintInt(this->console, model, true);
		terminal_PrintLn(this->console, "", true);
		terminal_PrintEE(this->console, cabell_message_002, true);
		terminal_PrintBuf(this->console, reinterpret_cast<const uint8_t *>(&new_radio_id), 5, true);
		terminal_PrintLn(this->console, "", true);
		terminal_PrintEE(this->console, cabell_message_003, true);
		terminal_FlushOutBuffer(this->console);		
	}
	setbit(this->cabell.state.protocol, CABELL_STATE_REBOOT_PENDING);
}

bool CABELL::decodeChannelValues(CABELL_RxPacket_t *packet, uint8_t channels_count, uint16_t *channels_value)
{
	uint8_t payload_index = 0;
	for (uint8_t i = 0; i < channels_count; i++) {
		*channels_value = (uint16_t)packet->payload_value[payload_index++];
		*channels_value |= (uint16_t)packet->payload_value[payload_index] << 8;
		if (i % 2) {							//channel number is ODD
			*channels_value = *channels_value >> 4;
			payload_index++;
		} else {							//channel number is EVEN
			*channels_value &= 0x0FFF;
		}
		if (!(testbit(this->cabell.state.protocol, CABELL_STATE_BIND_MODE))) {
			if ((*channels_value > CABELL_CHANNEL_MAX_VALUE) || (*channels_value < CABELL_CHANNEL_MIN_VALUE)) {
				setbit(this->cabell.state.errors, CABELL_ERROR_WHEN_DECODE_CHANNEL);
				return false;					// throw out entire packet if any value out of range
			}
		}
		channels_value++;
	}
	return true;
}

bool CABELL::validateChecksum(CABELL_RxPacket_t *packet, uint8_t max_index) 
{
	uint16_t packet_sum = packet->model_num + packet->option + packet->rx_mode + packet->reserved;
	for (uint8_t i = 0; i < max_index; i++) {
		packet_sum += packet->payload_value[i];
	}
	if (packet_sum != (((uint16_t)packet->checksum_msb << 8) + (uint16_t)packet->checksum_lsb)) {
		return false;
	} 
	return true;
}

bool CABELL::checkBindRequest(bool bind_button)
{
	uint8_t soft_bind_flag = eeprom_read_byte(&eeprom_soft_bind_flag);
	if (soft_bind_flag == CABELL_FAILSAFE_NO_PULSES) {
		soft_bind_flag = CABELL_DO_NOT_BIND_ON_BOOT;
		setbit(this->cabell.state.request, CABELL_STATE_REQUEST_FAILSAFE_NO_PULSES);
	}
	if ((!bind_button) || (soft_bind_flag != CABELL_DO_NOT_BIND_ON_BOOT)) {
		setbit(this->cabell.state.protocol, CABELL_STATE_BIND_MODE);
		this->cabell.radio_pipe_id = CABELL_BIND_RADIO_ADDR;
		if (this->console) terminal_PrintEE(this->console, cabell_message_000, true);
	} else {
		clrbit(this->cabell.state.protocol, CABELL_STATE_BIND_MODE);
		eeprom_read_block(&this->cabell.radio_pipe_id, &eeprom_radio_pipe_address, 8);
	}
	getChannelSequence(radio_channels, CABELL_RADIO_CHANNELS, this->cabell.radio_pipe_id);
	if (this->console) {
		terminal_PrintEE(this->console, cabell_message_008, true);
		terminal_PrintBuf(this->console, reinterpret_cast<const uint8_t *>(&this->cabell.radio_pipe_id), 5, true);
		terminal_PrintLn(this->console, "", true);
	}
	return testbit(this->cabell.state.protocol, CABELL_STATE_BIND_MODE);
}

void CABELL::readSetup(void)
{
	this->cabell.current_model = eeprom_read_byte(&eeprom_current_model);	
}

void CABELL::initResync(void)
{
	if (testbit(this->cabell.state.protocol, CABELL_STATE_IN_SYNC)) {
		if (this->console) terminal_PrintEE(this->console, cabell_message_005, true);
		clrbit(this->cabell.state.protocol, CABELL_STATE_IN_SYNC);
	}
	this->cabell.counter.resync++;
	this->cabell.timer.lost_sync = this->uptime->getTime();
	enableTelemetry(false);
	this->cabell.counter.sequential_miss = 0;
	this->cabell.timer.next_channel_switch = this->uptime->getTime() + (uint32_t)CABELL_RESYNC_WAIT;
	this->radio->setConfig(template_radio_config_rx);
	this->radio->flushRx();
	this->radio->clearInterrupt();
}

void CABELL::setServoChannelsValue(uint16_t *channels_value, uint8_t channels_count)
{
	if (testbit(this->cabell.state.protocol, CABELL_STATE_IN_SYNC) || testbit(this->cabell.state.protocol, CABELL_STATE_POWER_ON_LOCK)) {
		for (uint8_t i = 0; i < channels_count; i++) {
			this->servo_channels_value[i] = channels_value[i];
		}
		for (uint8_t i = channels_count; i < CABELL_MAX_CHANNELS; i++) {
			this->servo_channels_value[i] = CABELL_CHANNEL_MID_VALUE;
		}
	}
}

void CABELL::enableTelemetry(bool enable)
{
	if (testbit(this->cabell.state.protocol, CABELL_STATE_TELEMETRY_ENABLED) != enable) {
		if (enable) {
			setbit(this->cabell.state.protocol, CABELL_STATE_TELEMETRY_ENABLED);
			this->cabell.timer.packet_interval = CABELL_DEFAULT_PACKET_INTERVAL + CABELL_TELEMETRY_EXTEND_INTERVAL;
		} else {
			clrbit(this->cabell.state.protocol, CABELL_STATE_TELEMETRY_ENABLED);
			this->cabell.timer.packet_interval = CABELL_DEFAULT_PACKET_INTERVAL;
			initial_telemetry_skip_packets = 0;
		}
	}
}


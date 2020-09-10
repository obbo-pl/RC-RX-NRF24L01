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


#ifndef CABELL_NRF24L01_H_
#define CABELL_NRF24L01_H_

#include <avr/io.h>
#include <stdbool.h>
#include "config.h"
#include "nrf24l01_spi.h"
#include "terminal.h"
#include "uptime.h"
#include "RSSI.h"


// Channel
#define CABELL_RADIO_CHANNELS			9		// This is 1/5 of the total number of radio channels used for FHSS
#define CABELL_RADIO_CHANNELS_BANDS		5
#define CABELL_RADIO_MIN_CHANNEL_NUM		3

// RxPacket
#define CABELL_RX_PAYLOAD_BYTES			24		// 12 bits per value * 16 channels
#define CABELL_RESERVED_CHANNEL_MASK		0x3F
#define CABELL_DEFAULT_PACKET_INTERVAL		30
#define CABELL_TELEMETRY_EXTEND_INTERVAL	9
#define CABELL_MAX_PACKET_INTERVAL		40
#define CABELL_RESYNC_WAIT			((CABELL_RADIO_CHANNELS * CABELL_RADIO_CHANNELS_BANDS + 8) * CABELL_MAX_PACKET_INTERVAL)
#define CABELL_RESYNC_TIME_OUT			750
#define CABELL_PACKET_TIMEOUT_OFFSET		2
#define CABELL_SEQUENTIAL_HIT_TO_SYNC		5
#define CABELL_SEQUENTIAL_MISS_TO_RESYNC	5

// Telemetry
#define CABELL_TX_PAYLOAD_BYTES			2
#define CABELL_TELEMETRY_PACKET_LENGTH		4
#define CABELL_TELEMETRY_PACKETS_TO_SKIP	1000
#define CABELL_TELEMETRY_EMIT_INTERVAL		8

// Servo
#define CABELL_MAX_CHANNELS			CONFIG_CHANNELS_MAX_COUNT	// The maximum number of RC channels that can be sent in one packet
#define CABELL_MIN_CHANNELS			4				// The minimum number of channels that must be included in a packet
#define CABELL_CHANNEL_REDUCTION_MASK		0x0F
#define CABELL_CHANNEL_REDUCTION_MAX		(CABELL_MAX_CHANNELS - CABELL_MIN_CHANNELS)
#define CABELL_CHANNEL_MIN_VALUE		CONFIG_CHANNEL_MIN_VALUE
#define CABELL_CHANNEL_MAX_VALUE		CONFIG_CHANNEL_MAX_VALUE
#define CABELL_CHANNEL_MID_VALUE		((CABELL_CHANNEL_MIN_VALUE + CABELL_CHANNEL_MAX_VALUE) / 2)

#define CABELL_DO_NOT_BIND_ON_BOOT		0xAA
#define CABELL_FAILSAFE_NO_PULSES		0x99				// for compatibility only
#define CABELL_BIND_RADIO_ADDR			0xA4B7C123F7LL

#define CABELL_MASK_MAX_POWER_OVERRIDE		0x40


enum CABELL_STATE {
	CABELL_STATE_BIND_MODE,
	CABELL_STATE_REBOOT_PENDING,
	CABELL_STATE_IN_SYNC,
	CABELL_STATE_POWER_ON_LOCK,
	CABELL_STATE_INITIAL_GOOD_PACKET,
	CABELL_STATE_TELEMETRY_ENABLED,
	CABELL_STATE_SERVICE_MODE
};

enum CABELL_STATE_REQUEST {
	CABELL_STATE_REQUEST_FAILSAFE_DEFAULT,
	CABELL_STATE_REQUEST_FAILSAFE_ERASE,
	CABELL_STATE_REQUEST_FAILSAFE_SAVE,
	CABELL_STATE_REQUEST_FAILSAFE_NO_PULSES
};

enum CABELL_ERRORS {
	CABELL_ERROR_WRONG_MODEL_NUMBER,
	CABELL_ERROR_CHECKSUM,
	CABELL_ERROR_WHEN_DECODE_CHANNEL,
	CABELL_ERROR_UNKNOWN_MODE,
	CABELL_ERROR_BIND_IN_NORMAL_MODE,
	CABELL_ERROR_CHANNEL_MISMATCH
};



typedef struct {
	uint32_t rx;
	uint32_t tx;
	uint32_t normal;
	uint32_t bind;	
	uint32_t set_failsafe;
	uint32_t error_checksum;
	uint32_t error_total;
	uint32_t resync;
	uint32_t lost;
	uint32_t sequential_hit;
	uint32_t sequential_miss;
} CABELL_Counters_t;

typedef struct {
	uint32_t probe;
	uint32_t good;
} CABELL_SignalPower_t;

typedef struct {
	uint32_t next_channel_switch;
	uint32_t last_packet_received;
	uint32_t last_good_packet;			
	uint32_t packet_interval;
	uint32_t sync_recovery;
	uint32_t lost_sync;
} CABELL_Timers_t;
		
typedef struct {
	uint8_t protocol;
	uint8_t request;
	uint8_t errors;
} CABELL_State_t;
		
typedef struct {
	CABELL_Counters_t counter;
	CABELL_SignalPower_t signal_power;
	CABELL_Timers_t timer;
	CABELL_State_t state;
	uint8_t current_radio_channel;
	uint8_t current_model;
	uint64_t radio_pipe_id;
	uint8_t power_mode;
#ifdef TERMINAL_CABELL_FULL_STATISTICS
	uint16_t radio_scan[CABELL_RADIO_CHANNELS * CABELL_RADIO_CHANNELS_BANDS + CABELL_RADIO_MIN_CHANNEL_NUM];
	uint32_t len_less;
	uint32_t len_2700;
	uint32_t len_2900;
	uint32_t len_3100;
	uint32_t len_3300;
	uint32_t len_3500;
	uint32_t len_3700;
	uint32_t len_3900;
	uint32_t len_4100;
	uint32_t len_more;	
#endif
} CABELL_t;

	
class CABELL
{
	public:
		CABELL(NRF24L01 *radio, UpTime *uptime, bool service_mode, uint16_t *servo_channels);
		
		CABELL_t cabell;
		bool init(void);
		void bindTerminal(CircularBuffer_t *console);
		void readSetup(void);
		void initRadio(void);
		void setNextRadioChannel(void);
		bool exchangePacket(uint32_t *telemetry_emit_time);
		bool checkBindRequest(bool bind_button);
		void unbindReciever(void);
		void getState(uint8_t *protocol, uint8_t *errors);
		void setTelemetryFrame(uint8_t lipo1, uint8_t lipo2);
		void saveModel(uint8_t model);
		void bindReciever(uint8_t model, uint16_t *channels_value);
		void bindReciever(uint8_t mode, uint8_t model, uint16_t *channels_value);


	private:
		// Note bit 8 is used to indicate if the packet is the first of 2 on the channel. 
		// Mask out this bit before using the enum
		enum RxTxMode_t {		
			CABELL_RXMODE_normal                 = 0,
			CABELL_RXMODE_bind                   = 1,
			CABELL_RXMODE_setFailSafe            = 2,
			CABELL_RXMODE_normalWithTelemetry    = 3,
			CABELL_RXMODE_telemetryResponse      = 4,
			CABELL_RXMODE_bindFalesafeNoPulse    = 5,
			CABELL_RXMODE_unBind                 = 127
		};
		
		typedef struct {
			RxTxMode_t rx_mode;
			uint8_t reserved;      /* Contains the channel number that the packet was sent on in bits 0-5 */
			uint8_t option;        /* mask 0x0F    : Channel reduction.  
						*  			    The number of channels to not send (subtracted from the 16 max channels)
						*                at least 4 are always sent
						* mask 0x30>>4 : Receiver output mode
						*                0 (00) = Single PPM on individual pins for each channel 
						*                1 (01) = SUM PPM on channel 1 pin
						*                2 (10) = SBUS output
						*                3 (11) = Unused
						* mask 0x40>>6   Contains max power override flag for Multi-protocol TX module. Also sent to RX
						* mask 0x80>>7   Unused 
						*/  
			uint8_t  model_num;
			uint8_t  checksum_lsb;  /* Checksum least significant byte */
			uint8_t  checksum_msb;  /* Checksum most significant byte */
			uint8_t  payload_value[CABELL_RX_PAYLOAD_BYTES]; /* 12 bits per channel value, unsigned */
		} CABELL_RxPacket_t;   
		
		uint16_t *servo_channels_value;
		NRF24L01 *radio;
		UpTime *uptime;
		RSSI rssi;
		CircularBuffer_t *console;
		uint8_t radio_channels[CABELL_RADIO_CHANNELS];
		uint8_t telemetry_packet[CABELL_TELEMETRY_PACKET_LENGTH];
		uint8_t template_radio_config_rx;
		uint8_t template_radio_config_tx;
		uint16_t initial_telemetry_skip_packets;

		void clearState(void);
		void clearTimer(void);
		void clearCouters(void);
		void clearSignalPowerCounters(void);
#ifdef TERMINAL_CABELL_FULL_STATISTICS
		void clearRadioScan(void);
#endif
		bool processPackets(uint32_t *telemetry_emit_time);
		bool validateChecksum(CABELL_RxPacket_t *packet, uint8_t max_index);
		bool decodeChannelValues(CABELL_RxPacket_t *packet, uint8_t channels_count, uint16_t *channels_value);
		void updateRxCouters(uint8_t rx_mode);
		bool processRxMode (uint8_t rx_mode, uint8_t model_num, uint16_t *channels_value, uint8_t channels_count);
		void setServoChannelsValue(uint16_t *channels_value, uint8_t channels_count);
		void enableTelemetry(bool enable);
		uint32_t sendTelemetry(void);
		void initResync(void);
		void updateSequentialHit(void);
		void updateSequentialMiss(void);
		void checkSyncState(void);
		void setTelemetryPowerMode(uint8_t option);
};


#endif /* CABELL_NRF24L01_H_ */
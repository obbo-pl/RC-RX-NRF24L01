/*
 * NRF24L01_SPI.h
 *
 * Created: 2020-05-14 23:19:53
 * Atmel Studio 6 (Version: 6.2.1563 - Service Pack 2)
 * AVR/GNU C Compiler : 4.8.1
 *  Author: Krzysztof Markiewicz
 *  www.obbo.pl
 *
 * This program is distributed under the terms of the GNU General Public License
 */ 


#ifndef NRF24L01_SPI_H_
#define NRF24L01_SPI_H_

#include <avr/io.h>
#include <stdbool.h>
#include "nrf24l01_def.h"
#include "spi.h"


enum NRF24L01_BITRATE {
	NRF24L01_BITRATE_2M	= NRF24L01_06_BR_2M_bm,
	NRF24L01_BITRATE_1M	= NRF24L01_06_BR_1M_bm,
	NRF24L01_BITRATE_250k	= NRF24L01_06_BR_250k_bm
};

enum NRF24L01_TX_POWER {
	NRF24L01_TX_POWER_MIN 	= NRF24L01_06_TXPWR_m18_bm,
	NRF24L01_TX_POWER_LOW	= NRF24L01_06_TXPWR_m12_bm,
	NRF24L01_TX_POWER_HIGH	= NRF24L01_06_TXPWR_m6_bm,
	NRF24L01_TX_POWER_MAX	= NRF24L01_06_TXPWR_0_bm
};

enum NRF24L01_RETRANSMIT_DELAY {
	NRF24L01_RETRANSMIT_DELAY_250us		= NRF24L01_04_WAIT_250us_bm,
	NRF24L01_RETRANSMIT_DELAY_500us		= NRF24L01_04_WAIT_500us_bm,
	NRF24L01_RETRANSMIT_DELAY_750us		= NRF24L01_04_WAIT_750us_bm,
	NRF24L01_RETRANSMIT_DELAY_1000us	= NRF24L01_04_WAIT_1000us_bm,
	NRF24L01_RETRANSMIT_DELAY_1250us	= NRF24L01_04_WAIT_1250us_bm,
	NRF24L01_RETRANSMIT_DELAY_1500us	= NRF24L01_04_WAIT_1500us_bm,
	NRF24L01_RETRANSMIT_DELAY_1750us	= NRF24L01_04_WAIT_1750us_bm,
	NRF24L01_RETRANSMIT_DELAY_2000us	= NRF24L01_04_WAIT_2000us_bm,
	NRF24L01_RETRANSMIT_DELAY_2250us	= NRF24L01_04_WAIT_2250us_bm,
	NRF24L01_RETRANSMIT_DELAY_2500us	= NRF24L01_04_WAIT_2500us_bm,
	NRF24L01_RETRANSMIT_DELAY_2750us	= NRF24L01_04_WAIT_2750us_bm,
	NRF24L01_RETRANSMIT_DELAY_3000us	= NRF24L01_04_WAIT_3000us_bm,
	NRF24L01_RETRANSMIT_DELAY_3250us	= NRF24L01_04_WAIT_3250us_bm,
	NRF24L01_RETRANSMIT_DELAY_3500us	= NRF24L01_04_WAIT_3500us_bm,
	NRF24L01_RETRANSMIT_DELAY_3750us	= NRF24L01_04_WAIT_3750us_bm,
	NRF24L01_RETRANSMIT_DELAY_4000us	= NRF24L01_04_WAIT_4000us_bm
};

#define NRF24L01_DISABLE_DELAYS	


class NRF24L01
{
	public:
		NRF24L01(SPI *spi, uint8_t volatile *ce_regddr, uint8_t volatile *ce_regport, uint8_t ce,
				 uint8_t volatile *ss_regddr, uint8_t volatile *ss_regport, uint8_t ss);
	
		uint8_t readRegister(uint8_t reg);
		void readRegister(uint8_t reg, uint8_t *data, uint8_t length);
		void readPayload(uint8_t *data, uint8_t length);
		void writePayload(uint8_t *data, uint8_t length);
		void activateFeatures(void);
		void flushTx(void);
		void flushRx(void);
		void setChipEnable(bool enable);
		void setBitrate(enum NRF24L01_BITRATE bitrate);
		void setTxPower(enum NRF24L01_TX_POWER power);
		void powerUp(void);
		void powerDown(void);
		void setTxMode(void);
		void setRxMode(void);
		void setConfig(uint8_t config);
		uint8_t getConfig(void);
		void setCRC(uint8_t byte);
		void setAddressWidth(uint8_t address_width);
		uint8_t getAddressWidth(void);
		bool readPowerDetector(void);
		bool setChannel(uint8_t channel);
		bool isRxPacektInterupt(void);
		bool isRxPacektAvailable(void);
		void setRetries(enum NRF24L01_RETRANSMIT_DELAY delay, uint8_t count);
		void clearInterrupt(void);
		void clearInterrupt(bool rx, bool tx, bool rt);
		void maskInterrupt(bool rx, bool tx, bool rt);
		bool isChipConnected(void);
		void enableDynamicPayloads(bool enable);
		void enableDynamicPayloads(uint8_t pipe_id, bool enable);
		void setPayloadLength(uint8_t length);
		void setPayloadLength(uint8_t pipe_id, uint8_t length);
		void setAutoAck(bool enable);
		void setAutoAck(uint8_t pipe_id, bool enable);
		void enablePipe(uint8_t pipe_id, bool enable);
		void openWritingPipe(uint64_t address, bool ack);
		void openReadingPipe(uint8_t pipe_id, uint64_t address);
				 
	private:
		typedef struct {
			uint8_t volatile *regport;
			uint8_t pin;
		} NRF24L01_Signal_t;

		typedef struct {
			NRF24L01_Signal_t ce;
			NRF24L01_Signal_t ss;
			bool dynamic_payload;
			uint8_t address_width; 
		} NRF24L01_t;

		SPI *spi;
		NRF24L01_t nrf24;

		void writeRegister(uint8_t reg, uint8_t data);
		void writeRegister(uint8_t reg, uint8_t *data, uint8_t length);
		void strobe(uint8_t state);
		
};


#endif /* NRF24L01_SPI_H_ */


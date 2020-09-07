/*
 * NRF24L01_SPI.cpp
 *
 * Created: 2020-05-14 23:19:53
 * Atmel Studio 6 (Version: 6.2.1563 - Service Pack 2)
 * AVR/GNU C Compiler : 4.8.1
 *  Author: Krzysztof Markiewicz
 *  www.obbo.pl
 *
 * This program is distributed under the terms of the GNU General Public License
 */ 


#include <util/delay.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include "nrf24l01_spi.h"
#include "macro.h"


const uint8_t pipe_register_address[NRF24L01_RX_PIPE_COUNT] PROGMEM = {NRF24L01_0A_RX_ADDR_P0, NRF24L01_0B_RX_ADDR_P1, NRF24L01_0C_RX_ADDR_P2, 
								       NRF24L01_0D_RX_ADDR_P3, NRF24L01_0E_RX_ADDR_P4, NRF24L01_0F_RX_ADDR_P5};
const uint8_t pipe_register_payload_size[NRF24L01_RX_PIPE_COUNT] PROGMEM = {NRF24L01_11_RX_PW_P0, NRF24L01_12_RX_PW_P1, NRF24L01_13_RX_PW_P2, 
								            NRF24L01_14_RX_PW_P3, NRF24L01_15_RX_PW_P4, NRF24L01_16_RX_PW_P5};

#define NRF_CE_LOW		{char cSREG = SREG; cli(); clrbit(*(nrf24.ce.regport), nrf24.ce.pin); SREG = cSREG;}
#define NRF_CE_HIGH		{char cSREG = SREG; cli(); setbit(*(nrf24.ce.regport), nrf24.ce.pin); SREG = cSREG;}
#define NRF_SS_LOW		{char cSREG = SREG; cli(); clrbit(*(nrf24.ss.regport), nrf24.ss.pin); SREG = cSREG;}
#define NRF_SS_HIGH		{char cSREG = SREG; cli(); setbit(*(nrf24.ss.regport), nrf24.ss.pin); SREG = cSREG;}

// The SPI Master initiates the communication cycle when pulling low the SlaveSelect
#define SLAVE_SELECT		NRF_SS_LOW
#define SLAVE_UNSELECT		NRF_SS_HIGH


NRF24L01::NRF24L01(SPI *spi, uint8_t volatile *ce_regddr, uint8_t volatile *ce_regport, uint8_t ce,
				   uint8_t volatile *ss_regddr, uint8_t volatile *ss_regport, uint8_t ss)
{
	nrf24.ss.regport = ss_regport;
	nrf24.ss.pin = ss;
	setbit(*ss_regddr, nrf24.ss.pin);	// output
	NRF_SS_HIGH;
	nrf24.ce.regport = ce_regport;
	nrf24.ce.pin = ce;
	setbit(*ce_regddr, nrf24.ce.pin);	// output
	NRF_CE_LOW;
	this->spi = spi;
	
	nrf24.dynamic_payload = false;
	nrf24.address_width = 5;
}

uint8_t NRF24L01::readRegister(uint8_t reg)
{
	uint8_t result;
	SLAVE_SELECT;
	this->spi->write(NRF24L01_00_R_REGISTER | (NRF24L01_REGISTER_MASK & reg));
	result = this->spi->read();
	SLAVE_UNSELECT;
	return result;
}

void NRF24L01::readRegister(uint8_t reg, uint8_t *data, uint8_t length)
{
	SLAVE_SELECT;
	this->spi->write(NRF24L01_00_R_REGISTER | (NRF24L01_REGISTER_MASK & reg));
	while (length--) *data++ = this->spi->read();
	SLAVE_UNSELECT;
}

void NRF24L01::writeRegister(uint8_t reg, uint8_t data)
{
	SLAVE_SELECT;
	this->spi->write(NRF24L01_20_W_REGISTER | (NRF24L01_REGISTER_MASK & reg));
	this->spi->write(data);
	SLAVE_UNSELECT;
}

void NRF24L01::writeRegister(uint8_t reg, uint8_t *data, uint8_t length)
{
	SLAVE_SELECT;
	this->spi->write(NRF24L01_20_W_REGISTER | ( NRF24L01_REGISTER_MASK & reg));
	while (length--) this->spi->write(*data++);
	SLAVE_UNSELECT;
}

void NRF24L01::readPayload(uint8_t *data, uint8_t length)
{
	uint8_t refill = 0;
	if (!(nrf24.dynamic_payload)) refill = NRF24L01_PAYLOAD_SIZE_MAX - length;
	SLAVE_SELECT;
	this->spi->write(NRF24L01_61_RX_PAYLOAD);
	while (length--) *data++ = this->spi->read();
	while (refill--) this->spi->read();
	SLAVE_UNSELECT;
}

void NRF24L01::writePayload(uint8_t *data, uint8_t length)
{		
	uint8_t refill = 0;
	if (!(nrf24.dynamic_payload)) refill = NRF24L01_PAYLOAD_SIZE_MAX - length;
	SLAVE_SELECT;
	this->spi->write(NRF24L01_A0_TX_PAYLOAD);
	while (length--) this->spi->write(*data++);
	while (refill--) this->spi->write(0);
	SLAVE_UNSELECT;
}

void NRF24L01::strobe(uint8_t state)
{
	SLAVE_SELECT;
	this->spi->write(state);
	SLAVE_UNSELECT;
}

void NRF24L01::activateFeatures(void)
{
	// A new ACTIVATE command with the same data deactivates them again.
	// This is executable in power down or stand by modes only.
	// nRF24L01 only, the ACTIVATE command will be ignored on the nRF24L01+
	SLAVE_SELECT;
	this->spi->write(NRF24L01_50_ACTIVATE);
	this->spi->write(0x73);
	SLAVE_UNSELECT;
}

void NRF24L01::flushTx(void)
{
	this->strobe(NRF24L01_E1_FLUSH_TX);
}

void NRF24L01::flushRx(void)
{
	this->strobe(NRF24L01_E2_FLUSH_RX);
}

void NRF24L01::setChipEnable(bool enable)
{
	if (enable) {
		NRF_CE_HIGH;
		_delay_us(10);
#ifndef NRF24L01_DISABLE_DELAYS
		_delay_us(130);
#endif
	} else {
		NRF_CE_LOW;
	}
}

void NRF24L01::setBitrate(enum NRF24L01_BITRATE bitrate)
{
	uint8_t state  = this->readRegister(NRF24L01_06_RF_SETUP);
	state &= ~(NRF24L01_06_BR_MASK);
	this->writeRegister(NRF24L01_06_RF_SETUP, (state | bitrate));
}

void NRF24L01::setTxPower(enum NRF24L01_TX_POWER power)
{
	uint8_t state  = this->readRegister(NRF24L01_06_RF_SETUP);
	state &= ~(NRF24L01_06_TXPWR_MASK);
	this->writeRegister(NRF24L01_06_RF_SETUP, (state | power));
};

void NRF24L01::powerUp(void)
{
	uint8_t state = this->readRegister(NRF24L01_00_CONFIG);
	if (!(state & NRF24L01_00_PWR_UP_bm)) {
		state |= NRF24L01_00_PWR_UP_bm;
		this->writeRegister(NRF24L01_00_CONFIG, state);
#ifndef NRF24L01_DISABLE_DELAYS
		_delay_ms(5);
#endif
	}
}

void NRF24L01::powerDown(void)
{
	uint8_t state = this->readRegister(NRF24L01_00_CONFIG);
	if (state & NRF24L01_00_PWR_UP_bm) {
		state &= ~(NRF24L01_00_PWR_UP_bm);
		this->writeRegister(NRF24L01_00_CONFIG, state);
	}
}

void NRF24L01::setTxMode(void)
{
	uint8_t state = this->readRegister(NRF24L01_00_CONFIG);
	state &= ~(NRF24L01_00_PRIM_RX_bm);
	this->writeRegister(NRF24L01_00_CONFIG, state);
}

void NRF24L01::setRxMode(void)
{
	uint8_t state = this->readRegister(NRF24L01_00_CONFIG);
	state |= NRF24L01_00_PRIM_RX_bm;
	this->writeRegister(NRF24L01_00_CONFIG, state);
}

void NRF24L01::setConfig(uint8_t config)
{
	this->writeRegister(NRF24L01_00_CONFIG, config);
}

uint8_t NRF24L01::getConfig(void)
{
	return this->readRegister(NRF24L01_00_CONFIG);
}

void NRF24L01::setCRC(uint8_t byte)
{
	// byte = 0 - disable CRC
	// byte = 1 - enable 1 byte CRC
	// byte = 2 - enable 2 bytes CRC
	uint8_t state = this->readRegister(NRF24L01_00_CONFIG);
	state &= ~(NRF24L01_00_CRC_MASK);
	if (byte) state |= NRF24L01_00_EN_CRC_bm;
	if (byte > 1) state |= NRF24L01_00_CRCO_bm;
	this->writeRegister(NRF24L01_00_CONFIG, state);
}

void NRF24L01::setAddressWidth(uint8_t address_width)
{
	address_width = (address_width - 2) & NRF24L01_03_AW_MASK;
	if (address_width) {
		this->writeRegister(NRF24L01_03_SETUP_AW, address_width);
		nrf24.address_width = address_width + 2;
	}
}

uint8_t NRF24L01::getAddressWidth(void)
{
	return ((this->readRegister(NRF24L01_03_SETUP_AW) & NRF24L01_03_AW_MASK) + 2);
}

bool NRF24L01::readPowerDetector(void)
{
#ifndef NRF24L01_DISABLE_DELAYS
	_delay_us(40);
#endif
	return (this->readRegister(NRF24L01_09_CD) & RF24L01_09_CD_RPD_bm);
}

bool NRF24L01::setChannel(uint8_t channel)
{
	if (channel <= NRF24L01_CHANNEL_MAX) {
		this->writeRegister(NRF24L01_05_RF_CH, channel);
		return true;
	}
	return false;
}

bool NRF24L01::isRxPacektInterupt(void)
{
	if (this->readRegister(NRF24L01_07_STATUS) & NRF24L01_07_RX_DR_bm) {
		return true;
	}
	return false;
}

bool NRF24L01::isRxPacektAvailable(void)
{
	if ((this->readRegister(NRF24L01_17_FIFO_STATUS) & NRF24L01_17_RX_EMPTY_bm ) == 0) {
		return true;
	}
	return false;
}

void NRF24L01::setRetries(enum NRF24L01_RETRANSMIT_DELAY delay, uint8_t count)
{
	if (count > NRF24L01_04_RETR_UPTO15_bm) count = NRF24L01_04_RETR_UPTO15_bm;
	this->writeRegister(NRF24L01_04_SETUP_RETR, (delay | count));
}

void NRF24L01::clearInterrupt(void)
{
	this->clearInterrupt(true, true, true);
}

void NRF24L01::clearInterrupt(bool rx, bool tx, bool rt)
{
	uint8_t byte = 0;
	if (rx) byte |= NRF24L01_07_RX_DR_bm;
	if (tx) byte |= NRF24L01_07_TX_DS_bm;
	if (rt) byte |= NRF24L01_07_MAX_RT_bm;
	this->writeRegister(NRF24L01_07_STATUS, byte);
}

void NRF24L01::maskInterrupt(bool rx, bool tx, bool rt)
{
	uint8_t state = this->readRegister(NRF24L01_00_CONFIG);
	if (rx) state |= NRF24L01_00_MASK_RX_DR_bm;
	else state &= ~(NRF24L01_00_MASK_RX_DR_bm);
	if (tx) state |= NRF24L01_00_MASK_TX_DS_bm;
	else state &= ~(NRF24L01_00_MASK_TX_DS_bm);
	if (rt) state |= NRF24L01_00_MASK_MAX_RT_bm;
	else state &= ~(NRF24L01_00_MASK_MAX_RT_bm);
	this->writeRegister(NRF24L01_00_CONFIG, state);
}

bool NRF24L01::isChipConnected(void)
{
	uint8_t aw = this->readRegister(NRF24L01_03_SETUP_AW);
	if ((aw & NRF24L01_03_AW_MASK) && !(aw & ~NRF24L01_03_AW_MASK)) return true;
	return false;
}

void NRF24L01::enableDynamicPayloads(bool enable)
{
	uint8_t state = this->readRegister(NRF24L01_1D_FEATURE);
	if (enable) {
		state |= NRF2401_1D_EN_DPL_bm;
		this->writeRegister(NRF24L01_1D_FEATURE, state);
		this->writeRegister(NRF24L01_1C_DYNPD, NRF24L01_1C_DYNPD_MASK);
	} else {
		state &= ~(NRF2401_1D_EN_DPL_bm);
		this->writeRegister(NRF24L01_1C_DYNPD, 0);
		this->writeRegister(NRF24L01_1D_FEATURE, state);
	}
	nrf24.dynamic_payload = enable;
}

void NRF24L01::enableDynamicPayloads(uint8_t pipe_id, bool enable)
{
	uint8_t state = this->readRegister(NRF24L01_1D_FEATURE);
	uint8_t pipes = this->readRegister(NRF24L01_1C_DYNPD);
	if (enable) {
		state |= NRF2401_1D_EN_DPL_bm;
		this->writeRegister(NRF24L01_1D_FEATURE, state);
		pipes |= (NRF24L01_1C_DYNPD_MASK & (1 << pipe_id));
		this->writeRegister(NRF24L01_1C_DYNPD, pipes);
		nrf24.dynamic_payload = true;
	} else {
		pipes &= ~(NRF24L01_1C_DYNPD_MASK & (1 << pipe_id));
		this->writeRegister(NRF24L01_1C_DYNPD, pipes);
		if (pipes == 0) {
			state &= ~(NRF2401_1D_EN_DPL_bm);
			this->writeRegister(NRF24L01_1D_FEATURE, state);
			nrf24.dynamic_payload = false;
		}
	}
}

void NRF24L01::setPayloadLength(uint8_t length)
{
	if (length > NRF24L01_PAYLOAD_SIZE_MAX) length = NRF24L01_PAYLOAD_SIZE_MAX;
	for (uint8_t i = 0; i < NRF24L01_RX_PIPE_COUNT; i++) {
		this->writeRegister(pgm_read_byte(&pipe_register_payload_size[i]), length);
	}
}

void NRF24L01::setPayloadLength(uint8_t pipe_id, uint8_t length)
{
	if (pipe_id < NRF24L01_RX_PIPE_COUNT) {
		if (length > NRF24L01_PAYLOAD_SIZE_MAX) length = NRF24L01_PAYLOAD_SIZE_MAX;
		this->writeRegister(pgm_read_byte(&pipe_register_payload_size[pipe_id]), length);
	}
}

void NRF24L01::setAutoAck(bool enable)
{
	if (enable) this->writeRegister(NRF24L01_01_EN_AA, NRF24L01_01_EN_MASK);
	else this->writeRegister(NRF24L01_01_EN_AA, 0);
}

void NRF24L01::setAutoAck(uint8_t pipe_id, bool enable)
{
	uint8_t state = this->readRegister(NRF24L01_01_EN_AA);
	if (enable) state |= (NRF24L01_01_EN_MASK & (1 << pipe_id));
	else state &= ~(NRF24L01_01_EN_MASK & (1 << pipe_id));
	this->writeRegister(NRF24L01_01_EN_AA, state);
}

void NRF24L01::enablePipe(uint8_t pipe_id, bool enable)
{
		uint8_t state = this->readRegister(NRF24L01_02_EN_RXADDR);
		if (enable) state |= (NRF24L01_02_ERX_MASK & (1 << pipe_id));		
		else state &= ~(NRF24L01_02_ERX_MASK & (1 << pipe_id));
		this->writeRegister(NRF24L01_02_EN_RXADDR, state);
}

void NRF24L01::openWritingPipe(uint64_t address, bool ack)
{
	this->writeRegister(NRF24L01_10_TX_ADDR, reinterpret_cast<uint8_t *>(&address), nrf24.address_width);
	if (ack) this->writeRegister(NRF24L01_0A_RX_ADDR_P0, reinterpret_cast<uint8_t *>(&address), nrf24.address_width);
}

void NRF24L01::openReadingPipe(uint8_t pipe_id, uint64_t address)
{
	if (pipe_id < NRF24L01_RX_PIPE_COUNT) {
		if ( pipe_id < 2 ) {
			this->writeRegister(pgm_read_byte(&pipe_register_address[pipe_id]), reinterpret_cast<uint8_t *>(&address), nrf24.address_width);
		} else {
			// LSByte is written first
			this->writeRegister(pgm_read_byte(&pipe_register_address[pipe_id]), reinterpret_cast<uint8_t *>(&address), 1);
		}
		this->enablePipe(pipe_id, true);
	}
}


/*
 * nrf24l01_def.h
 *
 * Created: 2020-05-14 23:19:52
 * Atmel Studio 6 (Version: 6.2.1563 - Service Pack 2)
 * AVR/GNU C Compiler : 4.8.1
 *  Author: Krzysztof Markiewicz
 *  www.obbo.pl
 *
 * This program is distributed under the terms of the GNU General Public License
 */ 

#ifndef NRF24L01_DEF_H_
#define NRF24L01_DEF_H_


#define NRF24L01_CHANNEL_MAX		(125)
#define NRF24L01_PAYLOAD_SIZE_MAX	(32)
#define NRF24L01_RX_PIPE_COUNT		(6)

// Register map
#define NRF24L01_00_CONFIG		0x00		// Configuration Register
enum {
	NRF24L01_00_MASK_RX_DR_bm	= 0x40,		// Mask interrupt caused by RX_DR
	NRF24L01_00_MASK_TX_DS_bm	= 0x20,		// Mask interrupt caused by TX_DS
	NRF24L01_00_MASK_MAX_RT_bm	= 0x10,		// Mask interrupt caused by MAX_RT
	NRF24L01_00_CRC_MASK		= 0x0C,
 	NRF24L01_00_EN_CRC_bm		= 0x08,		// Enable CRC
 	NRF24L01_00_CRCO_bm		= 0x04,		// CRC encoding scheme: '0' - 1 byte, '1' - 2 bytes
	NRF24L01_00_PWR_UP_bm		= 0x02,		// '1' - POWER UP, '0' - POWER DOWN
	NRF24L01_00_PRIM_RX_bm		= 0x01
};
#define NRF24L01_01_EN_AA		0x01		// Enable ‘Auto Acknowledgment’ Function
enum {
	NRF24L01_01_EN_MASK		= 0x3F,
	NRF24L01_01_EN_P5_bm		= 0x20,
	NRF24L01_01_EN_P4_bm		= 0x10,
	NRF24L01_01_EN_P3_bm		= 0x08,
	NRF24L01_01_EN_P2_bm		= 0x04,
	NRF24L01_01_EN_P1_bm		= 0x02,
	NRF24L01_01_EN_P0_bm		= 0x01
};
#define NRF24L01_02_EN_RXADDR		0x02		// Enabled RX Addresses
enum {
	NRF24L01_02_ERX_MASK		= 0x3F,
	NRF24L01_02_ERX_P5_bm		= 0x20,
	NRF24L01_02_ERX_P4_bm		= 0x10,
	NRF24L01_02_ERX_P3_bm		= 0x08,
	NRF24L01_02_ERX_P2_bm		= 0x04,
	NRF24L01_02_ERX_P1_bm		= 0x02,
	NRF24L01_02_ERX_P0_bm		= 0x01
};
#define NRF24L01_03_SETUP_AW		0x03		// Setup of Address Widths
enum {
	NRF24L01_03_AW_MASK		= 0x03,
	NRF24L01_03_AW_5_bm		= 0x03,
	NRF24L01_03_AW_4_bm		= 0x02,
	NRF24L01_03_AW_3_bm		= 0x01	
};
#define NRF24L01_04_SETUP_RETR		0x04		// Setup of Automatic Retransmission
enum {
	NRF24L01_04_WAIT_250us_bm	= 0x00,		// Auto Retransmit Delay
	NRF24L01_04_WAIT_500us_bm	= 0x10,
	NRF24L01_04_WAIT_750us_bm	= 0x20,
	NRF24L01_04_WAIT_1000us_bm	= 0x30,
	NRF24L01_04_WAIT_1250us_bm	= 0x40,
	NRF24L01_04_WAIT_1500us_bm	= 0x50,
	NRF24L01_04_WAIT_1750us_bm	= 0x60,
	NRF24L01_04_WAIT_2000us_bm	= 0x70,
	NRF24L01_04_WAIT_2250us_bm	= 0x80,
	NRF24L01_04_WAIT_2500us_bm	= 0x90,
	NRF24L01_04_WAIT_2750us_bm	= 0xA0,
	NRF24L01_04_WAIT_3000us_bm	= 0xB0,
	NRF24L01_04_WAIT_3250us_bm	= 0xC0,
	NRF24L01_04_WAIT_3500us_bm	= 0xD0,
	NRF24L01_04_WAIT_3750us_bm	= 0xE0,
	NRF24L01_04_WAIT_4000us_bm	= 0xF0,
	NRF24L01_04_RETR_DISABLE_bm	= 0x00,		// Auto Retransmit Count
	NRF24L01_04_RETR_UPTO1_bm	= 0x01,
	NRF24L01_04_RETR_UPTO2_bm	= 0x02,
	NRF24L01_04_RETR_UPTO3_bm	= 0x03,
	NRF24L01_04_RETR_UPTO4_bm	= 0x04,
	NRF24L01_04_RETR_UPTO5_bm	= 0x05,
	NRF24L01_04_RETR_UPTO6_bm	= 0x06,
	NRF24L01_04_RETR_UPTO7_bm	= 0x07,
	NRF24L01_04_RETR_UPTO8_bm	= 0x08,
	NRF24L01_04_RETR_UPTO9_bm	= 0x09,
	NRF24L01_04_RETR_UPTO10_bm	= 0x0A,
	NRF24L01_04_RETR_UPTO11_bm	= 0x0B,
	NRF24L01_04_RETR_UPTO12_bm	= 0x0C,
	NRF24L01_04_RETR_UPTO13_bm	= 0x0D,
	NRF24L01_04_RETR_UPTO14_bm	= 0x0E,
	NRF24L01_04_RETR_UPTO15_bm	= 0x0F
};
#define NRF24L01_05_RF_CH		0x05		// RF Channel
#define NRF24L01_06_RF_SETUP		0x06		// RF Setup Register
enum {
	NRF24L01_06_BR_MASK		= 0x28,		// RF Data Rate
	NRF24L01_06_BR_2M_bm		= 0x08,
	NRF24L01_06_BR_1M_bm		= 0x00,
	NRF24L01_06_BR_250k_bm		= 0x20,
	NRF24L01_06_TXPWR_MASK		= 0x06,		// output power in TX mode
	NRF24L01_06_TXPWR_m18_bm	= 0x00,		// -18dBm
	NRF24L01_06_TXPWR_m12_bm	= 0x02,		// -12dBm
	NRF24L01_06_TXPWR_m6_bm		= 0x04,		// -6dBm
	NRF24L01_06_TXPWR_0_bm		= 0x06		// 0dBm
};
#define NRF24L01_07_STATUS		0x07		// Status Register
enum {
	NRF24L01_07_INT_MASK		= 0x70,	
	NRF24L01_07_RX_DR_bm		= 0x40,
	NRF24L01_07_TX_DS_bm		= 0x20,
	NRF24L01_07_MAX_RT_bm		= 0x10,
	NRF24L01_07_PAYLOAD_MASK	= 0x0E,
	NRF24L01_07_PAYLOAD_EPY_bm	= 0x0E,
	NRF24L01_07_PAYLOAD_P5_bm	= 0x0A,
	NRF24L01_07_PAYLOAD_P4_bm	= 0x08,
	NRF24L01_07_PAYLOAD_P3_bm	= 0x06,
	NRF24L01_07_PAYLOAD_P2_bm	= 0x04,
	NRF24L01_07_PAYLOAD_P1_bm	= 0x02,
	NRF24L01_07_PAYLOAD_P0_bm	= 0x00,
	NRF24L01_07_TX_FULL_bm		= 0x01
};
#define NRF24L01_08_OBSERVE_TX		0x08		// Transmit observe register
enum {
	RF24L01_08_PLOS_CNT_MASK	= 0xF0,
	RF24L01_08_ARC_CNT_MASK		= 0x0F
};
#define NRF24L01_09_CD			0x09		// Carrier Detect
enum {
	RF24L01_09_CD_RPD_bm		= 0x01
};
#define NRF24L01_0A_RX_ADDR_P0		0x0A		// Receive address data pipe 0.
#define NRF24L01_0B_RX_ADDR_P1		0x0B		// Receive address data pipe 1.
#define NRF24L01_0C_RX_ADDR_P2		0x0C		// Receive address data pipe 2. Only LSB.
#define NRF24L01_0D_RX_ADDR_P3		0x0D		// Receive address data pipe 3. Only LSB.
#define NRF24L01_0E_RX_ADDR_P4		0x0E		// Receive address data pipe 4. Only LSB.
#define NRF24L01_0F_RX_ADDR_P5		0x0F		// Receive address data pipe 5. Only LSB.
#define NRF24L01_10_TX_ADDR		0x10		// Transmit address.
#define NRF24L01_11_RX_PW_P0		0x11
#define NRF24L01_12_RX_PW_P1		0x12
#define NRF24L01_13_RX_PW_P2		0x13
#define NRF24L01_14_RX_PW_P3		0x14
#define NRF24L01_15_RX_PW_P4		0x15
#define NRF24L01_16_RX_PW_P5		0x16
#define NRF24L01_17_FIFO_STATUS		0x17		// FIFO Status Register
enum {
	NRF24L01_17_TX_REUSE_bm		= 0x40,
	NRF24L01_17_FIFO_FULL_bm	= 0x20,		// TX FIFO full flag
	NRF24L01_17_TX_EMPTY_bm		= 0x10,		// TX FIFO empty flag
	NRF24L01_17_RX_FULL_bm		= 0x02,		// RX FIFO full flag
	NRF24L01_17_RX_EMPTY_bm		= 0x01		// RX FIFO empty flag
};
#define NRF24L01_1C_DYNPD		0x1C		// Enable dynamic payload length
enum {
	NRF24L01_1C_DYNPD_MASK		= 0x3F,
	NRF24L01_1C_DYNPD_P5_bm		= 0x20,
	NRF24L01_1C_DYNPD_P4_bm		= 0x10,
	NRF24L01_1C_DYNPD_P3_bm		= 0x08,
	NRF24L01_1C_DYNPD_P2_bm		= 0x04,
	NRF24L01_1C_DYNPD_P1_bm		= 0x02,
	NRF24L01_1C_DYNPD_P0_bm		= 0x01
};
#define NRF24L01_1D_FEATURE		0x1D		// Feature Register
enum {
	NRF2401_1D_EN_DPL_bm		= 0x04,		// Enables Dynamic Payload Length
	NRF2401_1D_EN_ACK_PAY_bm	= 0x02,		// Enables Payload with ACK
	NRF2401_1D_EN_DYN_ACK_bm	= 0x01		// Enables the W_TX_PAYLOAD_NOACK command
};

// Instructions
enum {
	NRF24L01_REGISTER_MASK		= 0x1F,
	NRF24L01_00_R_REGISTER		= 0x00,		// last 4 bits will indicate reg. address
	NRF24L01_20_W_REGISTER		= 0x20,		// last 4 bits will indicate reg. address
	NRF24L01_50_ACTIVATE		= 0x50,		// the ACTIVATE command will be ignored on the nRF24L01+
	NRF24L01_60_R_RX_PL_WID		= 0x60,		// Read RX payload width for the top R_RX_PAYLOAD in the RX FIFO
	NRF24L01_61_RX_PAYLOAD		= 0x61,		// Read RX-payload
	NRF24L01_A0_TX_PAYLOAD		= 0xA0,		// Write TX-payload
	NRF24L01_A8_W_ACK_PAYLOAD0	= 0xA8,			
	NRF24L01_A9_W_ACK_PAYLOAD1	= 0xA9,
	NRF24L01_AA_W_ACK_PAYLOAD2	= 0xAA,
	NRF24L01_AB_W_ACK_PAYLOAD3	= 0xAB,
	NRF24L01_AC_W_ACK_PAYLOAD4	= 0xAC,
	NRF24L01_AD_W_ACK_PAYLOAD5	= 0xAD,
	NRF24L01_B0_TX_PYLD_NOACK	= 0xB0,		// Used in TX mode. Disables AUTOACK on this specific packet.
	NRF24L01_E1_FLUSH_TX		= 0xE1,		// Flush TX FIFO
	NRF24L01_E2_FLUSH_RX		= 0xE2,		// Flush RX FIFO
	NRF24L01_E3_REUSE_TX_PL		= 0xE3,		// Reuse last transmitted payload.
	NRF24L01_FF_NOP			= 0xFF		// No Operation
};



#endif /* NRF24L01_DEF_H_ */
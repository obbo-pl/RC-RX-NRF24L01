/*
 * terminal.h
 *
 * Created: 27.08.2019 13:53:31
 * Atmel Studio 6 (Version: 6.2.1563 - Service Pack 2)
 * AVR/GNU C Compiler : 4.8.1
 *  Author: Krzysztof Markiewicz
 *  www.obbo.pl
 *
 * This program is distributed under the terms of the GNU General Public License
 */ 


#ifndef TERMINAL_H_
#define TERMINAL_H_

#include <stdbool.h>
#include <avr/io.h>
#include "cbuffer.h"


#define TERMINAL_NEW_LINE		(0x0D)
#define TERMINAL_NEW_COMMAND		(0x40)
#define TERMINAL_HELP			(0x3F)
#define TERMINAL_SPACE			(0x20)

#define TERMINAL_INPUT_BUFFER_LENGTH	(64)
#define TERMINAL_OPTION_LENGTH		(32)

#define COMMAND_LENGTH			(4)


typedef struct {
	volatile char input_buffer[TERMINAL_INPUT_BUFFER_LENGTH];
	char command_option[TERMINAL_OPTION_LENGTH];
	uint8_t input_buffer_position;
	bool input_is_empty;
	bool input_is_full;
	bool change_to_write;
	CircularBuffer_t *output_buffer;
} DCTERMINAL_t;


typedef struct {
	const char *pattern;
	void (*callback)(DCTERMINAL_t *);
} TERMINAL_COMMAND_t;



void terminal_Init(DCTERMINAL_t *terminal, CircularBuffer_t *cbuffer);
void terminal_BindCommands(TERMINAL_COMMAND_t *commands, uint8_t length);

bool terminal_FindNewLine(DCTERMINAL_t *terminal, uint8_t *len);
void terminal_ParseCommand(DCTERMINAL_t *terminal, uint8_t len);
void terminal_InterruptHandler(DCTERMINAL_t *terminal);
void terminal_DropInputBuffer(DCTERMINAL_t *terminal);
void terminal_FlushOutBuffer(CircularBuffer_t *cbuffer);
void terminal_SendOutBuffer(CircularBuffer_t *cbuffer);
void terminal_SendBadArgument(CircularBuffer_t *output);
void terminal_SendOK(CircularBuffer_t *cbuffer);

void terminal_Print(CircularBuffer_t *cbuffer, const char *src, bool flush=false);
void terminal_PrintEE(CircularBuffer_t *cbuffer, const char *src, bool flush=false);
void terminal_PrintPM(CircularBuffer_t *cbuffer, const char *src, bool flush);
void terminal_PrintLn(CircularBuffer_t *cbuffer, const char *src, bool flush=false);
void terminal_PrintInt(CircularBuffer_t *cbuffer, int val, bool flush=false);
void terminal_PrintULong(CircularBuffer_t *cbuffer, uint32_t val, bool flush=false);
void terminal_PrintHex(CircularBuffer_t *cbuffer, uint8_t val, bool flush=false);
void terminal_PrintBin(CircularBuffer_t *cbuffer, uint8_t val, bool flush=false);
void terminal_PrintHexAndBin(CircularBuffer_t *cbuffer, uint8_t val, bool flush=false);
void terminal_PrintBuf(CircularBuffer_t *cbuffer, const uint8_t *buffer, uint8_t len, bool flush=false);
void terminal_SendNL(CircularBuffer_t *output, bool flush=false);




#endif /* TERMINAL_H_ */
/*
 * usart_terminal.c
 *
 * Created: 27.08.2019 13:54:28
 * Atmel Studio 6 (Version: 6.2.1563 - Service Pack 2)
 * AVR/GNU C Compiler : 4.8.1
 *  Author: Krzysztof Markiewicz
 *  www.obbo.pl
 *
 * This program is distributed under the terms of the GNU General Public License
 */ 

#include <stdlib.h>
#include <stdio.h>
#include <util/delay.h>
#include <avr/eeprom.h>
#include "terminal.h"
#include "usart.h"


uint8_t terminal_commands_array_size = 0;
TERMINAL_COMMAND_t *terminal_commands_array;


void terminal_SendHelp(CircularBuffer_t *cbuffer);
void terminal_SendUnknown(CircularBuffer_t *cbuffer);
void terminal_ClearInputBuffer(DCTERMINAL_t *terminal);
void terminal_ClearCommandOptionBuffer(char *buffer);
void terminal_ShrinkBuffer(DCTERMINAL_t *terminal, uint8_t len);
bool terminal_FindCommand(DCTERMINAL_t *terminal, uint8_t len);


const char EEMEM terminal_message_001[] = "? unknown command\n\r";
const char EEMEM terminal_message_002[] = "? bad argument\n\r";
const char EEMEM terminal_message_003[] = "OK\n\r";



void terminal_Init(DCTERMINAL_t *terminal, CircularBuffer_t *cbuffer)
{
	terminal->output_buffer = cbuffer;
}

void terminal_BindCommands(TERMINAL_COMMAND_t *commands, uint8_t length)
{
	terminal_commands_array = commands;
	terminal_commands_array_size = length;
}

void terminal_InterruptHandler(DCTERMINAL_t *terminal)
{
	char new_char;
	if (terminal->input_buffer_position < TERMINAL_INPUT_BUFFER_LENGTH) {
		new_char = usart_ReadChar();
		if ((new_char > 0x20) && (new_char < 0x7F)) {
			terminal->input_buffer[terminal->input_buffer_position] = new_char;
			terminal->input_buffer_position++;
			cbuffer_AppendChar(terminal->output_buffer, new_char);
			} else if (new_char == 0x20) {
			if (terminal->input_buffer_position > 0) {
				terminal->input_buffer[terminal->input_buffer_position] = new_char;
				terminal->input_buffer_position++;
			}
			cbuffer_AppendChar(terminal->output_buffer, new_char);
			} else if (new_char == 0x08) {
			if (terminal->input_buffer_position > 0) {
				terminal->input_buffer[terminal->input_buffer_position] = 0x00;
				terminal->input_buffer_position--;
			}
			cbuffer_AppendChar(terminal->output_buffer, new_char);
			} else if ((new_char == 0x0A) || (new_char == 0x0D)) {
			if (!(terminal->input_is_empty)) {
				terminal->input_buffer[terminal->input_buffer_position] = TERMINAL_NEW_LINE;
				terminal->input_buffer_position++;
			}
			terminal_SendNL(terminal->output_buffer, false);
		}
		if (terminal->input_buffer_position > 0) terminal->input_is_empty = false;
	}
	if (terminal->input_buffer_position >= TERMINAL_INPUT_BUFFER_LENGTH) terminal->input_is_full = true;
}

void terminal_ParseCommand(DCTERMINAL_t *terminal, uint8_t len)
{
	terminal_FindCommand(terminal, len);
	terminal_ShrinkBuffer(terminal, len);
	if (terminal->input_buffer_position == 0) terminal_ClearInputBuffer(terminal);
}

bool terminal_FindNewLine(DCTERMINAL_t *terminal, uint8_t *len)
{
	for (int i = 0; i < TERMINAL_INPUT_BUFFER_LENGTH; i++) {
		if (terminal->input_buffer[i] == TERMINAL_NEW_LINE) {
			*len = i + 1;
			return true;
		}
	}
	return false;
}

void terminal_SendHelp(CircularBuffer_t *cbuffer)
{
	for (int i = 0; i < terminal_commands_array_size; i++) {
		cbuffer_AppendString(cbuffer, terminal_commands_array[i].pattern);
		terminal_SendNL(cbuffer, false);
	}
	terminal_SendNL(cbuffer, false);
}

void terminal_SendUnknown(CircularBuffer_t *cbuffer)
{
	cbuffer_AppendEEString(cbuffer, terminal_message_001);
}

void terminal_SendOK(CircularBuffer_t *cbuffer)
{
	cbuffer_AppendEEString(cbuffer, terminal_message_003);
}

void terminal_SendBadArgument(CircularBuffer_t *cbuffer)
{
	cbuffer_AppendEEString(cbuffer, terminal_message_002);
}

bool terminal_FindCommand(DCTERMINAL_t *terminal, uint8_t len)
{
	bool result = false;
	if ((len == 2) && (terminal->input_buffer[0] == '?')) {
		terminal_SendHelp(terminal->output_buffer);
		result = true;
	}
	if (terminal->input_buffer[0] == TERMINAL_NEW_COMMAND) {
		for (int i = 0; i < terminal_commands_array_size; i++) {
			const char *patern;
			patern = terminal_commands_array[i].pattern;
			bool match = true;
			for (int j = 1; j < COMMAND_LENGTH; j++) {
				if (terminal->input_buffer[j] != patern[j]) match = false;
			}
			if (!((terminal->input_buffer[COMMAND_LENGTH] == TERMINAL_HELP)
			|| (terminal->input_buffer[COMMAND_LENGTH] == TERMINAL_NEW_LINE)
			|| (terminal->input_buffer[COMMAND_LENGTH] == TERMINAL_SPACE))) match = false;
			if (match) {
				for (int j = 0; j < (len - COMMAND_LENGTH); j++) {
					if (j >= (TERMINAL_OPTION_LENGTH - 1)) break;
					terminal->command_option[j] = terminal->input_buffer[j + COMMAND_LENGTH];
				}
				terminal_commands_array[i].callback(terminal);
				terminal_ClearCommandOptionBuffer(terminal->command_option);
				result = true;
			}
		}
	}
	if (!(result)) terminal_SendUnknown(terminal->output_buffer);
	return result;
}

void terminal_DropInputBuffer(DCTERMINAL_t *terminal)
{
	terminal_ClearInputBuffer(terminal);
	terminal_SendUnknown(terminal->output_buffer);
}

void terminal_ClearInputBuffer(DCTERMINAL_t *terminal)
{
	for (int i = 0; i < TERMINAL_INPUT_BUFFER_LENGTH; i++) {
		terminal->input_buffer[i] = 0x00;
	}
	terminal->input_buffer_position = 0;
	terminal->input_is_empty = true;
	terminal->input_is_full = false;
}

void terminal_ShrinkBuffer(DCTERMINAL_t *terminal, uint8_t len)
{
	terminal->input_buffer[len - 1] = 0x00;
	for (int i = len; i < TERMINAL_INPUT_BUFFER_LENGTH; i++) {
		terminal->input_buffer[i - len] = terminal->input_buffer[i];
		terminal->input_buffer[i] = 0x00;
	}
	terminal->input_buffer_position -= len;
}

void terminal_ClearCommandOptionBuffer(char *buffer)
{
	for (int i = 0; i < TERMINAL_OPTION_LENGTH; i++) {
		buffer[i] = 0x00;
	}
}

void terminal_SendOutBuffer(CircularBuffer_t *cbuffer)
{
	bool send_status = true;
	while (send_status && !(cbuffer_IsEmpty(cbuffer))) {
		send_status = usart_SendChar(cbuffer_ReadChar(cbuffer));
		if (send_status) cbuffer_DropChar(cbuffer);
	}
}

void terminal_FlushOutBuffer(CircularBuffer_t *cbuffer)
{
	bool send_status = true;
	while (send_status && !(cbuffer_IsEmpty(cbuffer))) {
		send_status = usart_SendChar(cbuffer_ReadChar(cbuffer));
		if (send_status) cbuffer_DropChar(cbuffer);
		_delay_ms(1);
	}
}

void terminal_Print(CircularBuffer_t *cbuffer, const char *src, bool flush)
{
	cbuffer_AppendString(cbuffer, src);
	if (flush) terminal_FlushOutBuffer(cbuffer);
}

void terminal_PrintEE(CircularBuffer_t *cbuffer, const char *src, bool flush)
{
	cbuffer_AppendEEString(cbuffer, src);
	if (flush) terminal_FlushOutBuffer(cbuffer);
}

void terminal_PrintPM(CircularBuffer_t *cbuffer, const char *src, bool flush)
{
	cbuffer_AppendPMString(cbuffer, src);
	if (flush) terminal_FlushOutBuffer(cbuffer);
}

void terminal_PrintLn(CircularBuffer_t *cbuffer, const char *src, bool flush)
{
	terminal_Print(cbuffer, src, flush);
	terminal_SendNL(cbuffer, flush);
	if (flush) terminal_FlushOutBuffer(cbuffer);
}

void terminal_PrintInt(CircularBuffer_t *cbuffer, int val, bool flush)
{
	char buff[7];
	itoa(val, buff, 10);
	cbuffer_AppendString(cbuffer, buff);
	if (flush) terminal_FlushOutBuffer(cbuffer);
}

void terminal_PrintULong(CircularBuffer_t *cbuffer, uint32_t val, bool flush)
{
	char buff[12];
	ultoa(val, buff, 10);
	cbuffer_AppendString(cbuffer, buff);
	if (flush) terminal_FlushOutBuffer(cbuffer);
}

void terminal_PrintHex(CircularBuffer_t *cbuffer, uint8_t val, bool flush)
{
	char buff[3];
	char *start = buff;
	if (val < 0x10) *start++ = 0x30;
	itoa(val, start, 16);
	cbuffer_AppendString(cbuffer, buff);
	if (flush) terminal_FlushOutBuffer(cbuffer);
}

void terminal_PrintBin(CircularBuffer_t *cbuffer, uint8_t val, bool flush)
{
	char buff[10];
	int i = 0x80;
	while (i > val)  {
		i = i >> 1;
		cbuffer_AppendString(cbuffer, "0");
	}
	if (val > 0) {
		itoa(val, buff, 2);
		cbuffer_AppendString(cbuffer, buff);
	}
	if (flush) terminal_FlushOutBuffer(cbuffer);
}

void terminal_PrintHexAndBin(CircularBuffer_t *cbuffer, uint8_t val, bool flush)
{
	cbuffer_AppendString(cbuffer, "0x");
	if (flush) terminal_FlushOutBuffer(cbuffer);
	terminal_PrintHex(cbuffer, val, flush);
	cbuffer_AppendString(cbuffer, "  ");
	if (flush) terminal_FlushOutBuffer(cbuffer);
	terminal_PrintBin(cbuffer, val, flush);
	terminal_SendNL(cbuffer, flush);
}

void terminal_PrintBuf(CircularBuffer_t *cbuffer, const uint8_t *buffer, uint8_t len, bool flush)
{
	while (len--) {
		terminal_PrintHex(cbuffer, buffer[len], flush);
	}
	if (flush) terminal_FlushOutBuffer(cbuffer);
}

void terminal_SendNL(CircularBuffer_t *cbuffer, bool flush)
{
	cbuffer_AppendString(cbuffer, "\n\r");
	if (flush) terminal_FlushOutBuffer(cbuffer);
}



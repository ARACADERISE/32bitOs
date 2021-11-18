#ifndef STDIO
#define STDIO
#include "../util.h"
#include "string.h"

/*
 * PutC:
 * 	Print a character to the string, then return the given character for possible reference.
 * */
uint8_t PutC(uint8_t Pchar, uint32_t fg, uint32_t bg)
{
	remove_cursor();
	//update_screen();

	if(Pchar == '\n')
	{
		//remove_cursor();
		cx = 0;
		cy++;
		update_screen();
		//update_cursor();
		//Update(0, cy+1);
		return '\0';
	}
				
	uint32_t *FrameBuffer = (uint32_t*) FBUF;
	FrameBuffer += GetCoords();
	
	TextFont = (uint8_t *)(TMEM + ((Pchar * 16) - 16));
	for(uint8_t l = 0; l < 16; l++)
	{
		for(int8_t b = 7; b >= 0; b--)
		{
			*FrameBuffer = (TextFont[l] & (1 << b)) ? fg : bg;
			FrameBuffer++;
		}
		FrameBuffer += (WIDTH - 8);
	}
	FrameBuffer -= (WIDTH - 8);
	cx++;
	update_screen();
	//update_cursor();

	return Pchar;
}
void Print(uint8_t *string, uint32_t fgcolor, uint32_t bgcolor, uint8_t end);

void PrintHex(uint32_t number, uint32_t fgcolor, uint32_t bgcolor, uint8_t append_zero, uint8_t newline)
{
	uint8_t hex_string[80];
	uint8_t numbers[17] = "0123456789ABCDEF";
	uint8_t i = 0, j = 0;

	while(number > 0)
	{
		hex_string[i] = numbers[(uint8_t)number & 0x0F];
		number>>=4;
		i++;
	}
	
	hex_string[i++] = 'x';
	hex_string[i++] = '0';
	hex_string[i] = '\0';
	i--;

	for(j = 0; j < i; j++,i--)
	{
		uint8_t temp = hex_string[j];
		hex_string[j] = hex_string[i];
		hex_string[i] = temp;
	}

	if(j < 4 && append_zero==0)
	{
		hex_string[j+2] = '0';
		hex_string[j+3] = '0';
		hex_string[j+4] = '\0';
	}

	Print((uint8_t *)hex_string, fgcolor, bgcolor, newline);
}

/*
 * Print:
 * 	Print sequence of characters to screen.
 * */
void Print(uint8_t *string, uint32_t fgcolor, uint32_t bgcolor, uint8_t end)
{
	uint32_t *ss, *ss2;
	while(!(*string == '\0'))
	{
		switch(*string)
		{
			case '\n':
			{
				if(CheckScreen() == 0)	
				{
					/*remove_cursor();
					cx = 0;
					cy++;
					update_cursor();*/
					//for(uint16_t i = 0; i < WIDTH-cx; i++)
					//	PutC(' ', MakeColor(0, 0, 0), MakeColor(0, 0, 0));
					//remove_cursor();
					cx = 0;
					cy++;
					update_cursor();
				}
				
				string++;
				break;
			}
			case '\t':
			{
				for(uint8_t i = 0; i < 4; i++)
					PutC(' ', MakeColor(0, 0, 0), MakeColor(0, 0, 0));

				string++;
				break;
			}
			default:
			{
				PutC(*string, fgcolor, bgcolor);
				string++;
			}
		}
	}

	if(CheckScreen() == 0 && end==0)
	{
		remove_cursor();
		cx = 0;
		cy++;
		update_screen();
	}
}

/*
 * getc - Get character input.
 *
 * Args:
 * 	valid - Are we capable of doing any action at current position?
 * 		I.E: If we are at `>`, we can't backspace anymore.
 */
uint8_t getc(uint8_t valid)
{
	uint8_t scancode	= 0;
	uint8_t char_value	= 0;
	uint8_t tb		= 0;
	uint8_t old_key		= 0;

	while(1)
	{
		//tb = inb(0x64);
		__asm__ __volatile__ ("inb $0x64, %%al" : "=a"(tb));
		if(tb & 1) break;
	}
	
	//scancode = inb(0x60);
	__asm__ __volatile__ ("inb $0x60, %%al" : "=a"(scancode));


	if(scancode==0x1C)
		return '0';
	if(scancode==0x0E && valid)
	{
		cx--;
		PutC('\0', MakeColor(0, 0, 0), MakeColor(0, 0, 0));
		remove_cursor();
		cx--;
		update_screen();
		return '\4';
	} else if(scancode == 0x0E && !valid)
		return '\3';

	char_value = scancodes[scancode].val;

	*(uint8_t *)0x1600 = char_value;

	return *(uint8_t*)0x1600;
}


void PrintTable()
{
	Print((uint8_t *)"", MakeColor(255, 255, 255), MakeColor(0, 0, 0), 0);
	setCoords(10, cy+1);
	uint8_t *file_manager_ptr = (uint8_t *)FileManager;
	uint8_t *file_manager_addr = (uint8_t *)FileManager;
	file_manager_addr += 0x31;
	uint16_t hex;
	
	Print((uint8_t *)
		"FILE\t\t\tSECTOR #\t\tADDRESS",
		MakeColor(255, 255, 255), MakeColor(0, 0, 0), 1
	);
	setCoords(10, cy+1);
	Print(
		(uint8_t*)"====\t\t\t========\t\t============",
		MakeColor(255, 255, 255), MakeColor(0, 0, 0), 0		
	);
	uint8_t ind = 0;

	while(*file_manager_ptr != 0)
	{
		setCoords(10, cy);

		for(uint8_t i = 0; i < 11; i++)
		{		
			PutC(*file_manager_ptr, MakeColor(255, 255, 255), MakeColor(0, 0, 0));
			file_manager_ptr++;
		}
		for(uint8_t i = 0; i < 5; i++)
			PutC(' ', MakeColor(255, 255, 255), MakeColor(0, 0, 0));
		hex = (uint16_t)*file_manager_ptr++;
		PrintHex(hex, MakeColor(255, 255, 255), MakeColor(0, 0, 0), 1, 1);

		for(uint8_t i = 0; i < 13; i++)
			PutC(' ', MakeColor(255, 255, 255), MakeColor(0, 0, 0));
		PrintHex(*(file_manager_addr+ind), MakeColor(255, 255, 255), MakeColor(0, 0, 0), 0, 0);
		ind+=2;
	}
	
	Print((uint8_t *)"", MakeColor(255, 255, 255), MakeColor(0, 0, 0), 0);
	//Update(0, cy+1);
}


#endif

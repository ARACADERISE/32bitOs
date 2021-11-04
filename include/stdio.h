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
	uint32_t *FrameBuffer = (uint32_t *)*(uint32_t *) FBUF;
	FrameBuffer += GetCoords();
	
	TextFont = (uint8_t *)(TMEM + ((Pchar * 16) - 16));
	for(uint8_t l = 0; l < 16; l++)
	{
		for(int8_t b = 8; b >= 0; b--)
		{
			*FrameBuffer = (TextFont[l] & (1 << b)) ? fg : bg;
			FrameBuffer++;
		}
		FrameBuffer += (WIDTH - 9);
	}

	cx++;
	FrameBuffer -= (WIDTH - 9);
	update_cursor();

	return Pchar;
}


/*
 * Print:
 * 	Print sequence of characters to screen.
 * */
void Print(uint8_t *string, uint32_t fgcolor, uint32_t bgcolor)
{
	remove_cursor();
	uint32_t *ss, *ss2;
	while(!(*string == '\0'))
	{
		switch(*string)
		{
			case '\n':
			{
				if(CheckScreen() == 0)	
					Update(0, cy+1);
				string++;
				break;
			}
			case '\t':
			{
				for(uint8_t i = 0; i < 4; i++)
				{
					PutC(' ', MakeColor(0, 0, 0), MakeColor(0, 0, 0));
					update_cursor();
				}
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

	if(CheckScreen() == 0)
		Update(0, cy+1);
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

	while(1)
	{
		tb = inb(0x64);
		if(tb & 1) break;
	}
	
	scancode = inb(0x60);

	if(scancode==0x1C)
		return '0';
	if(scancode==0x0E && valid)
	{
		cx--;
		PutC('\0', MakeColor(0, 0, 0), MakeColor(0, 0, 0));
		remove_cursor();
		cx--;
		Update(cx, cy);
		return '\4';
	} else if(scancode == 0x0E && !(valid))
		return '\3';
	
	char_value = scancodes[scancode].val;

	return char_value;
}

#endif

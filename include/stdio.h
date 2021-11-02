#ifndef STDIO
#define STDIO
#include "../util.h"
#include "string.h"

/*
 * CPutC - Colored Text
 *
 * Put characters in colored form.
 * */
//void CPutC(uint8_t c, uint16_t colored_attrib)
//{
//	*(AVidMem + cursor_pos) = c | (colored_attrib << 8);
//	update_cursor(cursor_pos+1);
//}

/*
 * PutC - Display general text.
 *
 * ------------		NOT USED	------------
 * */
//void PutC(uint8_t c)
//{
//	*(VidMem + cursor_pos * 2) = c;
//	update_cursor(cursor_pos+1);
//}

/*
 * Print - Print multiple characters to screen in string form.
 * 
 * Print in colored form via AVidMem.
 *
 * Args:
 * 	*str - String to print
 * 	colored_attrib - Value returned from make_colored for the foreground/background color
 * 	end - 0 for newline, 1 for nothing.
 * */
//void Print(const uint8_t *str, uint16_t colored_attrib, uint8_t end)
//{
//	uint8_t *ptr = (uint8_t *)str;
//	uint16_t curr = 0;
//
//	while(*ptr != 0)
//	{
//		switch(*ptr)
//		{
//			case 0xA:
//			{
//				cursor_pos+=WIDTH-curr;
//				cy++;
//				curr=0;
//				break;
//			}
//			case 0xD:
//			{
//				cursor_pos -= cursor_pos % WIDTH;
//				break;
//			}
//			case 0x9:
//			{
//				cursor_pos += 2;
//				curr+=2;
//				break;
//			}
//			default:
//			{
//				CPutC(*ptr, colored_attrib);
//				curr++;
//				break;
//			}
//		}
//
//		ptr++;
//	}
//
//	if(end == 0)
//	{
//		cursor_pos+=WIDTH-strlen((const uint8_t *)str);
//		cy++;
//		update_cursor(cursor_pos);
//	}
//	update_cursor(cursor_pos);
//}

/*
 * Print - Print multiple charactes to screen in string form.
 *
 * ------------		NOT USED	------------
 * */
//void _Print(const int8_t *str)
//{
//	uint8_t *ptr = (uint8_t *)str;
//	uint16_t index = cursor_pos;
//	while(*ptr != 0)
//	{
//		//PutC(*str, index);
//		*(VidMem + index * 2) = *ptr;
//		//*VidMem+=15;
//		index++;
//		ptr++;
//	}
//
//	index+=WIDTH-strlen((const uint8_t*)str);
//	cy++;
///	update_cursor(index);
//}

/*
 * getc - Get character input.
 */
uint8_t getc(void)
{
	uint8_t scancode	= 0;
	uint8_t char_value	= 0;
	uint8_t tb		= 0;
	uint8_t old		= 0;

	while(1)
	{
		tb = inb(0x64);
		if(tb & 1) break;
	}
	
	scancode = inb(0x60);

	if(scancode==0x1C)
	{
		cy++;
		update_cursor();
		return '0';
	}
	
	char_value = scancodes[scancode].val;

	return char_value;
}

#endif

#include "include/stdint.h"

#define WIDTH	80
#define HEIGHT	25

typedef enum
{
	black,
	blue,
	green,
	cyan,
	red,
	mag,
	brown,
	l_gray,
	d_gray,
	l_blue,
	l_green = 0xA,
	l_cyan = 0xB,
	l_red = 0xC,
	l_mag = 0xD,
	yellow = 0xE,
	white = 0xF
} Colors;

// Regular Text.
volatile uint8_t *VidMem = (volatile uint8_t *)0xB8000;

// Colored Text.
volatile uint16_t *AVidMem = (volatile uint16_t *)0xB8000;

// Text Font
volatile uint8_t *TextFont = (volatile uint8_t *)0x2000;

void reset_AVidMem()
{
	AVidMem = (volatile uint16_t *)0xB8000;
}
void reset_VidMem()
{
	VidMem = (volatile uint8_t *)0xB8000;
}

uint8_t inb(uint16_t port)
{
	uint8_t rv;
	__asm__ __volatile__ ("inb %1, %0" : "=a" (rv) : "dN" (port));
	return rv;
}

void outb(uint16_t port, uint8_t data)
{
	__asm__ __volatile__ ("outb %1, %0" : : "dN" (port), "a" (data));
}

uint16_t make_colored(Colors fore, Colors back)
{
	return (uint16_t) (back << 4) | (fore & 0x0F);
}

/*
 * Shortcuts for colored printing.
 * */
#define WOB	make_colored(white,black)
#define BOB	make_colored(blue, black)
#define GOB	make_colored(green, black)
#define COB	make_colored(cyan, black)
#define ROB	make_colored(red, black)
#define MOB	make_colored(mag, black)
#define BROB	make_colored(brown, black)
#define LBOB	make_colored(l_blue, black)
#define LGOB	make_colored(l_green, black)
#define LCOB	make_colored(l_cyan, black)
#define LMOB	make_colored(l_mag, black)
#define YOB	make_colored(yellow, black)
#define LGROB	make_colored(l_gray, black)

uint16_t Coords(uint8_t x, uint8_t y)
{
	return y * WIDTH + x;
}

static uint16_t cursor_pos;

/*
 * CPutC - Colored Text
 *
 * Put characters in colored form.
 * */
void CPutC(uint8_t c, uint16_t colored_attrib, uint16_t pos)
{
	*(AVidMem + pos) = c | (colored_attrib << 8);
}

/*
 * PutC - Display general text.
 *
 * ------------		NOT USED	------------
 * */
void PutC(uint8_t c, uint16_t index)
{
	*(VidMem + index * 2) = c;
}

void update_cursor(uint16_t pos)
{	
	if(pos > 2000) pos = 2000;
	if(pos < 0) pos = 0;
	
	outb(0x3D4, 0x0F);
	outb(0x3D5, (uint8_t) pos & 0xFF);
	outb(0x3D4, 0x0E);
	outb(0x3D5, (uint8_t) (pos >> 8) & 0xFF);

	cursor_pos = pos;
}

uint32_t strlen(const uint8_t *string)
{
	uint32_t length = 0;
	
	while(string[length] != 0)
		length++;

	return length;
}

/*
 * Print - Print multiple charactes to screen in string form.
 *
 * ------------		NOT USED	------------
 * */
void _Print(const int8_t *str)
{
	uint8_t *ptr = (uint8_t *)str;
	uint16_t index = cursor_pos;
	while(*ptr != 0)
	{
		//PutC(*str, index);
		*(VidMem + index * 2) = *ptr;
		//*VidMem+=15;
		index++;
		ptr++;
	}

	index+=WIDTH-strlen((const uint8_t*)str);
	update_cursor(index);
}

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
void Print(const int8_t *str, uint16_t colored_attrib, uint8_t end)
{
	uint8_t *ptr = (uint8_t *)str;
	uint16_t index = cursor_pos;
	uint16_t curr = 0;

	while(*ptr != 0)
	{
		switch(*ptr)
		{
			case 0xA:
			{
				index+=WIDTH-curr;
				curr=0;
				break;
			}
			case 0xD:
			{
				index -= index % WIDTH;
				//curr=0;
				break;
			}
			case 0x9:
			{
				index += 2;
				curr+=2;
				break;
			}
			default:
			{
				CPutC(*ptr, colored_attrib, index);
				index++;
				curr++;
				break;
			}
		}

		ptr++;
	}

	if(end == 0)
		index+=WIDTH-strlen((const uint8_t *)str);
	update_cursor(index);
}

void init_cursor()
{
	outb(0x3D4, 0x0A);
	outb(0x3D5, (inb(0x3D5) & 0xC0) | 0);
	outb(0x3D4, 0x0B);
	outb(0x3D5, (inb(0x3D5) & 0xE0) | 25);	
}


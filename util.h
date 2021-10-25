#include "include/stdint.h"

#define WIDTH	80
#define HEIGHT	25

typedef struct
{
	uint16_t x;
	uint16_t y;
} TerminalCursor;

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

static TerminalCursor tc;

// Regular Text.
uint8_t *VidMem = (uint8_t *)0xB8000;
//#define VidMem	(uint8_t *)0xB8000
// Colored Text.
volatile uint16_t *AVidMem = (volatile uint16_t *)0xB8000;
static uint16_t offset;

void reset_AVidMem()
{
	*AVidMem = *(volatile uint16_t *)0xB8000;
}
void reset_VidMem()
{
	VidMem = (uint8_t *)0xB8000;
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
void CPutC(uint8_t c, uint16_t colored_attrib)
{
	*AVidMem = c | (colored_attrib << 8);
	*AVidMem++;
	tc.x++;
}

/*
 * PutC - Display general text.
 * */
void PutC(uint8_t c, uint16_t index)
{
	//*(VidMem + index * 2) = c;
}

void update_cursor(uint16_t pos);

/*
 * Print - Print multiple charactes to screen in string form.
 * */
void Print(const uint8_t *str)
{
	uint8_t *ptr = (uint8_t *)str;
	uint16_t index = cursor_pos;
	while(*ptr != 0)
	{
		//PutC(*str, index);
		*(VidMem + index * 2) = *ptr;
		index++;
		ptr++;
	}

	update_cursor(index);
}

uint32_t strlen(uint8_t *string)
{
	uint32_t length = 0;
	//uint32_t index = 0;
	
	while(string[length] != 0)
		length++;

	return length;
}

void init_cursor()
{
	tc.x = 0;
	tc.y = 0;

	outb(0x3D4, 0x0A);
	outb(0x3D5, (inb(0x3D5) & 0xC0) | 0);
	outb(0x3D4, 0x0B);
	outb(0x3D5, (inb(0x3D5) & 0xE0) | 25);	
}

void update_cursor(uint16_t pos)
{
	//offset = 1 * tc.x;
	
	if(pos > 2000) pos = 2000;
	if(pos < 0) pos = 0;

	//outb(0x3D4, 14);
	//outb(0x3D5, offset >> 8);
	//outb(0x3D4, 15);
	//outb(0x3D5, offset);
	
	outb(0x3D4, 0x0F);
	outb(0x3D5, (uint8_t) pos & 0xFF);
	outb(0x3D4, 0x0E);
	outb(0x3D5, (uint8_t) (pos >> 8) & 0xFF);

	cursor_pos = pos;
	//*AVidMem = *VidMem;

}

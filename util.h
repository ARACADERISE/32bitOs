#ifndef UTIL
#define UTIL

#include "include/stdint.h"

#define WIDTH	1024//800
#define HEIGHT	768//600

// Make RGB color for pixels.
#define MakeColor(R, G, B) R*65536 + G*256 + B

// Drive number
volatile uint8_t *DriveNum = (volatile uint8_t *)0x1500;

// Text Font
volatile uint8_t *TextFont = (volatile uint8_t *)0x2000;

// Current cursor position
static uint16_t cursor_pos;

// cx, cy - Cursor x, Cursor y. Used for accurate cursor positions.
static uint16_t cx, cy;

// 1 extra bit in between.
#define GetCoords()	(cx * 9) + (cy * 16 * WIDTH)

// Memory address of FrameBuffer.
#define FBUF	0x4028

// Memory address of our custom font.
#define TMEM	0x2000

// Keyboard scancodes.
enum Scancodes
{
	esc=	0x01,
	n_1=	0x02,
	n_2=	0x03,
	n_3=	0x04,
	n_4=	0x05,
	n_5=	0x06,
	n_6=	0x07,
	n_7=	0x08,
	n_8=	0x09,
	n_9=	0x0A,
	n_0=	0x0B,
	minus=	0x0C,
	equals=	0x0D,
	back=	0x0E,
	tab=	0x0F,
	q=	0x10,
	w=	0x11,
	e=	0x12,
	r=	0x13,
	t=	0x14,
	y=	0x15,
	u=	0x16,
	i=	0x17,
	o=	0x18,
	p=	0x19,
	enter=	0x1C,
	a=	0x1E,
	s=	0x1F,
	d=	0x20,
	f=	0x21,
	g=	0x22,
	h=	0x23,
	j=	0x24,
	k=	0x25,
	l=	0x26,
	z=	0x2C,
	x=	0x2D,
	c=	0x2E,
	v=	0x2F,
	b=	0x30,
	n=	0x31,
	m=	0x32,
	space=	0x39
};

typedef struct
{
	uint8_t val;
	union {
		struct {
			enum Scancodes codes;
		};
	};
} Scancode;

/*
 * Struct to represent each scancode value.
 * */
const Scancode scancodes[] = {
	[esc] =		{.val = '\0'},
	[n_1] = 	{.val = '1'},
	[n_2] = 	{.val = '2'},
	[n_3] = 	{.val = '3'},
	[n_4] = 	{.val = '4'},
	[n_5] = 	{.val = '5'},
	[n_6] = 	{.val = '6'},
	[n_7] = 	{.val = '7'},
	[n_8] = 	{.val = '8'},
	[n_9] = 	{.val = '9'},
	[n_0] = 	{.val = '0'},
	[minus] =	{.val = '-'},
	[equals]=	{.val = '='},
	[tab]=		{.val = '\t'},
	[enter]=	{.val = '\n'},
	[a]=		{.val = 'a'},
	[b]=		{.val = 'b'},
	[c]=		{.val = 'c'},
	[d]=		{.val = 'd'},
	[e]=		{.val = 'e'},
	[f]=		{.val = 'f'},
	[g]=		{.val = 'g'},
	[h]=		{.val = 'h'},
	[i]=		{.val = 'i'},
	[j]=		{.val = 'j'},
	[k]=		{.val = 'k'},
	[l]=		{.val = 'l'},
	[m]=		{.val = 'm'},
	[n]=		{.val = 'n'},
	[o]=		{.val = 'o'},
	[p]=		{.val = 'p'},
	[q]=		{.val = 'q'},
	[r]=		{.val = 'r'},
	[s]=		{.val = 's'},
	[t]=		{.val = 't'},
	[u]=		{.val = 'u'},
	[v]=		{.val = 'v'},
	[w]=		{.val = 'w'},
	[x]=		{.val = 'x'},
	[y]=		{.val = 'y'},
	[z]=		{.val = 'z'},
	[space]=	{.val = ' '},
	[back]=		{.val = '\b'}
};

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

void update_cursor()
{	
	uint32_t *fb = (uint32_t *)*(uint32_t *) FBUF;
	
	fb += GetCoords();
	fb += (WIDTH * 15);

	TextFont = (uint8_t *)(TMEM + ((127 * 16 ) - 1));
	for(int8_t b = 7; b >= 0; b--)
	{
		*fb = (*TextFont & (1 << b)) ? MakeColor(255, 255, 255) : MakeColor(0, 0, 0);
		fb++;
	}
}

void remove_cursor()
{
	uint32_t *fb = (uint32_t *)*(uint32_t *) FBUF;
	fb += GetCoords();
	fb += (WIDTH * 15);

	for(int8_t b = 7; b >= 0; b--)
	{
		*fb++ = MakeColor(0, 0, 0);
	}
}

void Update(uint32_t x, uint32_t y)
{
	remove_cursor();
	cx = x;
	cy = y;
	update_cursor();
}

uint8_t CheckScreen()
{
	uint32_t *ss, *ss2;

	if(cy >= 40)
	{
		remove_cursor();
		ss = (uint32_t *)*(uint32_t *)0x4028;
		ss2 = ss + (1024 * 24);
		for(uint32_t p = 0; p < 0xBA000; p++)
			*ss++ = *ss2++;
		for(uint32_t p = 0; p < 0x6000; p++)
			*ss++ = MakeColor(0, 0, 0);
		Update(0, cy-1);
		return 1;
	}
	return 0;

}

void setCoords(uint32_t x, uint32_t y)
{
	Update(x, y);
}

void clear_screen()
{
	uint32_t i;
	uint32_t *FrameBuffer = (uint32_t *)*(uint32_t *)0x4028;
	for(i = 0; i < WIDTH*HEIGHT; i++)
		FrameBuffer[i] = MakeColor(0, 0, 0);
	Update(0, 0);
}


//#include "include/idt.h"

#endif

#ifndef UTIL
#define UTIL

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

// Drive number
volatile uint8_t *DriveNum = (volatile uint8_t *)0x1500;

// Regular Text.
volatile uint8_t *VidMem = (volatile uint8_t *)0xB8000;

// Colored Text.
volatile uint16_t *AVidMem = (volatile uint16_t *)0xB8000;

// Text Font
volatile uint8_t *TextFont = (volatile uint8_t *)0x2000;

// Current cursor position
static uint16_t cursor_pos;

// cx, cy - Cursor x, Cursor y. Used for accurate cursor positions.
static uint16_t cx, cy;

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
	[space]=	{.val = ' '}
};

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
//#include "include/idt.h"

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

uint16_t Coords(uint8_t _x, uint8_t _y)
{
	cx = _x;
	cy = _y;
	return _y * WIDTH + _x;
}

void clear_screen(uint32_t color)
{
	
	uint32_t i = 0;
	while(i < (80 * 25 * 2))
	{
	    *(VidMem + i) = ' ';
	    i++;
	    *(VidMem + i) = white;
	    i++;
	}
	reset_VidMem();
	reset_AVidMem();
	
	update_cursor(Coords(5, 1));
}

void init_cursor()
{
	outb(0x3D4, 0x0A);
	outb(0x3D5, (inb(0x3D5) & 0xC0) | 0);
	outb(0x3D4, 0x0B);
	outb(0x3D5, (inb(0x3D5) & 0xE0) | 25);	
}

#include "include/idt.h"

#endif

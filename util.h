#ifndef UTIL
#define UTIL

#include "include/stdint.h"
#include "include/stdio.h"

#define WIDTH	1024//800
#define HEIGHT	768//600

// Make RGB color for pixels.
#define MakeColor(R, G, B) R*65536 + G*256 + B

void *memset(void *b, uint32_t val, uint32_t count)
{
	__asm__ __volatile__ ("cld; rep stosb" : "+c"(count), "+D"(b) : "a"(val) : "memory");
	return b;
}

// Drive number
volatile uint8_t *DriveNum = (volatile uint8_t *)0x1500;

// Text Font
volatile uint8_t *TextFont = (volatile uint8_t *)0x2000;

// File Manager
volatile uint8_t *FileManager = (volatile uint8_t*)0x3000;

// Current cursor position
static uint32_t *last_pos;

// cx, cy - Cursor x, Cursor y. Used for accurate cursor positions.
static uint16_t cx, cy;

// 1 extra bit in between.
#define GetCoords()	(cx * 8) + (cy * 16 * WIDTH)

// Memory address of our custom font.
#define TMEM	0x2000

typedef struct vbe_mode_block
{
	uint16_t	attributes;
	uint8_t		window_a;
	uint8_t		window_b;
	uint16_t	granularity;
	uint16_t	window_size;
	uint16_t	segment_a;
	uint16_t	segment_b;
	uint32_t	win_func_ptr;
	uint16_t	pitch;
	uint16_t	width;
	uint16_t	height;
	uint8_t		w_char;
	uint8_t		y_char;
	uint8_t		planes;
	uint8_t		bpp;
	uint8_t		banks;
	uint8_t		memory_model;
	uint8_t		bank_size;
	uint8_t		image_pages;
	uint8_t		reserved1;
	uint8_t		red_mask_size;
	uint8_t		red_field_pos;
	uint8_t		green_mask_size;
	uint8_t		green_field_pos;
	uint8_t		blue_mask_size;
	uint8_t		blue_field_pos;
	uint8_t		reserved_mask_size;
	uint8_t		reserved_field_pos;
	uint8_t		direct_color_mode_info;
	uint32_t	physical_base_ptr;
	uint32_t	reserved2;
	uint16_t	reserved3;
	uint16_t	linear_bytes_psl;
	uint8_t		bank_number_of_ip;
	uint8_t		linear_number_of_ip;
	uint8_t		linear_red_mask_size;
	uint8_t		linear_red_field_pos;
	uint8_t		linear_green_mask_size;
	uint8_t		linear_green_field_pos;
	uint8_t		linear_blue_mask_size;
	uint8_t		linear_blue_field_pos;
	uint8_t		linear_res_mask_size;
	uint8_t		linear_res_field_pos;
	uint32_t	max_pixel_clock;
	uint8_t		reserved4[190];
} __attribute__((packed))Vbe_mode_block;

Vbe_mode_block *info = (Vbe_mode_block*)0x4000;

#define FBUF	info->physical_base_ptr

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
	uint32_t *fb = last_pos;

	TextFont = (uint8_t *)(TMEM + ((127 * 16) - 16));
	for(uint8_t l = 0; l < 16; l++)
	{
		for(int8_t b = 7; b >= 0; b--)
		{
			*fb = (TextFont[l] & (1 << b)) ? MakeColor(255, 255, 255) : MakeColor(0, 0, 0);
			fb++;
		}
		fb+=(WIDTH - 8);
	}
}

void remove_cursor()
{
	uint32_t *fb = last_pos;

	TextFont = (uint8_t *)(TMEM + ((127 * 16) - 16));
	for(uint8_t l = 0; l < 16; l++)
	{
		for(int8_t b = 7; b >= 0; b--)
		{
			*fb = (TextFont[l] & (1 << b)) ? MakeColor(0, 0, 0) : MakeColor(0, 0, 0);
			fb++;
		}
		fb += (WIDTH - 8);
	}
}

void clear_screen()
{
	uint32_t i;
	uint32_t *FrameBuffer = (uint32_t *)FBUF;
	for(i = 0; i < WIDTH*HEIGHT; i++)
		FrameBuffer[i] = MakeColor(0, 0, 0);
	cx=cy=0;
}

void init_screen()
{
	clear_screen();
	cx=cy=0;

	last_pos = (uint32_t*)FBUF;
	last_pos += GetCoords();
	update_cursor();
}

void update_screen()
{
	last_pos = (uint32_t *)FBUF;
	last_pos += GetCoords();

	update_cursor();
}

void setCoords(uint32_t x, uint32_t y)
{
	remove_cursor();
	cx=x;
	cy=y;

	last_pos = (uint32_t *)FBUF;
	last_pos += GetCoords();
	update_cursor();
}


uint8_t CheckScreen()
{
	uint32_t *ss, *ss2;

	if(cy >= 40)
	{
		remove_cursor();
		ss = (uint32_t *)FBUF;
		ss2 = ss + (1024 * 24);

		for(uint32_t p = 0; p < 0xBA000; p++)
			*ss++ = *ss2++;
		for(uint32_t p = 0; p < 0x6000; p++)
			*ss++ = MakeColor(0, 0, 0);
		cx=0;
		cy--;
		update_screen();
		return 1;
	}
	return 0;

}


#endif

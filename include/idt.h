#ifndef IDT
#define IDT
#include "stdint.h"

typedef struct
{
	uint16_t	low;
	uint16_t	kernel_cs;
	uint8_t		reserved;
	uint8_t		attributes;
	uint16_t	high;
} __attribute__((packed)) idt;

typedef struct
{
	uint16_t	limit;
	uint32_t	base;
} __attribute__((packed)) idtr;

extern idt	_idt[256];
extern uint32_t  isr1;
static idtr	_idtr;

void idt_set_desc(uint8_t v, uint8_t isr, uint8_t flags)
{
	idt *desc = &_idt[v];

	desc->low = (uint32_t)isr & 0xFFFF;
	desc->kernel_cs = 0x08;
	desc->attributes = flags;
	desc->high = (uint32_t)isr >> 16;
	desc->reserved = 0;
}

//void idt_init()
//{
//	_idtr.base = (uint32_t )&_idt[0];
//
//	_idtr.limit = (uint16_t)sizeof(idt) * 256 - 1;
//
//	for(uint8_t v = 0; v < 32; v++)
//	{
//		idt_set_desc(v, v, 0x8E);
//	}
//}
extern void loadIDT();

void idt_init()
{
	for(uint16_t i = 0; i < 256; i++)
	{
		_idt[i].reserved = 0;
		_idt[i].low = (uint32_t)isr1 & 0xFFFF;
		_idt[i].kernel_cs = 0x08;
		_idt[i].attributes = 0x8e;
		_idt[i].high = (uint32_t)isr1 >> 16;
	}
	
	outb(0x21, 0xFD);
	outb(0xA1, 0xFF);
	loadIDT();
}

void isr1_handler()
{
	outb(0x20, 0x20);
	outb(0xa0, 0x20);
}

#endif

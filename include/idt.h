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

//idt _idt[256];
extern idt	_idt[256];
extern uint32_t  isr1;
static idtr	_idtr;

void set_gate(uint8_t num, uint32_t base, uint16_t kcs, uint8_t flags)
{
	_idt[num].reserved = 0x0;
	_idt[num].low = (base & 0xFFFF);
	_idt[num].high = (base >> 16) & 0xFFFF;
	_idt[num].kernel_cs = kcs;
	_idt[num].attributes = flags | 0x60;
}

void memset(void *ptr, uint32_t val, uint32_t c)
{
	__asm__ __volatile__ ("cld; rep stosb" : "+c" (c), "+D" (ptr) : "a" (val) : "memory");
}

extern void loadIDT();

void idt_init()
{
	_idtr.limit = (sizeof(idt) * 256) - 1;
	_idtr.base = (uint32_t)&_idt;

	memset(&_idt, 0, sizeof(idt) * 256);

	//for(uint16_t i = 0; i < 256; i++)
	//{
	//	_idt[i].reserved = 0;
	//	_idt[i].low = (uint32_t)isr1 & 0xFFFF;
	//	_idt[i].kernel_cs = 0x08;
	//	_idt[i].attributes = 0x8e;
	//	_idt[i].high = (uint32_t)isr1 >> 16;
	//}
	
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

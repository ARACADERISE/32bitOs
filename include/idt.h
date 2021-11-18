#ifndef IDT
#define IDT
#include "stdint.h"

#define gate_interrupt	0x8E
#define	gate_trap	0x8F

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

typedef struct
{
	uint32_t	ip;
	uint32_t	cs;
	uint32_t	flags;
	uint32_t	sp;
	uint32_t	ss;
} __attribute__((packed)) interrupt_frame;

//idt _idt[256];
extern idt	_idt[256];
//extern uint32_t  isr1;
static idtr	_idtr;

void set_gate(uint8_t num, void *base, uint8_t flags)
{
	idt *_Idt = &_idt[num];
	_Idt->reserved		= 0x0;
	_Idt->low		= (uint32_t)base & 0xFFFF;
	_Idt->high		= ((uint32_t) base >> 16) & 0xFFFF;
	_Idt->kernel_cs		= 0x08;
	_Idt->attributes 	= flags | 0x60;
}

/*
 * Function wrapper for each interrupt.
 * */
__attribute__ ((interrupt))
void exception_handler_err(interrupt_frame *_interrupt, uint32_t err_code)
{
	//Print((uint8_t *)"ERR CODE", MakeColor(255, 255, 255), MakeColor(0, 0, 0), 0);
	if(err_code == 0xFFFC)
		__asm("int $0x3");
}
__attribute__ ((interrupt))
void exc_handler(interrupt_frame *_interrupt)
{
	//PrintHex(_interrupt->ip, MakeColor(255, 255, 255), MakeColor(0, 0, 0));
	//Print((uint8_t *)"NO ERR CODE", MakeColor(255, 255, 255), MakeColor(0, 0, 0), 0);
	__asm("hlt");
}
__attribute__ ((interrupt))
void int_handler(interrupt_frame *_interrupt)
{
	//Print((uint8_t *)"Interrupt", MakeColor(255, 255, 255), MakeColor(0, 0, 0), 0);
}

__attribute__((interrupt))
void interrupt_03(interrupt_frame *_interrupt)
{
	uint8_t good = 0x02;
	while(good & 0x02)
		good = inb(0x64);
	outb(0x64, 0xFE);
	_interrupt->ip++;
	__asm("hlt");
}

/*
 * TODO: Fill these two arrays out with interrupt/exception functions.
 *	For now, they are filled out with current exception/interrupt handlers.
 *	Would like to fill out with user-defined functions for each possible interrupt
 *	and exception.
 * */
static uint8_t inerr_index = 0;
void *interrupts_no_err[24] = {
	exc_handler, exc_handler, exc_handler, interrupt_03,
	exc_handler, exc_handler, exc_handler, exc_handler,
	exc_handler, exc_handler, exc_handler, exc_handler,
	exc_handler, exc_handler, exc_handler, exc_handler,
	exc_handler, exc_handler, exc_handler, exc_handler,
	exc_handler, exc_handler, exc_handler, exc_handler
};

static uint8_t iwerr_index = 0;
void *interrupts_with_err[8] = {
	exception_handler_err, exception_handler_err, 
	exception_handler_err, exception_handler_err,
	exception_handler_err, exception_handler_err, 
	exception_handler_err, exception_handler_err
};

void idt_init()
{
	
	//_idtr.limit = (sizeof(idt) * 256) - 1;
	_idtr.limit = (uint16_t) sizeof(_idt);
	_idtr.base = (uint32_t)&_idt;

	memset(&_idt, 0, sizeof(idt) * 256);

	for(uint8_t i = 0; i < 32; i++)
	{
		if(i == 8 || i == 10 || i == 11 || i == 12 ||
		   i == 13 || i == 14 || i == 17 || i == 21)
		{
			set_gate(i, interrupts_with_err[iwerr_index], gate_trap);
			iwerr_index++;
		}
		else
		{
			set_gate(i, interrupts_no_err[inerr_index], gate_trap);
			inerr_index++;
		}
	}

	for(uint16_t i = 32; i < 256; i++)
	{
		set_gate(i, int_handler, gate_interrupt);
	}

	__asm__ __volatile__("lidt %0" : : "memory"(_idtr));
	//__asm__ __volatile__("sti");
}

#endif

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
	_Idt->attributes 	= flags;// | 0x60;
}

void memset(void *ptr, uint32_t val, uint32_t c)
{
	__asm__ __volatile__ ("cld; rep stosb" : "+c" (c), "+D" (ptr) : "a" (val) : "memory");
}

/*
 * Function wrapper for each interrupt.
 * */
__attribute__ ((interrupt))
void exception_handler_err(interrupt_frame *_interrupt, uint32_t err_code)
{
	Print((uint8_t *)"ERR CODE", MakeColor(255, 255, 255), MakeColor(0, 0, 0));
}
__attribute__ ((interrupt))
void exc_handler(interrupt_frame *_interrupt)
{
	//PrintHex(_interrupt->cs, MakeColor(255, 255, 255), MakeColor(0, 0, 0));
	Print((uint8_t *)"NO ERR CODE", MakeColor(255, 255, 255), MakeColor(0, 0, 0));
}
__attribute__ ((interrupt))
void int_handler(interrupt_frame *_interrupt)
{
	Print((uint8_t *)"Interrupt", MakeColor(255, 255, 255), MakeColor(0, 0, 0));
}

__attribute__((interrupt))
void interrupt_03(interrupt_frame *_interrupt)
{
	Print((uint8_t*)"INTERRUPT 03", MakeColor(255, 255, 255), MakeColor(0, 0, 0));
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
	_idtr.limit = (sizeof(idt) * 256) - 1;
	_idtr.base = (uint32_t)&_idt[0];

	memset(&_idt, 0, sizeof(idt) * 256);

	for(uint8_t i = 0; i < 32; i++)
	{
		switch(i)
		{
			case 8:
			case 10:
			case 11:
			case 12:
			case 13:
			case 14:
			case 17:
			case 21:
			{
				set_gate(i, interrupts_with_err[iwerr_index], gate_trap);
				iwerr_index++;
			}
			default:
			{
				set_gate(i, interrupts_no_err[inerr_index], gate_trap);
				inerr_index++;
			}
		}

	}

	for(uint16_t i = 32; i < 256; i++)
	{
		set_gate(i, int_handler, gate_interrupt);
	}

	__asm__ __volatile__("lidt %0" : : "memory"(_idtr));
}
/*
 * KEEP FOR POSSIBLE REFERENCE
typedef struct registers
{
	uint32_t ds;
	uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
	uint32_t int_no, err_code;
	uint32_t eip, cs, eflags, useresp, ss;
} Registers;

typedef void (*isr_t)(Registers);
isr_t handlers[256];
void r_interrupt_handler(uint8_t n, isr_t h)
{
	handlers[n] = h;
}

void isr_handle(Registers r)
{
	if(r.int_no == 1)
		Print((uint8_t *)"YES", MakeColor(255, 255, 255), MakeColor(0, 0, 0));
}
void irq_handle(Registers r)
{
	if(r.int_no >= 40)
		outb(0xA0, 0x20);
	outb(0x20, 0x20);

	if(handlers[r.int_no] != 0)
	{
		if(r.int_no == 1)
			Print((uint8_t *)"OK", MakeColor(0, 200, 200), MakeColor(0, 0, 0));
		isr_t h = handlers[r.int_no-32];
		h(r);
	}
}

void isr1_handle()
{
	outb(0x20, 0x20);
	outb(0xa0, 0x20);
}*/

#endif

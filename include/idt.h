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

extern void isr0();
extern void isr1();
extern void isr2();
extern void isr3();
extern void isr4();
extern void isr5();
extern void isr6();
extern void isr7();
extern void isr8();
extern void isr9();
extern void isr10();
extern void isr11();
extern void isr12();
extern void isr13();
extern void isr14();
extern void isr15();
extern void isr16();
extern void isr17();
extern void isr18();
extern void isr19();
extern void isr20();
extern void isr21();
extern void isr22();
extern void isr23();
extern void isr24();
extern void isr25();
extern void isr26();
extern void isr27();
extern void isr28();
extern void isr29();
extern void isr30();
extern void isr31();

extern void irq0();
extern void irq1();
extern void irq2();
extern void irq3();
extern void irq4();
extern void irq5();
extern void irq6();
extern void irq7();
extern void irq8();
extern void irq9();
extern void irq10();
extern void irq11();
extern void irq12();
extern void irq13();
extern void irq14();
extern void irq15();

//idt _idt[256];
extern idt	_idt[256];
//extern uint32_t  isr1;
static idtr	_idtr;

void set_gate(uint8_t num, uint32_t base, uint16_t kcs, uint8_t flags)
{
	_idt[num].reserved = 0x0;
	_idt[num].low = (base & 0xFFFF);
	_idt[num].high = (base >> 16) & 0xFFFF;
	_idt[num].kernel_cs = kcs;
	_idt[num].attributes = flags;// | 0x60;
}

void memset(void *ptr, uint32_t val, uint32_t c)
{
	__asm__ __volatile__ ("cld; rep stosb" : "+c" (c), "+D" (ptr) : "a" (val) : "memory");
}

//extern void loadIDT();
extern void flush_idt(uint32_t);

void idt_init()
{
	_idtr.limit = (sizeof(idt) * 256) - 1;
	_idtr.base = (uint32_t)&_idt;

	memset(&_idt, 0, sizeof(idt) * 256);

	outb(0x20, 0x11);
	outb(0xA0, 0x11);
	outb(0x21, 0x20);
	outb(0xA1, 0x28);
	outb(0x21, 0x04);
	outb(0xA1, 0x02);
	outb(0x21, 0x01);
	outb(0xA1, 0x01);
	outb(0x21, 0x0);
	outb(0xA1, 0x0);


	set_gate(0, (uint32_t)isr0, 0x08, 0x8E);
	set_gate(1, (uint32_t)isr1, 0x08, 0x8E);
	set_gate(2, (uint32_t)isr2, 0x08, 0x8E);
	set_gate(3, (uint32_t)isr3, 0x08, 0x8E);
	set_gate(4, (uint32_t)isr4, 0x08, 0x8E);
	set_gate(5, (uint32_t)isr5, 0x08, 0x8E);
	set_gate(6, (uint32_t)isr6, 0x08, 0x8E);
	set_gate(7, (uint32_t)isr7, 0x08, 0x8E);
	set_gate(8, (uint32_t)isr8, 0x08, 0x8E);
	set_gate(9, (uint32_t)isr9, 0x08, 0x8E);
	set_gate(10, (uint32_t)isr10, 0x08, 0x8E);
	set_gate(11, (uint32_t)isr11, 0x08, 0x8E);
	set_gate(12, (uint32_t)isr12, 0x08, 0x8E);
	set_gate(13, (uint32_t)isr13, 0x08, 0x8E);
	set_gate(14, (uint32_t)isr14, 0x08, 0x8E);
	set_gate(15, (uint32_t)isr15, 0x08, 0x8E);
	set_gate(16, (uint32_t)isr16, 0x08, 0x8E);
	set_gate(17, (uint32_t)isr17, 0x08, 0x8E);
	set_gate(18, (uint32_t)isr18, 0x08, 0x8E);
	set_gate(19, (uint32_t)isr19, 0x08, 0x8E);
	set_gate(20, (uint32_t)isr20, 0x08, 0x8E);
	set_gate(21, (uint32_t)isr21, 0x08, 0x8E);
	set_gate(22, (uint32_t)isr22, 0x08, 0x8E);
	set_gate(23, (uint32_t)isr23, 0x08, 0x8E);
	set_gate(24, (uint32_t)isr24, 0x08, 0x8E);
	set_gate(25, (uint32_t)isr25, 0x08, 0x8E);
	set_gate(26, (uint32_t)isr26, 0x08, 0x8E);
	set_gate(27, (uint32_t)isr27, 0x08, 0x8E);
	set_gate(28, (uint32_t)isr28, 0x08, 0x8E);
	set_gate(29, (uint32_t)isr29, 0x08, 0x8E);
	set_gate(30, (uint32_t)isr30, 0x08, 0x8E);
	set_gate(31, (uint32_t)isr31, 0x08, 0x8E);

	//for(uint16_t i = 0; i < 256; i++)
	//{
	//	_idt[i].reserved = 0;
	//	_idt[i].low = (uint32_t)isr1 & 0xFFFF;
	//	_idt[i].kernel_cs = 0x08;
	//	_idt[i].attributes = 0x8e;
	//	_idt[i].high = (uint32_t)isr1 >> 16;
	//}
	
	//outb(0x21, 0xFD);
	//outb(0xA1, 0xFF);
	//flush_idt((uint32_t)&_idtr);
	//loadIDT();

	set_gate(32, (uint32_t)irq0, 0x08, 0x8E);
	set_gate(33, (uint32_t)irq1, 0x08, 0x8E);
	set_gate(34, (uint32_t)irq2, 0x08, 0x8E);
	set_gate(35, (uint32_t)irq3, 0x08, 0x8E);
	set_gate(36, (uint32_t)irq4, 0x08, 0x8E);
	set_gate(37, (uint32_t)irq5, 0x08, 0x8E);
	set_gate(38, (uint32_t)irq6, 0x08, 0x8E);
	set_gate(39, (uint32_t)irq7, 0x08, 0x8E);
	set_gate(40, (uint32_t)irq8, 0x08, 0x8E);
	set_gate(41, (uint32_t)irq9, 0x08, 0x8E);
	set_gate(42, (uint32_t)irq10, 0x08, 0x8E);
	set_gate(43, (uint32_t)irq11, 0x08, 0x8E);
	set_gate(44, (uint32_t)irq12, 0x08, 0x8E);
	set_gate(45, (uint32_t)irq13, 0x08, 0x8E);
	set_gate(46, (uint32_t)irq14, 0x08, 0x8E);
	set_gate(47, (uint32_t)irq15, 0x08, 0x8E);
	flush_idt((uint32_t)&_idtr);

}

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
	Print((uint8_t *)"YES", MakeColor(255, 255, 255), MakeColor(0, 0, 0));
}
void irq_handle(Registers r)
{
	if(r.int_no >= 40)
		outb(0xA0, 0x20);
	outb(0x20, 0x20);

	if(handlers[r.int_no] != 0)
	{
		isr_t h = handlers[r.int_no];
		h(r);
	}
}

void isr1_handle()
{
	outb(0x20, 0x20);
	outb(0xa0, 0x20);
}

#endif

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

static idt	_idt[256];
static idtr	_idtr;

#endif

SECTIONS
{
	.text 0x5000 :
	{
		*(kernel_entry);
		*(.text*);
	}

	.idt BLOCK(0x1000) : ALIGN(0x1000)
	{
		_idt = .;
		. = . * 0x1000;
	}

	.rodata :
	{
		*(.rodata*);
	}
	.bss :
	{
		*(.bss*);
	}
}

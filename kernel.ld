SECTIONS
{
	.text 0x5000 :
	{
		*(kernel_entry);
		*(.text*);
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

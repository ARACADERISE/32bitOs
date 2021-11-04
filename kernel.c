
#include "include/stdio.h"
#include "include/idt.h"
#include "include/string.h"

__attribute__((section("kernel_entry"))) void kernel_main(void)
{
	idt_init();
	clear_screen();	

	uint8_t c = 0;
	uint8_t input[8];
	uint8_t index = 0;

	setCoords(10, 4);	
	Print((uint8_t *)"Welcome to MocaOS!\n\tCreated and Distributed by ARACADERISE(MocaCDeveloper)\n\tCopyright 2021, All Rights Reserved\n\n", MakeColor(124, 253, 236), MakeColor(0, 0, 0));
	PutC('>', MakeColor(255, 255, 255), MakeColor(0, 0, 0));

	while(1)
	{
		c = getc((index != 0) ? 1 : 0);
		if(c == '0')
		{
			if(strcmp(input, (uint8_t *) "clear")==0)
				clear_screen();
			else if(strcmp(input, (uint8_t *) "reboot")==0)
			{
				//__asm ("jmpl $0x0F000,$0x0FFF0");
				__asm("jmpl $0xFFFF, $0x0000");
			}
			else {
				CheckScreen();
				Print((uint8_t *)"\n\t Unknown Command ", MakeColor(255, 255, 255), MakeColor(255, 0, 0));
				Update(0, cy+1);
			}

			index=0;
			for(int8_t i = 0; i < 8; i++)
				input[i] = 0;
			PutC('>', MakeColor(255, 255, 255), MakeColor(0, 0, 0));
			continue;
		}
		if(c == '\4')
		{
			input[index] = 0;
			index--;
			continue;
		}
		if(c == '\3') continue;
		if(c)
		{
			input[index] = PutC(c, MakeColor(255, 255, 255), MakeColor(0, 0, 0));
			index++;
		}
	}

}

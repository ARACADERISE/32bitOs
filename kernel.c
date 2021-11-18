
#include "include/stdio.h"
#include "include/idt.h"
#include "include/string.h"
#include "include/memory.h"

typedef struct SMAP_entry
{
	uint64_t	base;
	uint64_t	length;
	uint32_t	type;
	uint32_t	acpi;
} __attribute__((packed)) SMAP_entry_t;


__attribute__((section("kernel_entry"))) void kernel_main(void)
{
	idt_init();
	init_screen();

	//setCoords(10, 2);
	Print((uint8_t *)"\n\n\t\tWelcome to MocaOS\n"
			 "\tDistributed and kept to date by ARACADERISE(MocaCDeveloper)\n"
			 "\tCopyright 2021, MocaCDeveloper. All Rights Reserved\n\n",
			// Foreground
			MakeColor(117, 196, 255),
			// Background
			MakeColor(0, 0, 0),
			// Newline? Yes!
			0
	);

	uint8_t index = 0;
	uint8_t input[8];
	uint8_t c;

	//setCoords(4, cy);
	
	//Print((uint8_t *)"\t>", MakeColor(255, 255, 255), MakeColor(0, 0, 0), 1);
	PutC('>', MakeColor(255, 255, 255), MakeColor(0, 0, 0));

	while(1) {
		c = getc((index != 0) ? 1 : 0);

		if(c == '0')
		{
			if(strcmp(input, (uint8_t *)"clear")==0)
			{
				clear_screen();
				goto end;
			}
			if(strcmp(input, (uint8_t *)"reboot")==0)
			{
				__asm("int $0x3");//("jmpl $0xFFFF, $0x0000");
				goto end;
			}
			if(strcmp(input, (uint8_t *)"table")==0)
			{
				PrintTable();
				goto end;
			}
			CheckScreen();
			cy++;

end:
			index=0;
			for(uint8_t i = 0; i < 8; i++)
				input[i] = 0;
			
			//setCoords(4, cy);
			remove_cursor();
			cx=0;
			update_screen();
			//setCoords(4, cy);
			PutC('>', MakeColor(255, 255, 255), MakeColor(0, 0, 0));
			//Print((uint8_t *)"\t>", MakeColor(255, 255, 255), MakeColor(0, 0, 0), 1);
			continue;
		}
		if(c == '\4')
		{
			index--;
			input[index] = 0;
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

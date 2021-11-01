
#include "include/stdio.h"
#include "include/idt.h"
#include "include/string.h"

__attribute__((section("kernel_entry"))) void kernel_main(void)
{
	init_cursor();
	update_cursor(Coords(5, 1));

	idt_init();

	
	Print(
		(const uint8_t *)"MocaOS\n\tCreated by MocaCDeveloper\n\t"
				 "Copyright 2021, All Rights Reserved.\n\t"
				 "https://github.com/ARACADERISE/32bitOs\n\n",
		LCOB,
		1
	);

	uint8_t c = 0;
	uint8_t input[8];
	uint8_t index = 0;
	CPutC('>', WOB);
	while(1)
	{
		c = getc();
		if(c == '0')
		{
		    if(strcmp(input, (uint8_t *)"help")==0)
		        Print(
		            (const uint8_t *) "\tThis is the Help Menu!\n", 
		            LMOB, 
		            1
		        );
            if(strcmp(input, (uint8_t *)"clear")==0)
                clear_screen((uint32_t)WOB);
		    index = 0;
		    for(uint8_t i = 0; i < 8; i++)
		        input[i] = '\0';
		    CPutC('>', WOB);
		    continue;
		}
		if(c)
		{
			input[index] = c;
			index++;
			CPutC(c, WOB);
			//cursor_pos++;
		}
	}

}

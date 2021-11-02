
#include "include/stdio.h"
#include "include/idt.h"
#include "include/string.h"

__attribute__((section("kernel_entry"))) void kernel_main(void)
{
	init_cursor();
	//update_cursor(Coords(5, 1));
	update_cursor();
	idt_init();

	
	//fb[0] = 0x00EEEEEE;
	//fb[1] = 0x00EEEEEE;
	//fb[2] = 0x00EEEEEE;
	//fb[3] = 0x00EEEEEE;
	
	clear_screen();	
	cx = cy = 0;
	
	uint32_t *FrameBuffer = (uint32_t *)*(uint32_t *) 0x4028;
	FrameBuffer += GetCoords();
	TextFont = (uint8_t *)(0x2000 + (('B' * 16) - 16));
	
	for(uint8_t line = 0; line < 16; line++)
	{
		for(int8_t bit = 7; bit >= 0; bit--)
		{
			*FrameBuffer = (TextFont[line] & (1 << bit)) ? MakeColor(124, 253, 236) : MakeColor(0, 0, 0);
			FrameBuffer++;
		}
		FrameBuffer += (WIDTH - 8);
	}
	cx++;

	update_cursor();
	//fb = (uint32_t *)*(uint32_t *) 0x4028;
	//fb += GetCoords();
	//fb += (800 * 15);
	//TextFont = (uint8_t *)(0x2000 + ((127 * 16) - 1));
	//for(int8_t bit = 7; bit >= 0; bit--)
	//{
	//	*fb = (*TextFont & (1 << bit)) ? MakeColor(0, 0, 0) : MakeColor(0, 0, 0);
	//	fb++;
	//}
	//Print(
	//	(const uint8_t *)"MocaOS\n\tCreated by MocaCDeveloper\n\t"
	//			 "Copyright 2021, All Rights Reserved.\n\t"
	//			 "https://github.com/ARACADERISE/32bitOs\n\n",
	//	LCOB,
	//	1
	//);

	//uint8_t c = 0;
	//uint8_t input[8];
	//uint8_t index = 0;
	//CPutC('>', WOB);
	while(1)
	{
	//	c = getc();
	//	if(c == '0')
	//	{
	//	    if(strcmp(input, (uint8_t *)"help")==0)
	//	        Print(
	//	            (const uint8_t *) "\tThis is the Help Menu!\n", 
	//	            LMOB, 
	//	            1
	//	        );
          //  if(strcmp(input, (uint8_t *)"clear")==0)
          //      clear_screen((uint32_t)WOB);
	//	    index = 0;
	//	    for(uint8_t i = 0; i < 8; i++)
	//	        input[i] = '\0';
	//	    CPutC('>', WOB);
	//	    continue;
	//	}
	//	if(c)
	//	{
	//		input[index] = c;
	//		index++;
	//		CPutC(c, WOB);
			//cursor_pos++;
	//	}
	}

}


#include "util.h"

__attribute__((section("kernel_entry"))) void kernel_main(void)
{
	const int8_t *string = "Welcome!";

	//for(int32_t i = 0; i < 25; i++)
	//{
	//	for(int32_t x = 0; x < 80; x++)
	//	{
	//		*VidMem++ = ' ';
	//		*VidMem++ = 15;
	//	}
	//}

	//reset_VidMem();

	init_cursor();
	update_cursor(Coords(12, 18));
	Print((const uint8_t *)"Hi");
}

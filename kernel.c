
#include "util.h"

__attribute__((section("kernel_entry"))) void kernel_main(void)
{
	init_cursor();
	update_cursor(Coords(5, 1));

	
	Print(
		(const int8_t *)"MocaOS\n\tCreated by MocaCDeveloper\n\t"
				 "Copyright 2021, All Rights Reserved.\n\t"
				 "https://github.com/ARACADERISE/32bitOs\n\n",
		LCOB,
		1
	);
}

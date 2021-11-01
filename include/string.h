#ifndef STRING
#define STRING
#include "../util.h"

uint32_t strlen(const uint8_t *string)
{
	uint32_t length = 0;
	
	while(string[length] != 0)
		length++;

	return length;
}

uint8_t strcmp(uint8_t *str1, uint8_t *str2)
{
    if(!(strlen(str1) == strlen(str2)))
        return 3;
    
    for(uint8_t i = 0; i < strlen(str1); i++)
    {
        if(str1[i] != str2[i])
            return 1;
    }
    
    return 0;
}

#endif
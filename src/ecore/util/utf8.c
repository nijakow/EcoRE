#include "utf8.h"

bool Eco_Utf8_Decode(const char* str,
                     Eco_Codepoint* result,
                     unsigned int* length)
{
    Eco_Codepoint  value;
    unsigned int   index;
    unsigned int   bits;
    char           c;

    if ((str[0] & (char) 0x80) == 0x00) {
        *result = (unsigned char) str[0];
        *length = 1;
        return true;
    } else {
        c    = str[0];
        bits = 0;
        while ((c & (char) 0x80) != 0x00)
        {
            c    = c    << 1;
            bits = bits  + 1;
        }
        value = str[0] & ((1 << (7 - bits)) - 1);
        for (index = 1; (((unsigned char) str[index]) & (unsigned char) 0xc0) == 0x80; index++)
        {
            value <<= 6;
            value  |= str[index] & (char) 0x3f;
        }

        *result = value;
        *length = index;

        return true;
    }
}

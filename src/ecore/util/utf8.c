#include "utf8.h"

unsigned int Eco_Utf8_Encode(Eco_Codepoint codepoint, char* buffer)
{
    if (codepoint < 0) {
        /*
         * Insert Unicode's REPLACEMENT CHARACTER
         */
        buffer[0] = 0xef;
        buffer[1] = 0xbf;
        buffer[2] = 0xbd;
        buffer[3] = 0x00;
        return 3;
    } else if (codepoint <= 0x7f) {
        buffer[0] = codepoint;
        return 1;
    } else if (codepoint <= 0x7ff) {
        buffer[0] = ((codepoint >> 6) & 0x1f) | 0xc0;
        buffer[1] = ((codepoint >> 0) & 0x3f) | 0x80;
        return 2;
    } else if (codepoint <= 0xffff) {
        buffer[0] = ((codepoint >> 12) & 0x0f) | 0xe0;
        buffer[1] = ((codepoint >>  6) & 0x3f) | 0x80;
        buffer[2] = ((codepoint >>  0) & 0x3f) | 0x80;
        return 3;
    } else if (codepoint <= 0x10ffff) {
        buffer[0] = ((codepoint >> 18) & 0x07) | 0xf0;
        buffer[1] = ((codepoint >> 12) & 0x3f) | 0x80;
        buffer[2] = ((codepoint >>  6) & 0x3f) | 0x80;
        buffer[3] = ((codepoint >>  0) & 0x3f) | 0x80;
        return 4;
    } else {
        /*
         * Insert Unicode's REPLACEMENT CHARACTER
         */
        buffer[0] = 0xef;
        buffer[1] = 0xbf;
        buffer[2] = 0xbd;
        buffer[3] = 0x00;
        return 3;
    }
}

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

#ifndef EVE_H
#define EVE_H

#include <stdint.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

typedef uint8_t u8;
typedef uint32_t u32;

typedef u32 Eve_Int;

void Eve_PushFrame();
void Eve_PopFrame();

void Eve_SetColorRGBA(u8 r, u8 g, u8 b, u8 a);

void Eve_DrawRect(Eve_Int x, Eve_Int y, Eve_Int w, Eve_Int h);
void Eve_DrawText(const char* text, Eve_Int x, Eve_Int y);

void Eve_Clip(Eve_Int x, Eve_Int y, Eve_Int w, Eve_Int h);

#endif /* EVE_H */

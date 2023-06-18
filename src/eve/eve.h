#ifndef EVE_H
#define EVE_H

#include <stdint.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

typedef uint8_t u8;
typedef uint32_t u32;

void Eve_LoadIdentity();

void Eve_PushMatrix();
void Eve_PopMatrix();

void Eve_SetColorRGBA(u8 r, u8 g, u8 b, u8 a);

void Eve_DrawRect(u32 x, u32 y, u32 w, u32 h);
void Eve_DrawText(const char* text, u32 x, u32 y);

void Eve_Clip(u32 x, u32 y, u32 w, u32 h);

#endif /* EVE_H */

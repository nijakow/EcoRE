#ifndef EVE_H
#define EVE_H

#include <stdbool.h>
#include <stdint.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

typedef uint8_t u8;

typedef int32_t Eve_Int;
typedef uint32_t Eve_UInt;

Eve_UInt Eve_CurrentWidth();
Eve_UInt Eve_CurrentHeight();

void Eve_PushFrame();
void Eve_PopFrame();

void Eve_TranslateI2(Eve_Int x, Eve_Int y);

void Eve_SetColorRGBA(u8 r, u8 g, u8 b, u8 a);
void Eve_SetColorRGB(u8 r, u8 g, u8 b);

void Eve_Clip(Eve_Int x, Eve_Int y, Eve_Int w, Eve_Int h);

void Eve_DrawLine(Eve_Int x1, Eve_Int y1, Eve_Int x2, Eve_Int y2);

void Eve_DrawRect(Eve_Int x, Eve_Int y, Eve_Int w, Eve_Int h);
void Eve_FillRect(Eve_Int x, Eve_Int y, Eve_Int w, Eve_Int h);

void Eve_DrawText(const char* text, Eve_Int x, Eve_Int y);

void Eve_Render();


void Eve_PollEvent();

bool Eve_IsEventQuit();
bool Eve_IsEventKeyDown();
bool Eve_IsEventKeyUp();
bool Eve_IsEventMouseMotion();
bool Eve_IsEventMouseButtonDown();
bool Eve_IsEventMouseButtonUp();

Eve_UInt Eve_GetEventMouseX();
Eve_UInt Eve_GetEventMouseY();

Eve_UInt Eve_GetEventKeySym();

void Eve_Init();
void Eve_Terminate();

#endif /* EVE_H */

#ifndef EVE_H
#define EVE_H

#include <stdbool.h>
#include <stdint.h>

#include <math.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

typedef uint8_t u8;

typedef int32_t Eve_Int;
typedef uint32_t Eve_UInt;



struct Eve_Font
{
    TTF_Font*  font;
};

void Eve_Font_Create(struct Eve_Font* self, const char* path, Eve_UInt size);
void Eve_Font_CreateFromFont(struct Eve_Font* self, TTF_Font* font);
void Eve_Font_Destroy(struct Eve_Font* self);

struct Eve_Font*  Eve_Font_New(const char* path, Eve_UInt size);
struct Eve_Font*  Eve_Font_NewFromFont(TTF_Font* font);
void              Eve_Font_Delete(struct Eve_Font* self);

Eve_UInt Eve_Font_GetTextWidth(struct Eve_Font* self, const char* text);
Eve_UInt Eve_Font_GetTextHeight(struct Eve_Font* self, const char* text);

Eve_UInt Eve_Font_GetCharWidth(struct Eve_Font* self, Eve_UInt c);
Eve_UInt Eve_Font_GetCharHeight(struct Eve_Font* self, Eve_UInt c);

struct Eve_Font*  Eve_GetDefaultFont();



struct Eve_Texture
{
    SDL_Texture*  texture;
    Eve_Int       width;
    Eve_Int       height;
};

void Eve_Texture_CreateFromImage(struct Eve_Texture* self, const char* path);
void Eve_Texture_Destroy(struct Eve_Texture* self);

struct Eve_Texture*  Eve_Texture_NewFromImage(const char* path);
void                 Eve_Texture_Delete(struct Eve_Texture* self);

Eve_Int Eve_Texture_GetWidth(struct Eve_Texture* self);
Eve_Int Eve_Texture_GetHeight(struct Eve_Texture* self);


Eve_UInt Eve_CurrentWidth();
Eve_UInt Eve_CurrentHeight();

void Eve_PushFrame();
void Eve_PopFrame();

void Eve_TranslateI2(Eve_Int x, Eve_Int y);

void Eve_SetColorRGBA(u8 r, u8 g, u8 b, u8 a);
void Eve_SetColorRGB(u8 r, u8 g, u8 b);

void Eve_Clip(Eve_Int x, Eve_Int y, Eve_Int w, Eve_Int h);

void Eve_DrawLine(Eve_Int x1, Eve_Int y1, Eve_Int x2, Eve_Int y2);
void Eve_DrawArrow(Eve_Int x1, Eve_Int y1, Eve_Int x2, Eve_Int y2);

void Eve_DrawRect(Eve_Int x, Eve_Int y, Eve_Int w, Eve_Int h);
void Eve_FillRect(Eve_Int x, Eve_Int y, Eve_Int w, Eve_Int h);

void Eve_DrawText(const char* text, Eve_Int x, Eve_Int y, struct Eve_Font* font);
void Eve_DrawChar(Eve_UInt c, Eve_Int x, Eve_Int y, struct Eve_Font* font);

void Eve_DrawTexture(struct Eve_Texture* texture, Eve_Int x, Eve_Int y);

void Eve_Clear();
void Eve_Render();


bool Eve_PollEvent();

bool Eve_IsEventQuit();
bool Eve_IsEventKeyDown();
bool Eve_IsEventKeyUp();
bool Eve_IsEventMouseMotion();
bool Eve_IsEventMouseButtonDown();
bool Eve_IsEventMouseButtonUp();
bool Eve_IsEventMouseWheel();

Eve_UInt Eve_GetEventMouseX();
Eve_UInt Eve_GetEventMouseY();

Eve_UInt Eve_GetEventMouseButtonID();

Eve_Int Eve_GetEventMouseScrollX();
Eve_Int Eve_GetEventMouseScrollY();

Eve_UInt Eve_GetEventKeySym();
Eve_UInt Eve_GetEventKeyScancode();
Eve_UInt Eve_GetEventKeyMod();


Eve_UInt Eve_GetTextWidth(const char* text);
Eve_UInt Eve_GetTextHeight(const char* text);

Eve_UInt Eve_GetCharWidth(Eve_UInt c);
Eve_UInt Eve_GetCharHeight(Eve_UInt c);

void Eve_Delay(Eve_UInt ms);


void Eve_Init(const char* default_font_path, Eve_UInt default_font_size);
void Eve_Terminate();

#endif /* EVE_H */

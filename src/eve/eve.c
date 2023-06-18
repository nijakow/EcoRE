
#include "eve.h"



struct Eve_Color {
    uint32_t rgba;
};

struct Eve_Color Eve_Color_FromRGBA(u8 r, u8 g, u8 b, u8 a) {
    struct Eve_Color color;
    color.rgba = (r << 24) | (g << 16) | (b << 8) | a;
    return color;
}

struct Eve_Color Eve_Color_FromRGB(u8 r, u8 g, u8 b) {
    return Eve_Color_FromRGBA(r, g, b, 255);
}

struct Eve_Color Eve_Color_ToRGBA(struct Eve_Color color, u8* r, u8* g, u8* b, u8* a) {
    *r = (color.rgba >> 24) & 0xFF;
    *g = (color.rgba >> 16) & 0xFF;
    *b = (color.rgba >>  8) & 0xFF;
    *a = (color.rgba >>  0) & 0xFF;
    return color;
}

struct Eve_Color Eve_Color_ToRGB(struct Eve_Color color, u8* r, u8* g, u8* b) {
    u8 a;
    Eve_Color_ToRGBA(color, r, g, b, &a);
    return color;
}



struct Eve_Rect {
    Eve_Int x;
    Eve_Int y;
    Eve_Int w;
    Eve_Int h;
};

struct Eve_Rect Eve_Rect_FromXYWH(Eve_Int x, Eve_Int y, Eve_Int w, Eve_Int h) {
    struct Eve_Rect rect;
    rect.x = x;
    rect.y = y;
    rect.w = w;
    rect.h = h;
    return rect;
}


struct Eve_Frame {
    struct Eve_Color color;
    Eve_Int          x;
    Eve_Int          y;
    Eve_Int          xmax;
    Eve_Int          ymax;
};


struct Eve_FrameStack {
    struct Eve_Frame frames[256];
    Eve_Int          size;
};

void Eve_FrameStack_Create(struct Eve_FrameStack* stack) {
    stack->size = 0;
}

void Eve_FrameStack_Destroy(struct Eve_FrameStack* stack) {
    stack->size = 0;
}

void Eve_FrameStack_Reset(struct Eve_FrameStack* stack) {
    stack->size = 0;
}

void Eve_FrameStack_PushFrame(struct Eve_FrameStack* stack, struct Eve_Frame* frame) {
    stack->frames[stack->size] = *frame;
    stack->size++;
}

void Eve_FrameStack_PopFrame(struct Eve_FrameStack* stack, struct Eve_Frame* frame) {
    stack->size--;
    *frame = stack->frames[stack->size];
}


struct Eve_RenderState {
    struct SDL_Window*     window;
    struct SDL_Renderer*   renderer;

    TTF_Font*              font;

    struct Eve_FrameStack  frame_stack;
    struct Eve_Frame       frame;
    struct Eve_Color       color;
};

void Eve_RenderState_Create(struct Eve_RenderState* self, struct SDL_Window* window, struct SDL_Renderer* renderer, const char* font_path, Eve_Int font_size) {
    Eve_Int  window_width;
    Eve_Int  window_height;

    self->window   = window;
    self->renderer = renderer;

    self->font     = TTF_OpenFont(font_path, font_size);

    SDL_GetWindowSize(window, &window_width, &window_height);

    Eve_FrameStack_Create(&self->frame_stack);
    self->frame.color = Eve_Color_FromRGB(0, 0, 0);
    self->frame.x     = 0;
    self->frame.y     = 0;
    self->frame.xmax  = window_width;
    self->frame.ymax  = window_height;
}

void Eve_RenderState_Reset(struct Eve_RenderState* self) {
    Eve_Int  window_width;
    Eve_Int  window_height;

    SDL_GetWindowSize(self->window, &window_width, &window_height);

    Eve_FrameStack_Reset(&self->frame_stack);
    self->frame.color = Eve_Color_FromRGB(0, 0, 0);
    self->frame.x     = 0;
    self->frame.y     = 0;
    self->frame.xmax  = window_width;
    self->frame.ymax  = window_height;
}

void Eve_RenderState_PushFrame(struct Eve_RenderState* self) {
    Eve_FrameStack_PushFrame(&self->frame_stack, &self->frame);
}

void Eve_RenderState_PopFrame(struct Eve_RenderState* self) {
    Eve_FrameStack_PopFrame(&self->frame_stack, &self->frame);
}

void Eve_RenderState_TranslateI2(struct Eve_RenderState* self, Eve_Int x, Eve_Int y) {
    self->frame.x += x;
    self->frame.y += y;
}

void Eve_RenderState_SetColor(struct Eve_RenderState* self, struct Eve_Color color) {
    u8 r, g, b, a;

    self->frame.color = color;
    Eve_Color_ToRGBA(color, &r, &g, &b, &a);
    SDL_SetRenderDrawColor(self->renderer, r, g, b, a);
}

struct SDL_Rect Eve_RenderState_GetCurrentRect(struct Eve_RenderState* self) {
    struct SDL_Rect rect;
    rect.x = self->frame.x;
    rect.y = self->frame.y;
    rect.w = self->frame.xmax - self->frame.x;
    rect.h = self->frame.ymax - self->frame.y;
    return rect;
}

void Eve_RenderState_Clip(struct Eve_RenderState* self) {
    struct SDL_Rect rect;
    
    rect = Eve_RenderState_GetCurrentRect(self);
    SDL_RenderSetClipRect(self->renderer, &rect);
}

void Eve_RenderState_DrawRect(struct Eve_RenderState* self) {
    struct SDL_Rect rect;
    
    rect = Eve_RenderState_GetCurrentRect(self);
    SDL_RenderDrawRect(self->renderer, &rect);
}

void Eve_RenderState_FillRect(struct Eve_RenderState* self) {
    struct SDL_Rect rect;
    
    rect = Eve_RenderState_GetCurrentRect(self);
    SDL_RenderFillRect(self->renderer, &rect);
}

void Eve_RenderState_DrawText(struct Eve_RenderState* self, const char* text) {
    struct SDL_Rect rect;
    SDL_Surface*    surface;
    SDL_Texture*    texture;
    SDL_Color       color;
    SDL_Rect        dstrect;

    rect = Eve_RenderState_GetCurrentRect(self);
    surface = TTF_RenderText_Solid(self->font, text, color);
    texture = SDL_CreateTextureFromSurface(self->renderer, surface);
    dstrect = rect;
    SDL_RenderCopy(self->renderer, texture, NULL, &dstrect);
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
}



struct Eve_RenderState EVE_DEFAULT_RENDER_STATE;


void Eve_PushFrame() {
    Eve_RenderState_PushFrame(&EVE_DEFAULT_RENDER_STATE);
}

void Eve_PopFrame() {
    Eve_RenderState_PopFrame(&EVE_DEFAULT_RENDER_STATE);
}

void Eve_TranslateI2(Eve_Int x, Eve_Int y) {
    Eve_RenderState_TranslateI2(&EVE_DEFAULT_RENDER_STATE, x, y);
}

void Eve_SetColorRGBA(u8 r, u8 g, u8 b, u8 a) {
    Eve_RenderState_SetColor(&EVE_DEFAULT_RENDER_STATE, Eve_Color_FromRGBA(r, g, b, a));
}

void Eve_SetColorRGB(u8 r, u8 g, u8 b) {
    Eve_RenderState_SetColor(&EVE_DEFAULT_RENDER_STATE, Eve_Color_FromRGB(r, g, b));
}

void Eve_Clip(Eve_Int x, Eve_Int y, Eve_Int w, Eve_Int h) {
    Eve_RenderState_Clip(&EVE_DEFAULT_RENDER_STATE);
}

void Eve_DrawRect(Eve_Int x, Eve_Int y, Eve_Int w, Eve_Int h) {
    Eve_RenderState_DrawRect(&EVE_DEFAULT_RENDER_STATE);
}

void Eve_FillRect(Eve_Int x, Eve_Int y, Eve_Int w, Eve_Int h) {
    Eve_RenderState_FillRect(&EVE_DEFAULT_RENDER_STATE);
}

void Eve_DrawText(const char* text, Eve_Int x, Eve_Int y) {
    // Implementation for drawing text using SDL_ttf
}

void Eve_Init() {
}

void Eve_Terminate() {
}


#include "eve.h"



struct Eve_Color {
    u32 rgba;
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


struct Eve_Frame {
    struct Eve_Color color;
    struct Eve_Rect  rect;
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

void Eve_FrameStack_PushFrame(struct Eve_FrameStack* stack, struct Eve_Frame* frame) {
    stack->frames[stack->size] = *frame;
    stack->size++;
}

void Eve_FrameStack_PopFrame(struct Eve_FrameStack* stack, struct Eve_Frame* frame) {
    stack->size--;
    *frame = stack->frames[stack->size];
}


struct Eve_RenderState {
    struct Eve_FrameStack frame_stack;
    struct Eve_Frame      frame;
    struct Eve_Color      color;
};


void Eve_RenderState_PushFrame(struct Eve_RenderState* self) {
    Eve_FrameStack_PushFrame(&self->frame_stack, &self->frame);
}

void Eve_RenderState_PopFrame(struct Eve_RenderState* self) {
    Eve_FrameStack_PopFrame(&self->frame_stack, &self->frame);
}

void Eve_RenderState_TranslateI2(struct Eve_RenderState* self, Eve_Int x, Eve_Int y) {
    self->frame.rect.x += x;
    self->frame.rect.y += y;
}

void Eve_RenderState_Clip(struct Eve_RenderState* self) {
    // Implementation for clipping
}

void Eve_RenderState_SetColor(struct Eve_RenderState* self, struct Eve_Color color) {
    self->frame.color = color;
}



void Eve_PushFrame() {
    // Implementation for matrix stack
}

void Eve_PopFrame() {
    // Implementation for matrix stack
}

void Eve_SetColorRGBA(u8 r, u8 g, u8 b, u8 a) {
    // Implementation for setting color
}

void Eve_DrawRect(Eve_Int x, Eve_Int y, Eve_Int w, Eve_Int h) {
    // Implementation for drawing rectangle
}

void Eve_DrawText(const char* text, Eve_Int x, Eve_Int y) {
    // Implementation for drawing text using SDL_ttf
}

void Eve_Clip(Eve_Int x, Eve_Int y, Eve_Int w, Eve_Int h) {
    // Implementation for clipping
}

void Eve_Init() {
}

void Eve_Terminate() {
}

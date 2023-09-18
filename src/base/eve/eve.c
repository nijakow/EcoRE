
#include "eve.h"



static size_t Eve_EncodeAsUtf8(Eve_UInt codepoint, char* buffer)
{
    if (codepoint <= 0x7F)
    {
        buffer[0] = codepoint;
        return 1;
    }
    else if (codepoint <= 0x7FF)
    {
        buffer[0] = 0xC0 | (codepoint >> 6);
        buffer[1] = 0x80 | (codepoint & 0x3F);
        return 2;
    }
    else if (codepoint <= 0xFFFF)
    {
        buffer[0] = 0xE0 | (codepoint >> 12);
        buffer[1] = 0x80 | ((codepoint >> 6) & 0x3F);
        buffer[2] = 0x80 | (codepoint & 0x3F);
        return 3;
    }
    else if (codepoint <= 0x10FFFF)
    {
        buffer[0] = 0xF0 | (codepoint >> 18);
        buffer[1] = 0x80 | ((codepoint >> 12) & 0x3F);
        buffer[2] = 0x80 | ((codepoint >> 6) & 0x3F);
        buffer[3] = 0x80 | (codepoint & 0x3F);
        return 4;
    }
    else
    {
        return 0;
    }
}



void Eve_Font_Create(struct Eve_Font* self, const char* path, Eve_UInt size)
{
    self->font = TTF_OpenFont(path, size);
}

void Eve_Font_CreateFromFont(struct Eve_Font* self, TTF_Font* font)
{
    self->font = font;
}

void Eve_Font_Destroy(struct Eve_Font* self)
{
    TTF_CloseFont(self->font);
}


struct Eve_Font*  Eve_Font_New(const char* path, Eve_UInt size)
{
    struct Eve_Font*  font;

    font = malloc(sizeof(struct Eve_Font));

    if (font != NULL)
    {
        Eve_Font_Create(font, path, size);
    }

    return font;
}

struct Eve_Font*  Eve_Font_NewFromFont(TTF_Font* font)
{
    struct Eve_Font*  eve_font;

    eve_font = malloc(sizeof(struct Eve_Font));

    if (eve_font != NULL)
    {
        Eve_Font_CreateFromFont(eve_font, font);
    }

    return eve_font;
}

void Eve_Font_Delete(struct Eve_Font* self)
{
    Eve_Font_Destroy(self);
    free(self);
}

Eve_UInt Eve_Font_GetTextWidth(struct Eve_Font* self, const char* text)
{
    int w;
    int h;

    TTF_SizeUTF8(self->font, text, &w, &h);

    return w;
}

Eve_UInt Eve_Font_GetTextHeight(struct Eve_Font* self, const char* text)
{
    int w;
    int h;

    TTF_SizeUTF8(self->font, text, &w, &h);

    return h;
}


Eve_UInt Eve_Font_GetCharWidth(struct Eve_Font* self, Eve_UInt c)
{
    char buffer[5];

    buffer[Eve_EncodeAsUtf8(c, buffer)] = '\0';

    return Eve_Font_GetTextWidth(self, buffer);
}

Eve_UInt Eve_Font_GetCharHeight(struct Eve_Font* self, Eve_UInt c)
{
    char buffer[5];

    buffer[Eve_EncodeAsUtf8(c, buffer)] = '\0';

    return Eve_Font_GetTextHeight(self, buffer);
}


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

SDL_Color Eve_Color_ToSDL(struct Eve_Color color) {
    u8 r, g, b, a;
    Eve_Color_ToRGBA(color, &r, &g, &b, &a);
    SDL_Color sdl_color;
    sdl_color.r = r;
    sdl_color.g = g;
    sdl_color.b = b;
    sdl_color.a = a;
    return sdl_color;
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


uint32_t Eve_HashText(const char* text) {
    uint32_t hash = 0;
    while (*text) {
        hash = (hash << 7) + (hash << 1) + hash + *text;
        text++;
    }
    return hash;
}

struct Eve_TextCacheNode {
    struct Eve_TextCacheNode*   next_global;
    unsigned int                age_created;
    struct Eve_TextCacheNode*   next;
    struct Eve_TextCacheNode**  prev;
    char*                       text;
    struct Eve_Color            color;
    TTF_Font*                   font;
    SDL_Texture*                texture;
    Eve_UInt                    width;
    Eve_UInt                    height;
};

void Eve_TextCacheNode_Unlink(struct Eve_TextCacheNode* self);

void Eve_TextCacheNode_Create(struct Eve_TextCacheNode* self, struct Eve_TextCacheNode** insert_at, unsigned int age_created, const char* text, struct Eve_Color color, TTF_Font* font, SDL_Texture* texture, Eve_UInt width, Eve_UInt height) {
    self->next_global = *insert_at;
    *insert_at        = self;
    self->age_created = age_created;
    self->next        = NULL;
    self->prev        = NULL;
    self->text        = strdup(text);
    self->color       = color;
    self->font        = font;
    self->texture     = texture;
    self->width       = width;
    self->height      = height;
}

void Eve_TextCacheNode_Destroy(struct Eve_TextCacheNode* self) {
    Eve_TextCacheNode_Unlink(self);
    free(self->text);
    SDL_DestroyTexture(self->texture);
}

void Eve_TextCacheNode_Delete(struct Eve_TextCacheNode* self) {
    Eve_TextCacheNode_Destroy(self);
    free(self);
}

void Eve_TextCacheNode_Link(struct Eve_TextCacheNode* self, struct Eve_TextCacheNode** list) {
    self->next = *list;
    if (*list != NULL) {
        (*list)->prev = &self->next;
    }
    *list = self;
    self->prev = list;
}

void Eve_TextCacheNode_Unlink(struct Eve_TextCacheNode* self) {
    if (self->next != NULL) {
        self->next->prev = self->prev;
    }
    if (self->prev != NULL) {
        *self->prev = self->next;
    }
}


struct Eve_TextCacheBucket {
    struct Eve_TextCacheNode*  head;
};

void Eve_TextCacheBucket_Create(struct Eve_TextCacheBucket* self) {
    self->head = NULL;
}

void Eve_TextCacheBucket_Destroy(struct Eve_TextCacheBucket* self) {
    struct Eve_TextCacheNode*  node;
    struct Eve_TextCacheNode*  next;

    // FIXME, XXX: This might crash since the deletion function unlinks the node!
    for (node = self->head; node != NULL; node = next) {
        next = node->next;
        Eve_TextCacheNode_Delete(node);
    }
}


/*
 * A hash map of text cache nodes.
 */
struct Eve_TextCache {
    struct Eve_TextCacheBucket  buckets[4096];
    unsigned int                bucket_count;

    struct Eve_TextCacheNode*   all_nodes;
    struct Eve_TextCacheNode**  insert_at;
    unsigned int                age;
};

void Eve_TextCache_Create(struct Eve_TextCache* self) {
    self->bucket_count = 4096;
    for (unsigned int i = 0; i < self->bucket_count; i++) {
        Eve_TextCacheBucket_Create(&self->buckets[i]);
    }
    self->all_nodes = NULL;
    self->insert_at = &self->all_nodes;
    self->age       = 0;
}

void Eve_TextCache_Destroy(struct Eve_TextCache* self) {
    for (unsigned int i = 0; i < self->bucket_count; i++) {
        Eve_TextCacheBucket_Destroy(&self->buckets[i]);
    }
}

struct Eve_TextCacheNode* Eve_TextCache_FindOrCreate(struct Eve_TextCache* self, const char* text, struct Eve_Color color, SDL_Renderer* renderer, TTF_Font* font) {
    struct Eve_TextCacheBucket*  bucket;
    struct Eve_TextCacheNode*    node;
    uint32_t                     hash;
    SDL_Surface*                 surface;
    SDL_Texture*                 texture;
    SDL_Color                    sdl_color;
    int                          width;
    int                          height;

    if (text == NULL || text[0] == '\0') {
        return NULL;
    }
    
    hash   = Eve_HashText(text);
    bucket = &self->buckets[hash % self->bucket_count];
    node   = bucket->head;

    while (node != NULL) {
        if (strcmp(node->text, text) == 0 && node->color.rgba == color.rgba && node->font == font) {
            return node;
        }
        node = node->next;
    }

    sdl_color = Eve_Color_ToSDL(color);
    surface   = TTF_RenderUTF8_Blended(font, text, sdl_color);
    
    if (surface == NULL) {
        return NULL;
    }

    texture   = SDL_CreateTextureFromSurface(renderer, surface);
    width     = surface->w;
    height    = surface->h;
    SDL_FreeSurface(surface);

    node = malloc(sizeof(struct Eve_TextCacheNode));
    Eve_TextCacheNode_Create(node, self->insert_at, self->age, text, color, font, texture, width, height);
    Eve_TextCacheNode_Link(node, &bucket->head);
    self->insert_at = &node->next_global;
    return node;
}

void Eve_TextCache_Tick(struct Eve_TextCache* self) {
    struct Eve_TextCacheNode*  next;

    if (self->all_nodes != NULL) {
        self->age++;

        while (self->all_nodes != NULL && self->all_nodes->age_created + 1024 < self->age) {
            next = self->all_nodes->next_global;
            Eve_TextCacheNode_Delete(self->all_nodes);
            self->all_nodes = next;
        }

        if (self->all_nodes == NULL) {
            self->insert_at = &self->all_nodes;
        }
    }
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
    struct SDL_Texture*    texture;

    struct Eve_Font*       font;

    SDL_Event              event;

    struct Eve_FrameStack  frame_stack;
    struct Eve_Frame       frame;
    struct Eve_Color       color;

    struct Eve_TextCache   text_cache;
};

void Eve_RenderState_Create(struct Eve_RenderState* self, struct SDL_Window* window, struct SDL_Renderer* window_renderer, TTF_Font* font) {
    Eve_Int  window_width;
    Eve_Int  window_height;

    self->window   = window;
    self->renderer = window_renderer;

    self->font     = Eve_Font_NewFromFont(font);

    SDL_GetWindowSize(window, &window_width, &window_height);

    self->texture  = SDL_CreateTexture(window_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, window_width, window_height);

    SDL_SetTextureBlendMode(self->texture, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawBlendMode(window_renderer, SDL_BLENDMODE_BLEND);

    Eve_FrameStack_Create(&self->frame_stack);
    self->frame.color = Eve_Color_FromRGB(0, 0, 0);
    self->frame.x     = 0;
    self->frame.y     = 0;
    self->frame.xmax  = window_width;
    self->frame.ymax  = window_height;

    Eve_TextCache_Create(&self->text_cache);
}

void Eve_RenderState_Destroy(struct Eve_RenderState* self) {
    Eve_TextCache_Destroy(&self->text_cache);

    Eve_FrameStack_Destroy(&self->frame_stack);

    Eve_Font_Delete(self->font);

    SDL_DestroyRenderer(self->renderer);
    SDL_DestroyWindow(self->window);
}

void Eve_RenderState_Reset(struct Eve_RenderState* self) {
    Eve_Int  window_width;
    Eve_Int  window_height;

    SDL_RenderClear(self->renderer);

    SDL_GetWindowSize(self->window, &window_width, &window_height);

    Eve_FrameStack_Reset(&self->frame_stack);
    self->frame.color = Eve_Color_FromRGB(0, 0, 0);
    self->frame.x     = 0;
    self->frame.y     = 0;
    self->frame.xmax  = window_width;
    self->frame.ymax  = window_height;
}

Eve_UInt Eve_RenderState_CurrentWidth(struct Eve_RenderState* self) {
    return self->frame.xmax - self->frame.x;
}

Eve_UInt Eve_RenderState_CurrentHeight(struct Eve_RenderState* self) {
    return self->frame.ymax - self->frame.y;
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
    SDL_SetTextureAlphaMod(self->texture, a);
}

struct SDL_Rect Eve_RenderState_GetCurrentRect(struct Eve_RenderState* self) {
    struct SDL_Rect rect;
    rect.x = self->frame.x;
    rect.y = self->frame.y;
    rect.w = self->frame.xmax - self->frame.x;
    rect.h = self->frame.ymax - self->frame.y;
    return rect;
}

void Eve_RenderState_Clip(struct Eve_RenderState* self, Eve_Int x, Eve_Int y, Eve_Int w, Eve_Int h) {
    struct SDL_Rect rect;

    rect = Eve_RenderState_GetCurrentRect(self);

    rect.x += x;
    rect.y += y;
    rect.w  = w;
    rect.h  = h;

    SDL_RenderSetClipRect(self->renderer, &rect);
}

void Eve_RenderState_DrawPoint(struct Eve_RenderState* self, Eve_Int x, Eve_Int y) {
    SDL_RenderDrawPoint(self->renderer, x, y);
}

void Eve_RenderState_DrawLine(struct Eve_RenderState* self, Eve_Int x1, Eve_Int y1, Eve_Int x2, Eve_Int y2) {
    SDL_RenderDrawLine(self->renderer, x1, y1, x2, y2);
}

void Eve_RenderState_DrawArrow(struct Eve_RenderState* self, Eve_Int x1, Eve_Int y1, Eve_Int x2, Eve_Int y2) {
    float angle;

    angle = atan2(y2 - y1, x2 - x1);
    
    SDL_RenderDrawLine(self->renderer, x1, y1, x2, y2);

    SDL_RenderDrawLine(self->renderer, x2, y2, x2 - 10 * cos(angle - M_PI / 6), y2 - 10 * sin(angle - M_PI / 6));
    SDL_RenderDrawLine(self->renderer, x2, y2, x2 - 10 * cos(angle + M_PI / 6), y2 - 10 * sin(angle + M_PI / 6));
}

void Eve_RenderState_DrawRect(struct Eve_RenderState* self, Eve_Int x, Eve_Int y, Eve_UInt w, Eve_UInt h) {
    struct SDL_Rect rect;

    rect.x = x;
    rect.y = y;
    rect.w = w;
    rect.h = h;

    SDL_RenderDrawRect(self->renderer, &rect);
}

void Eve_RenderState_FillRect(struct Eve_RenderState* self, Eve_Int x, Eve_Int y, Eve_UInt w, Eve_UInt h) {
    struct SDL_Rect rect;

    rect.x = x;
    rect.y = y;
    rect.w = w;
    rect.h = h;

    SDL_RenderFillRect(self->renderer, &rect);
}

void Eve_RenderState_BlurRect(struct Eve_RenderState* self, Eve_Int x, Eve_Int y, Eve_UInt w, Eve_UInt h) {
    struct SDL_Rect rect;

    rect.x = x;
    rect.y = y;
    rect.w = w;
    rect.h = h;

    /*
     * Gaussian blur on the texture.
     * Iterate over all pixels of self->texture and calculate the average of the surrounding pixels.
     */
    SDL_Color original_pixels[w * h];
    SDL_RenderReadPixels(self->renderer, &rect, SDL_PIXELFORMAT_ABGR8888, original_pixels, w * sizeof(SDL_Color));

    SDL_Rect rect2;

    for (int xx = 0; xx < w; xx += 3) {
        for (int yy = 0; yy < h; yy += 3) {
            unsigned int ww = (w - xx) < 3 ? (w - xx) : 3;
            unsigned int hh = (h - yy) < 3 ? (h - yy) : 3;
            
            unsigned int r_sum = 0;
            unsigned int g_sum = 0;
            unsigned int b_sum = 0;

            for (int dx = 0; dx < ww; dx++) {
                if (xx + dx >= w) {
                    continue;
                }

                for (int dy = 0; dy < hh; dy++) {
                    if (yy + dy >= h) {
                        continue;
                    }

                    SDL_Color* original_pixel = &original_pixels[(xx + dx) + (yy + dy) * w];

                    r_sum += original_pixel->r;
                    g_sum += original_pixel->g;
                    b_sum += original_pixel->b;
                }
            }

            int count = ww * hh;

            SDL_SetRenderDrawColor(self->renderer, r_sum / count, g_sum / count, b_sum / count, 255);
            // SDL_RenderDrawPoint(self->renderer, x + xx, y + yy);

            rect2.x = x + xx;
            rect2.y = y + yy;
            rect2.w = ww;
            rect2.h = hh;

            SDL_RenderFillRect(self->renderer, &rect2);
        }
    }
}

void Eve_RenderState_DrawText(struct Eve_RenderState* self, const char* text, Eve_Int x, Eve_Int y, struct Eve_Font* font) {
    struct Eve_TextCacheNode*  node;
    SDL_Rect                   rect;

    node = Eve_TextCache_FindOrCreate(&self->text_cache, text, self->frame.color, self->renderer, font->font);

    if (node != NULL) {
        rect.x = x;
        rect.y = y;
        rect.w = node->width;
        rect.h = node->height;

        SDL_RenderCopy(self->renderer, node->texture, NULL, &rect);
    }
}

void Eve_RenderState_DrawTexture(struct Eve_RenderState* self, struct Eve_Texture* texture, Eve_Int x, Eve_Int y) {
    SDL_Rect rect;

    rect.x = x;
    rect.y = y;
    rect.w = texture->width;
    rect.h = texture->height;

    SDL_RenderCopy(self->renderer, texture->texture, NULL, &rect);
}

void Eve_RenderState_Clear(struct Eve_RenderState* self) {
    SDL_RenderClear(self->renderer);
}

void Eve_RenderState_Render(struct Eve_RenderState* self) {
    SDL_RenderPresent(self->renderer);
    Eve_TextCache_Tick(&self->text_cache);
}

bool Eve_RenderState_PollEvent(struct Eve_RenderState* self) {
    return SDL_PollEvent(&self->event) != 0;
}

bool Eve_RenderState_IsEventQuit(struct Eve_RenderState* self) {
    return self->event.type == SDL_QUIT;
}

bool Eve_RenderState_IsEventKeyDown(struct Eve_RenderState* self) {
    return self->event.type == SDL_KEYDOWN;
}

bool Eve_RenderState_IsEventKeyUp(struct Eve_RenderState* self) {
    return self->event.type == SDL_KEYUP;
}

bool Eve_RenderState_IsEventMouseMotion(struct Eve_RenderState* self) {
    return self->event.type == SDL_MOUSEMOTION;
}

bool Eve_RenderState_IsEventMouseButtonDown(struct Eve_RenderState* self) {
    return self->event.type == SDL_MOUSEBUTTONDOWN;
}

bool Eve_RenderState_IsEventMouseButtonUp(struct Eve_RenderState* self) {
    return self->event.type == SDL_MOUSEBUTTONUP;
}

bool Eve_RenderState_IsEventMouseWheel(struct Eve_RenderState* self) {
    return self->event.type == SDL_MOUSEWHEEL;
}

Eve_UInt Eve_RenderState_GetEventMouseX(struct Eve_RenderState* self) {
    return self->event.motion.x;
}

Eve_UInt Eve_RenderState_GetEventMouseY(struct Eve_RenderState* self) {
    return self->event.motion.y;
}

Eve_UInt Eve_RenderState_GetEventMouseButtonID(struct Eve_RenderState* self) {
    return self->event.button.button;
}

Eve_Int Eve_RenderState_GetEventMouseScrollX(struct Eve_RenderState* self) {
    int flip_factor = (self->event.wheel.direction == SDL_MOUSEWHEEL_FLIPPED) ? -1 : 1;
    if (!(SDL_GetModState() & KMOD_LSHIFT)) {
        return self->event.wheel.x * flip_factor;
    } else {
        return self->event.wheel.y * flip_factor;
    }
}

Eve_Int Eve_RenderState_GetEventMouseScrollY(struct Eve_RenderState* self) {
    int flip_factor = (self->event.wheel.direction == SDL_MOUSEWHEEL_FLIPPED) ? -1 : 1;
    if (!(SDL_GetModState() & KMOD_LSHIFT)) {
        return self->event.wheel.y * flip_factor;
    } else {
        return self->event.wheel.x * flip_factor;
    }
}

Eve_UInt Eve_RenderState_GetEventKeySym(struct Eve_RenderState* self) {
    return self->event.key.keysym.sym;
}

Eve_UInt Eve_RenderState_GetEventKeyScancode(struct Eve_RenderState* self) {
    return self->event.key.keysym.scancode;
}

Eve_UInt Eve_RenderState_GetEventKeyMod(struct Eve_RenderState* self) {
    return self->event.key.keysym.mod;
}




struct Eve_RenderState EVE_DEFAULT_RENDER_STATE;

struct Eve_Font* Eve_GetDefaultFont() {
    return EVE_DEFAULT_RENDER_STATE.font;
}

Eve_UInt Eve_CurrentWidth() {
    return Eve_RenderState_CurrentWidth(&EVE_DEFAULT_RENDER_STATE);
}

Eve_UInt Eve_CurrentHeight() {
    return Eve_RenderState_CurrentHeight(&EVE_DEFAULT_RENDER_STATE);
}

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
    Eve_RenderState_Clip(&EVE_DEFAULT_RENDER_STATE, x, y, w, h);
}

void Eve_DrawPoint(Eve_Int x, Eve_Int y) {
    Eve_RenderState_DrawPoint(&EVE_DEFAULT_RENDER_STATE, x, y);
}

void Eve_DrawLine(Eve_Int x1, Eve_Int y1, Eve_Int x2, Eve_Int y2) {
    Eve_RenderState_DrawLine(&EVE_DEFAULT_RENDER_STATE, x1, y1, x2, y2);
}

void Eve_DrawArrow(Eve_Int x1, Eve_Int y1, Eve_Int x2, Eve_Int y2) {
    Eve_RenderState_DrawArrow(&EVE_DEFAULT_RENDER_STATE, x1, y1, x2, y2);
}

void Eve_DrawRect(Eve_Int x, Eve_Int y, Eve_Int w, Eve_Int h) {
    Eve_RenderState_DrawRect(&EVE_DEFAULT_RENDER_STATE, x, y, w, h);
}

void Eve_FillRect(Eve_Int x, Eve_Int y, Eve_Int w, Eve_Int h) {
    Eve_RenderState_FillRect(&EVE_DEFAULT_RENDER_STATE, x, y, w, h);
}

void Eve_BlurRect(Eve_Int x, Eve_Int y, Eve_Int w, Eve_Int h) {
    Eve_RenderState_BlurRect(&EVE_DEFAULT_RENDER_STATE, x, y, w, h);
}

void Eve_DrawText(const char* text, Eve_Int x, Eve_Int y, struct Eve_Font* font) {
    Eve_RenderState_DrawText(&EVE_DEFAULT_RENDER_STATE, text, x, y, font);
}

void Eve_DrawChar(Eve_UInt c, Eve_Int x, Eve_Int y, struct Eve_Font* font) {
    char buffer[5];

    buffer[Eve_EncodeAsUtf8(c, buffer)] = '\0';

    Eve_DrawText(buffer, x, y, font);
}

void Eve_DrawTexture(struct Eve_Texture* texture, Eve_Int x, Eve_Int y) {
    Eve_RenderState_DrawTexture(&EVE_DEFAULT_RENDER_STATE, texture, x, y);
}

void Eve_Clear() {
    Eve_RenderState_Clear(&EVE_DEFAULT_RENDER_STATE);
}

void Eve_Render() {
    Eve_RenderState_Render(&EVE_DEFAULT_RENDER_STATE);
}

bool Eve_PollEvent() {
    return Eve_RenderState_PollEvent(&EVE_DEFAULT_RENDER_STATE);
}

bool Eve_IsEventQuit() {
    return Eve_RenderState_IsEventQuit(&EVE_DEFAULT_RENDER_STATE);
}

bool Eve_IsEventKeyDown() {
    return Eve_RenderState_IsEventKeyDown(&EVE_DEFAULT_RENDER_STATE);
}

bool Eve_IsEventKeyUp() {
    return Eve_RenderState_IsEventKeyUp(&EVE_DEFAULT_RENDER_STATE);
}

bool Eve_IsEventMouseMotion() {
    return Eve_RenderState_IsEventMouseMotion(&EVE_DEFAULT_RENDER_STATE);
}

bool Eve_IsEventMouseButtonDown() {
    return Eve_RenderState_IsEventMouseButtonDown(&EVE_DEFAULT_RENDER_STATE);
}

bool Eve_IsEventMouseButtonUp() {
    return Eve_RenderState_IsEventMouseButtonUp(&EVE_DEFAULT_RENDER_STATE);
}

bool Eve_IsEventMouseWheel()
{
    return Eve_RenderState_IsEventMouseWheel(&EVE_DEFAULT_RENDER_STATE);
}

Eve_UInt Eve_GetEventMouseX() {
    return Eve_RenderState_GetEventMouseX(&EVE_DEFAULT_RENDER_STATE);
}

Eve_UInt Eve_GetEventMouseY() {
    return Eve_RenderState_GetEventMouseY(&EVE_DEFAULT_RENDER_STATE);
}

Eve_UInt Eve_GetEventMouseButtonID() {
    return Eve_RenderState_GetEventMouseButtonID(&EVE_DEFAULT_RENDER_STATE);
}

Eve_Int Eve_GetEventMouseScrollX() {
    return Eve_RenderState_GetEventMouseScrollX(&EVE_DEFAULT_RENDER_STATE);
}

Eve_Int Eve_GetEventMouseScrollY() {
    return Eve_RenderState_GetEventMouseScrollY(&EVE_DEFAULT_RENDER_STATE);
}

Eve_UInt Eve_GetEventKeySym() {
    return Eve_RenderState_GetEventKeySym(&EVE_DEFAULT_RENDER_STATE);
}

Eve_UInt Eve_GetEventKeyScancode() {
    return Eve_RenderState_GetEventKeyScancode(&EVE_DEFAULT_RENDER_STATE);
}

Eve_UInt Eve_GetEventKeyMod() {
    return Eve_RenderState_GetEventKeyMod(&EVE_DEFAULT_RENDER_STATE);
}


Eve_UInt Eve_GetTextWidth(const char* text) {
    int w;
    int h;

    TTF_SizeUTF8(EVE_DEFAULT_RENDER_STATE.font->font, text, &w, &h);

    return w;
}

Eve_UInt Eve_GetTextHeight(const char* text) {
    int w;
    int h;

    TTF_SizeUTF8(EVE_DEFAULT_RENDER_STATE.font->font, text, &w, &h);

    return h;
}

Eve_UInt Eve_GetCharWidth(Eve_UInt c) {
    char buffer[5];

    buffer[Eve_EncodeAsUtf8(c, buffer)] = '\0';

    return Eve_GetTextWidth(buffer);
}

Eve_UInt Eve_GetCharHeight(Eve_UInt c) {
    char buffer[5];

    buffer[Eve_EncodeAsUtf8(c, buffer)] = '\0';

    return Eve_GetTextHeight(buffer);
}



void Eve_Texture_CreateFromImage(struct Eve_Texture* self, const char* path) {
    self->texture = IMG_LoadTexture(EVE_DEFAULT_RENDER_STATE.renderer, path);
    SDL_QueryTexture(self->texture, NULL, NULL, &self->width, &self->height);
}

void Eve_Texture_Destroy(struct Eve_Texture* self) {
    SDL_DestroyTexture(self->texture);
}

struct Eve_Texture* Eve_Texture_NewFromImage(const char* path) {
    struct Eve_Texture*  texture;

    texture = malloc(sizeof(struct Eve_Texture));

    if (texture != NULL) {
        Eve_Texture_CreateFromImage(texture, path);
    }

    return texture;
}

void Eve_Texture_Delete(struct Eve_Texture* self) {
    Eve_Texture_Destroy(self);
    free(self);
}

Eve_Int Eve_Texture_GetWidth(struct Eve_Texture* self) {
    return self->width;
}

Eve_Int Eve_Texture_GetHeight(struct Eve_Texture* self) {
    return self->height;
}


void Eve_Delay(Eve_UInt ms) {
    SDL_Delay(ms);
}


void Eve_Init(const char* default_font_path, Eve_UInt default_font_size) {
    struct SDL_Window*    window;
    struct SDL_Renderer*  renderer;
    TTF_Font*             font;

    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();

    window   = SDL_CreateWindow("Eve", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1920, 1080, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    font = TTF_OpenFont(default_font_path, default_font_size);

    Eve_RenderState_Create(&EVE_DEFAULT_RENDER_STATE, window, renderer, font);
}

void Eve_Terminate() {
    Eve_RenderState_Destroy(&EVE_DEFAULT_RENDER_STATE);
    TTF_Quit();
    SDL_Quit();
}

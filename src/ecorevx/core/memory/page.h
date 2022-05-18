#ifndef ECO_CORE_MEMORY_PAGE_H
#define ECO_CORE_MEMORY_PAGE_H

#include "../../base/defs.h"

/*
 * The amount of bits osed for the offset of a pointer inside of its page
 */
#define Eco_PAGE_SHIFT (24)

/*
 * The size of a page, in bytes
 */
#define Eco_PAGE_SIZE (((Eco_Size_t) 1) << Eco_PAGE_SHIFT)

/*
 * Apply this mask to get the offset of a pointer inside of its page
 */
#define Eco_PAGE_MASK ((((Eco_Uintptr_t) 1) << Eco_PAGE_SHIFT) - 1)

/*
 * Apply this mask to get the base address of a page
 */
#define Eco_PAGEBASE_MASK ~((((Eco_Uintptr_t) 1) << Eco_PAGE_SHIFT) - 1)


struct Eco_PageInfo
{
    struct Eco_Page*  next;
    struct Eco_Page** prev;
    struct Eco_Page*  next_in_list;
    struct Eco_Page** prev_in_list;
};

struct Eco_Page
{
    void*                alloc;
    struct Eco_PageInfo* info;
};


/*
 * Returns the pointer to the page base associated with the pointer.
 * This is useful if we want to access data by page index.
 * 
 * The pointer can point anywhere on the page.
 */
static inline void* Eco_Page_Base(void* ptr)
{
    return (void*) (((Eco_Uintptr_t) ptr) & Eco_PAGEBASE_MASK);
}

/*
 * Returns the pointer to the page associated with the pointer.
 *
 * The pointer can point anywhere on the page.
 */
static inline struct Eco_Page* Eco_Page(void* ptr)
{
    return (struct Eco_Page*) Eco_Page_Base(ptr);
}

/*
 * Returns the pointer to the page info of a page.
 *
 * The pointer has to point to the page base.
 */
static inline struct Eco_PageInfo* Eco_Page_GetInfo(struct Eco_Page* self)
{
    return self->info;
}

/*
 * Returns the address of the heap area.
 *
 * The pointer has to point to the page base.
 */
static inline void* Eco_Page_GetHeapStart(void* base)
{
    /*
     * TODO: This calculation works, but at the moment there isn't
     *       any reserved space for the card markers. If we want to
     *       add those, this function has to be changed.
     */
    return ((char*) base) + sizeof(struct Eco_Page) + sizeof(struct Eco_PageInfo);
}

/*
 * Returns the pointer to the page info of the page this
 * pointer points to.
 * 
 * The pointer can point anywhere on the page.
 */
static inline struct Eco_PageInfo* Eco_PageInfo(void* ptr)
{
    return Eco_Page_GetInfo(Eco_Page(ptr));
}

/*
 * Returns the pointer to the end of a page.
 *
 * The pointer has to point to the page base.
 */
static inline void* Eco_Page_End(void* base)
{
    return ((char*) base) + Eco_PAGE_SIZE;
}

/*
 * Returns the pointer to the current allocation spot of a page.
 *
 * The pointer has to point to the page base.
 */
static inline void* Eco_Page_Here(struct Eco_Page* self)
{
    return self->alloc;
}

/*
 * Returns the pointer to the next page.
 *
 * The pointer has to point to the page base.
 */
static inline struct Eco_Page** Eco_Page_NextPage(struct Eco_Page* self)
{
    return &self->info->next;
}

/*
 * Returns the pointer to the previous page.
 *
 * The pointer has to point to the page base.
 */
static inline struct Eco_Page*** Eco_Page_PrevPage(struct Eco_Page* self)
{
    return &self->info->prev;
}

/*
 * Returns the pointer to the next page in this page list.
 *
 * The pointer has to point to the page base.
 */
static inline struct Eco_Page** Eco_Page_NextPageInList(struct Eco_Page* self)
{
    return &self->info->next_in_list;
}

/*
 * Returns the pointer to the previous page in this page list.
 *
 * The pointer has to point to the page base.
 */
static inline struct Eco_Page*** Eco_Page_PrevPageInList(struct Eco_Page* self)
{
    return &self->info->prev_in_list;
}


static inline Eco_Size_t Eco_Page_GetFreeSpace(struct Eco_Page* self)
{
    return (Eco_Page_End(self) - Eco_Page_Here(self));
}

static inline bool Eco_Page_HasSpaceFor(struct Eco_Page* self, Eco_Size_t size)
{
    return Eco_Page_GetFreeSpace(self) >= size;
}


struct Eco_Page* Eco_Page_New();
void             Eco_Page_Delete(struct Eco_Page*);

void*            Eco_Page_Alloc(struct Eco_Page*, Eco_Size_t);
void             Eco_Page_Clear(struct Eco_Page*);

void             Eco_Page_Unlink(struct Eco_Page*);
void             Eco_Page_Link(struct Eco_Page*, struct Eco_Page**);

#endif

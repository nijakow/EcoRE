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
};

struct Eco_Page
{
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
 * Returns the pointer to the page info of the page this
 * pointer points to.
 * 
 * The pointer can point anywhere on the page.
 */
static inline struct Eco_PageInfo* Eco_PageInfo(void* ptr)
{
    return Eco_Page_GetInfo(Eco_Page(ptr));
}

struct Eco_Page* Eco_Page_New();
void             Eco_Page_Delete(struct Eco_Page*);

#endif

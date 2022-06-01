#include <ecore/base/config.h>

#ifdef ECO_CONFIG_USE_LIBDWARF

#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

#include <ecore/util/libc.h>
#include <ecore/util/memory.h>

#include "dwarf.h"

static void Eco_DwarfDie_LoadRecursively(struct Eco_DwarfDie* die)
{
    while (die != NULL)
    {
        Eco_DwarfDie_LoadRecursively(Eco_DwarfDie_Child(die));
        die = Eco_DwarfDie_Sibling(die);
    }
}

static void Eco_Dwarf_Load(struct Eco_Dwarf* dwarf)
{
    struct Eco_DwarfDie*  die;
    struct Eco_DwarfDie*  die_next;

    die         = Eco_Dwarf_GetFirstDie(dwarf);
    dwarf->head = die;
    while (die != NULL)
    {
        Eco_DwarfDie_LoadRecursively(die);
        die_next         =  Eco_Dwarf_GetFirstDie(dwarf);
        die->sibling     =  die_next;
        if (die_next != NULL)
            die_next->holder = &die->sibling;
        die              =  die_next;
    }
}

bool Eco_Dwarf_Create(struct Eco_Dwarf* dwarf, const char* path)
{
    dwarf->dies = NULL;
    dwarf->fd   = open(path, O_RDONLY);

    if (dwarf->fd < 0) return false;

    if (dwarf_init(dwarf->fd, DW_DLC_READ, NULL, NULL, &dwarf->debug, NULL) == DW_DLV_OK) {
        Eco_Dwarf_Load(dwarf);
        return true;
    } else {
        close(dwarf->fd);
        return false;
    }
}

void Eco_Dwarf_Destroy(struct Eco_Dwarf* dwarf)
{
    while (dwarf->dies != NULL)
        Eco_DwarfDie_Delete(dwarf->dies);
    dwarf_finish(dwarf->debug, NULL);
    close(dwarf->fd);
}

static struct Eco_DwarfDie* Eco_Dwarf_Resolve(struct Eco_Dwarf* dwarf, Dwarf_Off dwarf_die_off)
{
    struct Eco_DwarfDie*  die;
    Dwarf_Off             off;

    for (die = dwarf->dies;
         die != NULL;
         die = die->next)
    {
        if (dwarf_dieoffset(die->die, &off, NULL) == DW_DLV_OK)
        {
            if (off == dwarf_die_off)
                return die;
        }
    }
    return NULL;
}


struct Eco_DwarfDie* Eco_DwarfDie_New(struct Eco_Dwarf*    session,
                                      Dwarf_Die            dwarf_die,
                                      struct Eco_DwarfDie* parent)
{
    struct Eco_DwarfDie*  die;

    die = Eco_LibC_Alloc(sizeof(struct Eco_DwarfDie));

    if (die != NULL)
    {
        die->session    =  session;
        die->die        =  dwarf_die;
        die->parent     =  parent;
        die->sibling    =  NULL;
        die->child      =  NULL;
        die->holder     =  NULL;
        die->prev       = &session->dies;
        die->next       =  session->dies;
        if (session->dies != NULL)
            session->dies->prev = &die->next;
        session->dies   =  die;
    }

    return die;
}

void Eco_DwarfDie_Delete(struct Eco_DwarfDie* die)
{
    if (die->sibling != NULL) Eco_DwarfDie_Delete(die->sibling);
    if (die->child != NULL)   Eco_DwarfDie_Delete(die->child);
    if (die->holder != NULL)  *(die->holder) = die->sibling;
    if (die->next != NULL)    die->next->prev = die->prev;
    *(die->prev) = die->next;
    dwarf_dealloc(die->session->debug, die->die, 0);
    Eco_LibC_Free(die);
}

struct Eco_DwarfDie* Eco_Dwarf_GetFirstDie(struct Eco_Dwarf* dwarf)
{
    struct Eco_DwarfDie*  die;
    Dwarf_Die             dwarf_die;

    dwarf_next_cu_header(dwarf->debug, NULL, NULL, NULL, NULL, NULL, NULL);

    if (dwarf_siblingof(dwarf->debug, NULL, &dwarf_die, NULL) != DW_DLV_OK)
        return NULL;

    die = Eco_DwarfDie_New(dwarf, dwarf_die, NULL);

    if (die != NULL)
    {
        /* Add initialization code here */
    }

    return die;
}

struct Eco_DwarfDie* Eco_DwarfDie_Sibling(struct Eco_DwarfDie* prev)
{
    struct Eco_DwarfDie*  die;
    Dwarf_Die             dwarf_die;

    if (prev->sibling != NULL)
        return prev->sibling;

    if (dwarf_siblingof(prev->session->debug, prev->die, &dwarf_die, NULL) != DW_DLV_OK)
        return NULL;

    die = Eco_DwarfDie_New(prev->session, dwarf_die, prev->parent);

    if (die != NULL)
    {
        die->holder   = &prev->sibling;
        prev->sibling =  die;
    }

    return die;
}

struct Eco_DwarfDie* Eco_DwarfDie_Child(struct Eco_DwarfDie* parent)
{
    struct Eco_DwarfDie*  die;
    Dwarf_Die             dwarf_die;

    if (parent->child != NULL)
        return parent->child;

    if (dwarf_child(parent->die, &dwarf_die, NULL) != DW_DLV_OK)
        return NULL;

    die = Eco_DwarfDie_New(parent->session, dwarf_die, parent);

    if (die != NULL)
    {
        die->holder   = &parent->child;
        parent->child =  die;
    }

    return die;
}

bool Eco_DwarfDie_Is(struct Eco_DwarfDie* die, const char* name)
{
    Dwarf_Half   tag;
    const char*  tag_name;

    dwarf_tag(die->die, &tag, NULL);
    dwarf_get_TAG_name(tag, &tag_name);
    return Eco_StrEq(name, tag_name);
}

bool Eco_DwarfDie_Name(struct Eco_DwarfDie* die, char* dest, Eco_Size_t size)
{
    Dwarf_Error  error;
    Dwarf_Half   tag;
    const char*  tag_name;

    tag   = 0;
    error = 0;
    if (dwarf_tag(die->die, &tag, &error) != DW_DLV_OK)
        return false;
    dwarf_get_TAG_name(tag, &tag_name);
    Eco_Strncpy(dest, tag_name, size);
    return true;
}

bool Eco_DwarfDie_AttrName(struct Eco_DwarfDie* die, char* dest, Eco_Size_t size)
{
    char*  die_name;

    if (dwarf_diename(die->die, &die_name, NULL) != DW_DLV_OK)
        return false;
    Eco_Strncpy(dest, die_name, size);
    dwarf_dealloc(die->session->debug, die_name, DW_DLA_STRING);
    return true; /* TODO: Check success */
}

bool Eco_DwarfDie_AttrType(struct Eco_DwarfDie* die, struct Eco_DwarfDie** loc)
{
    Dwarf_Bool       hasattr;
    Dwarf_Attribute  attr;
    Dwarf_Error      error;
    Dwarf_Off        off;
    //Dwarf_Die        dwarf_die;

    if (dwarf_hasattr(die->die, DW_AT_type, &hasattr, &error) != DW_DLV_OK)
        return false;
    if (!hasattr)
        return false;
    if (dwarf_attr(die->die, DW_AT_type, &attr, &error) != DW_DLV_OK)
        return false;
    if (dwarf_global_formref(attr, &off, &error) != DW_DLV_OK)
        return false;
    if (loc != NULL)
        *loc = Eco_Dwarf_Resolve(die->session, off);
    return true;
}

static void Eco_DwarfDie_PrintRecursively(struct Eco_DwarfDie* die, int depth)
{
    struct Eco_DwarfDie*  other;
    struct Eco_DwarfDie*  type;
    char                  buffer[1024];

    for (int x = 0; x < depth; x++)
        printf("  ");
    if (Eco_DwarfDie_Name(die, buffer, sizeof(buffer)))
        printf("%s", buffer);
    else
        printf("???");
    if (Eco_DwarfDie_AttrName(die, buffer, sizeof(buffer)))
        printf(": %s", buffer);
    if (Eco_DwarfDie_AttrType(die, &type))
        printf(", type=%p", type);
    printf("\n");
    if ((other = Eco_DwarfDie_Child(die)) != NULL)
        Eco_DwarfDie_PrintRecursively(other, depth + 1);
    if ((other = Eco_DwarfDie_Sibling(die)) != NULL)
        Eco_DwarfDie_PrintRecursively(other, depth);
}

static void Eco_DwarfDie_Print(struct Eco_DwarfDie* die)
{
    Eco_DwarfDie_PrintRecursively(die, 0);
}

void Eco_Dwarf_Test(const char* path)
{
    struct Eco_Dwarf      session;

    Eco_Dwarf_Create(&session, path);
    if (session.head != NULL)
        Eco_DwarfDie_Print(session.head);
    Eco_Dwarf_Destroy(&session);
}

#endif

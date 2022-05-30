#include <fcntl.h>
#include <unistd.h>

#include <ecore/util/libc.h>
#include <ecore/util/memory.h>

#include "dwarf.h"

bool Eco_Dwarf_Create(struct Eco_Dwarf* dwarf, const char* path)
{
    dwarf->fd = open(path, O_RDONLY);

    if (dwarf->fd < 0) return false;

    if (dwarf_init(dwarf->fd, DW_DLC_READ, NULL, NULL, &dwarf->debug, NULL) == DW_DLV_OK) {
        return true;
    } else {
        close(dwarf->fd);
        return false;
    }
}

void Eco_Dwarf_Destroy(struct Eco_Dwarf* dwarf)
{
    dwarf_finish(dwarf->debug, NULL);
    close(dwarf->fd);
}

struct Eco_DwarfDie* Eco_DwarfDie_Sibling(struct Eco_DwarfDie* prev)
{
    struct Eco_DwarfDie*  die;
    Dwarf_Die             dwarf_die;

    if (prev->sibling != NULL)
        return prev->sibling;

    if (dwarf_siblingof(prev->session->debug, prev->die, &dwarf_die, NULL) == DW_DLV_ERROR)
        return NULL;

    die = Eco_LibC_Alloc(sizeof(struct Eco_DwarfDie));

    if (die != NULL)
    {
        die->session    =  prev->session;
        die->die        =  dwarf_die;
        die->parent     =  prev->parent;
        die->sibling    =  NULL;
        die->child      =  NULL;
        die->prev       = &prev->sibling;
        prev->sibling   =  die;
    }

    return die;
}

struct Eco_DwarfDie* Eco_DwarfDie_Child(struct Eco_DwarfDie* parent)
{
    struct Eco_DwarfDie*  die;
    Dwarf_Die             dwarf_die;

    if (parent->child != NULL)
        return parent->child;

    if (dwarf_child(parent->die, &dwarf_die, NULL) == DW_DLV_ERROR)
        return NULL;

    die = Eco_LibC_Alloc(sizeof(struct Eco_DwarfDie));

    if (die != NULL)
    {
        die->session    =  parent->session;
        die->die        =  dwarf_die;
        die->parent     =  parent;
        die->sibling    =  NULL;
        die->child      =  NULL;
        die->prev       = &parent->child;
        parent->child   =  die;
    }

    return die;
}

void Eco_DwarfDie_Delete(struct Eco_DwarfDie* die)
{
    if (die->sibling != NULL) Eco_DwarfDie_Delete(die->sibling);
    if (die->child != NULL)   Eco_DwarfDie_Delete(die->child);
    if (die->prev != NULL)   *(die->prev) = NULL;
    dwarf_dealloc(die->session->debug, die->die, 0);
    Eco_LibC_Free(die);
}

bool Eco_DwarfDie_Is(struct Eco_DwarfDie* die, const char* name)
{
    Dwarf_Half   tag;
    const char*  tag_name;

    dwarf_tag(die->die, &tag, NULL);
    dwarf_get_TAG_name(tag, &tag_name);
    return Eco_StrEq(name, tag_name);
}

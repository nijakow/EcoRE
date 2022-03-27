#ifndef ECO_OBJECTS_SYMBOL_SYMBOL_H
#define ECO_OBJECTS_SYMBOL_SYMBOL_H

#include <ecore/objects/base/object.h>

struct eco_symbol
{
    struct eco_object    _;

    struct eco_symbol**  prev;
    struct eco_symbol*   next;

    eco_size_t           name_length;
    char                 name[0];
};

struct eco_symbol* eco_symbol_get(const char*);

#endif

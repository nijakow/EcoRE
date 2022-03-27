#include <ecore/objects/base/type.h>
#include <ecore/utils/string.h>

#include "symbol.h"

static struct eco_symbol* ECO_SYMBOLS = NULL;

static struct eco_type ECO_SYMBOL_TYPE =
{

};

static struct eco_symbol* eco_symbol_new(const char* name)
{
    eco_size_t          name_length;
    struct eco_symbol*  symbol;

    name_length = eco_strlen(name);
    symbol      = eco_object_new(&ECO_SYMBOL_TYPE, sizeof(symbol) + (name_length + 1) * sizeof(char));

    if (symbol != NULL)
    {
        symbol->name_length = name_length;
        eco_memcpy(symbol->name, name, name_length + 1);

        if (ECO_SYMBOLS != NULL) {
            ECO_SYMBOLS->next->prev = &symbol->next;
        }

        symbol->prev = &ECO_SYMBOLS;
        symbol->next =  ECO_SYMBOLS;
        ECO_SYMBOLS  =  symbol;
    }

    return symbol;
}

struct eco_symbol* eco_symbol_get(const char* name)
{
    struct eco_symbol*  symbol;

    for (symbol = ECO_SYMBOLS; symbol != NULL; symbol = symbol->next)
    {
        if (eco_streq(symbol->name, name))
            return symbol;
    }
    return eco_symbol_new(name);
}

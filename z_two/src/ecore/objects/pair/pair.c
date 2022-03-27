#include <ecore/objects/base/type.h>

#include "pair.h"

static struct eco_type ECO_PAIR_TYPE =
{
};

struct eco_pair* eco_cons(eco_any car, eco_any cdr)
{
    struct eco_pair*  pair;

    pair = eco_object_new(&ECO_PAIR_TYPE, sizeof(struct eco_pair));

    if (pair != NULL)
    {
        pair->car = car;
        pair->cdr = cdr;
    }

    return pair;
}

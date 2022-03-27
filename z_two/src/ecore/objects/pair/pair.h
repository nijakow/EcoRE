#ifndef ECO_OBJECTS_PAIR_PAIR_H
#define ECO_OBJECTS_PAIR_PAIR_H

#include <ecore/base/any.h>
#include <ecore/objects/base/object.h>

struct eco_pair
{
    struct eco_object  _;

    eco_any            car;
    eco_any            cdr;
};

struct eco_pair* eco_cons(eco_any car, eco_any cdr);

#endif

#ifndef ECO_OBJECTS_TAGGED_TAGGED_H
#define ECO_OBJECTS_TAGGED_TAGGED_H

#include <ecore/base/any.h>
#include <ecore/objects/base/object.h>

struct eco_tagged
{
    struct eco_object  _;

    eco_size_t         body_length;

    eco_any            tag;
    eco_any            body[0];
};

struct eco_tagged* eco_tagged_new(eco_any tag, eco_size_t size);

#endif

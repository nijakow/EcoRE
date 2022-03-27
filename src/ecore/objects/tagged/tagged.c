#include <ecore/objects/base/type.h>

#include "tagged.h"

static struct eco_type ECO_TAGGED_TYPE =
{
};

struct eco_tagged* eco_tagged_new(eco_any tag, eco_size_t size)
{
    eco_size_t          index;
    struct eco_tagged*  tagged;

    tagged = eco_object_new(&ECO_TAGGED_TYPE, sizeof(struct eco_tagged) + sizeof(eco_any) * size);

    if (tagged != NULL)
    {
        tagged->tag         = tag;
        tagged->body_length = size;
        for (index = 0; index < size; index++)
            tagged->body[index] = eco_any_nil();
    }

    return tagged;
}

#include "instance.h"

#include "../../objects/key.h"
#include "../../vm/memory/memory.h"


void Eco_EConnect_Instance_Create(struct Eco_EConnect_Instance* instance)
{
    instance->objects_by_id     = NULL;
    instance->objects_by_id_max = 0;

    Eco_EConnect_Instance_BindObject(instance, (struct Eco_Object*) Eco_Key_Find("ecosphere.econnect.block"), 0);
    Eco_EConnect_Instance_BindObject(instance, (struct Eco_Object*) Eco_Key_Find("ecosphere.object.key"), 1);
}

void Eco_EConnect_Instance_Destroy(struct Eco_EConnect_Instance* instance)
{
    if (instance->objects_by_id != NULL) {
        Eco_Memory_Free(instance->objects_by_id);
    }
}

void Eco_EConnect_Instance_BindObject(struct Eco_EConnect_Instance* instance,
                                      struct Eco_Object* object,
                                      unsigned int id)
{
    unsigned int  i;

    if (instance->objects_by_id_max <= id) {
        i                        = instance->objects_by_id_max;
        instance->objects_by_id  = Eco_Memory_Realloc(instance->objects_by_id,
                                                      sizeof(struct Eco_Object*) * (id + 8));
        instance->objects_by_id_max = (id + 8);

        while (i < instance->objects_by_id_max)
            instance->objects_by_id[i++] = NULL;
    }

    instance->objects_by_id[id] = object;
}

void Eco_EConnect_Instance_OptionallyBindObject(struct Eco_EConnect_Instance* instance,
                                                struct Eco_Object* object,
                                                unsigned int id)
{
    if (id != 0) {
        Eco_EConnect_Instance_BindObject(instance, object, id);
    }
}

void Eco_EConnect_Instance_UnbindObject(struct Eco_EConnect_Instance* instance,
                                        unsigned int id)
{
    if (id < instance->objects_by_id_max) {
        instance->objects_by_id[id] = NULL;
    }
}

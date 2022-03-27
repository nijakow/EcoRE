#ifndef ECO_OBJECTS_VM_CODE_CODE_H
#define ECO_OBJECTS_VM_CODE_CODE_H

#include <ecore/base/any.h>
#include <ecore/objects/base/object.h>

struct eco_code
{
    struct eco_object  _;

    eco_byte*          bytecodes;
    eco_size_t         bytecodes_length;

    eco_any*           constants;
    eco_size_t         constants_length;

    struct eco_code**  subcodes;
    eco_size_t         subcodes_length;

    eco_byte           payload[0];
};

struct eco_code* eco_code_new(eco_byte*,        eco_size_t,
                              eco_any*,         eco_size_t,
                              struct eco_code*, eco_size_t);

#endif

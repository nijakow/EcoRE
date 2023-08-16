#include "code.h"

#include "object_body.h"

#include <ecore/io/econnect/reader/parser.h>
#include <ecore/objects/vm/code/code.h>
#include <ecore/vm/memory/memory.h>


bool Eco_EConnect_Builtin_GetCode(struct Eco_EConnect_Reader* reader,
                                  struct Eco_EConnect_Result* result)
{
    unsigned int      id;
    unsigned int      index;
    struct Eco_Code*  the_code;

    the_code                      = Eco_Code_New();

    id                            = Eco_EConnect_ParseUInt(&reader->stream);

    Eco_EConnect_Instance_OptionallyBindObject(reader->instance, (struct Eco_Object*) the_code, id);

    the_code->register_count      = Eco_EConnect_ParseUInt(&reader->stream);
    the_code->arg_count           = Eco_EConnect_ParseUInt(&reader->stream);
    the_code->has_varargs         = Eco_EConnect_ParseUInt(&reader->stream) != 0;

    the_code->constant_count      = Eco_EConnect_ParseUInt(&reader->stream);
    the_code->constants           = Eco_Memory_Alloc(the_code->constant_count * sizeof(Eco_Any));
    for (index = 0; index < the_code->constant_count; index++)
    {
        if (!Eco_EConnect_Reader_ReadAny(reader, result, &the_code->constants[index]))
            return false;
    }
    
    the_code->bytecode_count      = Eco_EConnect_ParseUInt(&reader->stream);
    the_code->bytecodes           = Eco_Memory_Alloc(the_code->bytecode_count);
    Eco_EConnect_ParseBytes(&reader->stream, (char*) the_code->bytecodes, the_code->bytecode_count);

    Eco_EConnect_Result_Create_Object(result, (struct Eco_Object*) the_code);

    return true;
}

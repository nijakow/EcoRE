#include "code.h"

#include "object_body.h"

#include "../../parser.h"
#include "../../../../../objects/vm/code/code.h"
#include "../../../../../vm/memory/memory.h"


bool Eco_EConnect_Builtin_GetCode(struct Eco_EConnect_Reader* reader,
                                  struct Eco_EConnect_Result* result)
{
    unsigned int      id;
    unsigned int      index;
    struct Eco_Code*  the_code;

    the_code       = Eco_Code_New();

    id             = Eco_EConnect_ParseUInt(&reader->stream);

    Eco_EConnect_Instance_OptionallyBindObject(reader->instance, (struct Eco_Object*) the_code, id);
    if (!Eco_EConnect_Reader_ReadObjectBody(reader, result, (struct Eco_Object*) the_code))
        return false;

    the_code->register_count      = Eco_EConnect_ParseUInt(&reader->stream);
    the_code->arg_count           = Eco_EConnect_ParseUInt(&reader->stream);

    the_code->constant_count      = Eco_EConnect_ParseUInt(&reader->stream);
    the_code->constants           = Eco_Memory_Alloc(the_code->constant_count * sizeof(Eco_Any));
    for (index = 0; index < the_code->constant_count; index++)
    {
        if (!Eco_EConnect_Reader_ReadAny(reader, result, &the_code->constants[index]))
            return false;
    }

    the_code->code_instance_count = Eco_EConnect_ParseUInt(&reader->stream);
    the_code->code_instances      = Eco_Memory_Alloc(the_code->code_instance_count * sizeof(struct Eco_Code*));
    for (index = 0; index < the_code->code_instance_count; index++)
    {
        if (!Eco_EConnect_Reader_ReadObject(reader, result, (struct Eco_Object**) &the_code->code_instances[index]))
            return false;
    }
    
    the_code->bytecode_count      = Eco_EConnect_ParseUInt(&reader->stream);
    the_code->bytecodes           = Eco_Memory_Alloc(the_code->bytecode_count);
    Eco_EConnect_ParseBytes(&reader->stream, (char*) the_code->bytecodes, the_code->bytecode_count);

    Eco_EConnect_Result_Create_Object(result, (struct Eco_Object*) the_code);

    return true;
}

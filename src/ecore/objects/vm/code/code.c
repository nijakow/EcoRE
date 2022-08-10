#include "code.h"
#include "ecore/objects/base/object.h"

#include <ecore/objects/base/type.h>
#include <ecore/objects/misc/array/array.h>
#include <ecore/objects/misc/blob/blob.h>
#include <ecore/vm/memory/memory.h>
#include <ecore/vm/memory/gc/gc.h>
#include <ecore/util/memory.h>


/*
 *    T y p e
 */

static struct Eco_TypeCore Eco_Code_TYPECORE;
       struct Eco_Type*    Eco_Code_TYPE;

void Eco_Code_Init()
{
    Eco_TypeCore_Create(&Eco_Code_TYPECORE, "Eco_Code");

    Eco_Code_TYPECORE.send = (Eco_TypeCore_SendFunc) Eco_Object_Send;
    Eco_Code_TYPECORE.mark = (Eco_TypeCore_MarkFunc) Eco_Code_Mark;
    Eco_Code_TYPECORE.del  = (Eco_TypeCore_DelFunc) Eco_Code_Del;

    Eco_Code_TYPE          = Eco_Type_NewPrefab(&Eco_Code_TYPECORE);
}

void Eco_Code_Terminate()
{
    Eco_TypeCore_Destroy(&Eco_Code_TYPECORE);
}


/*
 *    B a s i c s
 */

struct Eco_Code* Eco_Code_New()
{
    return Eco_Object_New(Eco_Code_TYPE, sizeof(struct Eco_Code));
}

void Eco_Code_Mark(struct Eco_GC_State* state, struct Eco_Code* code)
{
    unsigned int  i;

    for (i = 0; i < code->constant_count; i++)
        Eco_GC_State_MarkAny(state, code->constants[i]);

    Eco_Object_Mark(state, &(code->_));
}

void Eco_Code_Del(struct Eco_Code* code)
{
    if (code->bytecodes != NULL)
        Eco_Memory_Free(code->bytecodes);
    if (code->constants != NULL)
        Eco_Memory_Free(code->constants);
    Eco_Object_Del(&(code->_));
}


/*
 *    C o n s t r u c t i o n
 */

struct Eco_Code* Eco_Code_ConstructFromEco(struct Eco_Blob*  bytecodes,
                                           struct Eco_Array* constants,
                                           unsigned int      register_count,
                                           unsigned int      fixed_args,
                                           bool              has_varargs)
{
    struct Eco_Code*  code;
    unsigned int      index;

    code = Eco_Code_New();

    if (code != NULL)
    {
        code->bytecode_count = Eco_Blob_Size(bytecodes);
        /*
         * TODO: Write something like
         *          Eco_Blob_Dup(bytecodes, &code->bytecode_count, &code->bytecodes)
         *       for this
         */
        code->bytecodes           = Eco_Memory_Alloc(code->bytecode_count * sizeof(u8));
        Eco_Memcpy(code->bytecodes, bytecodes->bytes, code->bytecode_count);
        code->constant_count      = Eco_Array_Size(constants);
        code->constants           = Eco_Memory_Alloc(code->constant_count * sizeof(Eco_Any));
        for (index = 0; index < code->constant_count; index++)
            Eco_Any_AssignAny(&code->constants[index], Eco_Array_At(constants, index));
        code->register_count = register_count;
        code->arg_count      = fixed_args;
        code->has_varargs    = has_varargs;
    }

    return code;
}

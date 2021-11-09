#include "builtins.h"

#include "builtin.h"

#include "core/clone.h"
#include "core/flow.h"
#include "core/init.h"
#include "core/io.h"
#include "core/math.h"
#include "core/slots.h"
#include "core/string.h"
#include "core/vector.h"

#include <ecore/vm/fiber.h>
#include <ecore/objects/misc/key/key.h>
#include <ecore/io/logging/log.h>


struct Eco_Object* Eco_VM_Builtin_LOBBY = NULL;


bool Eco_VM_Builtin_Trap(struct Eco_Fiber* fiber, unsigned int args)
{
    Eco_Fiber_SetState(fiber, Eco_Fiber_State_ERROR_BUILTIN_TRAP);
    return false;
}

static void Eco_VM_Builtins_AddBuiltin(const char* name, Eco_Builtin builtin)
{
    struct Eco_Key*  key;

    key = Eco_Key_Find(name);
    
    if (key != NULL) {
        key->builtin = builtin;
        Eco_Object_MakeSticky((struct Eco_Object*) key);
    } else {
        Eco_Log_Critical("Unable to install builtin '%s': Key search returned NULL!\n");
    }
}


void Eco_VM_Builtins_Init()
{
    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.clone", Eco_VM_Builtin_Clone);
    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.flow.value", Eco_VM_Builtin_Value);
    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.flow.reset", Eco_VM_Builtin_Reset);
    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.io.print", Eco_VM_Builtin_Print);
    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.math.add_integers", Eco_VM_Builtin_AddIntegers);
    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.math.add", Eco_VM_Builtin_Add);
    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.math.sub", Eco_VM_Builtin_Subtract2);
    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.math.mul", Eco_VM_Builtin_Multiply2);
    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.math.div", Eco_VM_Builtin_Divide2);
    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.math.mod", Eco_VM_Builtin_Modulo2);
    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.math.less", Eco_VM_Builtin_Less2);
    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.slots.add_value_slot", Eco_VM_Builtin_AddValueSlot);
    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.object.string.at_byte", Eco_VM_Builtin_StringAtByte);
    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.object.string.step_at_byte", Eco_VM_Builtin_StringStepAtByte);
    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.object.string.byte_length", Eco_VM_Builtin_StringByteLength);
    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.object.string.char_length", Eco_VM_Builtin_StringCharLength);
    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.object.vector.at", Eco_VM_Builtin_VectorAt);
    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.object.vector.at_put", Eco_VM_Builtin_VectorAtPut);
    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.object.vector.length", Eco_VM_Builtin_VectorLength);

    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.init.set_lobby", Eco_VM_Builtin_SetLobby);
    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.init.set_true", Eco_VM_Builtin_SetTrue);
    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.init.set_false", Eco_VM_Builtin_SetFalse);
    
    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.init.set_proxy", Eco_VM_Builtin_SetProxy);
    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.constant.integer_type", Eco_VM_Builtin_GetIntegerType);
    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.constant.character_type", Eco_VM_Builtin_GetCharacterType);
    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.constant.block_type", Eco_VM_Builtin_GetBlockType);
    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.constant.string_type", Eco_VM_Builtin_GetStringType);
    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.constant.vector_type", Eco_VM_Builtin_GetVectorType);
}

void Eco_VM_Builtins_Terminate()
{
}

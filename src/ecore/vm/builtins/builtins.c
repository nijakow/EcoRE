#include "builtins.h"

#include "builtin.h"

#include "core/array.h"
#include "core/blob.h"
#include "core/clone.h"
#include "core/code.h"
#include "core/flow.h"
#include "core/init.h"
#include "core/io.h"
#include "core/is.h"
#include "core/key.h"
#include "core/math.h"
#include "core/port.h"
#include "core/reflection.h"
#include "core/slots.h"
#include "core/string.h"

#include <ecore/objects/misc/key/key.h>
#include <ecore/io/logging/log.h>


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
    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.is", Eco_VM_Builtin_Is);
    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.clone", Eco_VM_Builtin_Clone);

    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.flow.value", Eco_VM_Builtin_Value);
    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.flow.reset", Eco_VM_Builtin_Reset);

    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.object.code.new", Eco_VM_Builtin_Code_New);
    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.object.code.value", Eco_VM_Builtin_Code_Value);
    
    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.math.add_integers", Eco_VM_Builtin_AddIntegers);
    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.math.add", Eco_VM_Builtin_Add);
    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.math.sub", Eco_VM_Builtin_Subtract2);
    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.math.mul", Eco_VM_Builtin_Multiply2);
    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.math.div", Eco_VM_Builtin_Divide2);
    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.math.mod", Eco_VM_Builtin_Modulo2);
    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.math.bit_or", Eco_VM_Builtin_BitOr2);
    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.math.bit_and", Eco_VM_Builtin_BitAnd2);
    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.math.bit_xor", Eco_VM_Builtin_BitXor2);
    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.math.bitshift_left", Eco_VM_Builtin_LShift2);
    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.math.bitshift_right", Eco_VM_Builtin_RShift2);
    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.math.less", Eco_VM_Builtin_Less2);

    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.int.as_char", Eco_VM_Builtin_IntAsChar);
    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.char.as_int", Eco_VM_Builtin_CharAsInt);

    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.object.key.as_string", Eco_VM_Builtin_KeyAsString);

    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.object.string.at_byte", Eco_VM_Builtin_StringAtByte);
    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.object.string.step_at_byte", Eco_VM_Builtin_StringStepAtByte);
    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.object.string.byte_length", Eco_VM_Builtin_StringByteLength);
    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.object.string.char_length", Eco_VM_Builtin_StringCharLength);
    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.object.string.as_key", Eco_VM_Builtin_StringAsKey);

    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.object.array.new", Eco_VM_Builtin_ArrayNew);
    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.object.array.size", Eco_VM_Builtin_ArraySize);
    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.object.array.at", Eco_VM_Builtin_ArrayAt);
    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.object.array.at_put", Eco_VM_Builtin_ArrayAtPut);
    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.object.array.to_string", Eco_VM_Builtin_ArrayToString);

    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.object.blob.new", Eco_VM_Builtin_BlobNew);
    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.object.blob.size", Eco_VM_Builtin_BlobSize);
    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.object.blob.at", Eco_VM_Builtin_BlobAt);
    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.object.blob.at_put_int8", Eco_VM_Builtin_BlobAtPutInt8);
    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.object.blob.at_put_int16", Eco_VM_Builtin_BlobAtPutInt16);
    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.object.blob.at_put_int32", Eco_VM_Builtin_BlobAtPutInt32);
    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.object.blob.open_file", Eco_VM_Builtin_BlobOpenFile);

    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.object.port.new", Eco_VM_Builtin_NewPort);
    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.object.port.read_byte", Eco_VM_Builtin_PortReadByte);
    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.object.port.flush_output", Eco_VM_Builtin_PortFlushOutput);
    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.object.port.write_byte", Eco_VM_Builtin_PortWriteByte);
    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.object.port.write_char", Eco_VM_Builtin_PortWriteChar);
    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.object.port.notify_me", Eco_VM_Builtin_PortNotifyMe);

    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.object.interface.get_entry_info", Eco_VM_Builtin_InterfaceGetEntryInfo);
    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.object.interface.add_entry", Eco_VM_Builtin_InterfaceAddEntry);

    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.reflect.get_type", Eco_VM_Builtin_GetType);
    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.reflect.get_type_slot_names", Eco_VM_Builtin_GetTypeSlotNames);
    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.reflect.add_value_slot", Eco_VM_Builtin_AddValueSlot);
    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.reflect.remove_slot", Eco_VM_Builtin_RemoveSlot);

    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.init.set_lobby", Eco_VM_Builtin_SetLobby);
    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.init.set_true", Eco_VM_Builtin_SetTrue);
    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.init.set_false", Eco_VM_Builtin_SetFalse);
    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.init.set_proxy", Eco_VM_Builtin_SetProxy);

    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.constant.array_type", Eco_VM_Builtin_GetArrayType);
    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.constant.blob_type", Eco_VM_Builtin_GetBlobType);
    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.constant.code_type", Eco_VM_Builtin_GetCodeType);
    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.constant.integer_type", Eco_VM_Builtin_GetIntegerType);
    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.constant.character_type", Eco_VM_Builtin_GetCharacterType);
    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.constant.block_type", Eco_VM_Builtin_GetBlockType);
    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.constant.key_type", Eco_VM_Builtin_GetKeyType);
    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.constant.string_type", Eco_VM_Builtin_GetStringType);
    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.constant.port_type", Eco_VM_Builtin_GetPortType);
    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.constant.interface_type", Eco_VM_Builtin_GetInterfaceType);

    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.debug.shortlog", Eco_VM_Builtin_Print);
}

void Eco_VM_Builtins_Terminate()
{
}

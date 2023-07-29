#include "builtins.h"

#include "builtin.h"

#include "core/array.h"
#include "core/blob.h"
#include "core/clone.h"
#include "core/code.h"
#include "core/ffi.h"
#include "core/flow.h"
#include "core/init.h"
#include "core/io.h"
#include "core/is.h"
#include "core/key.h"
#include "core/math.h"
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
    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.myself", Eco_VM_Builtin_Myself);
    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.is", Eco_VM_Builtin_Is);
    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.clone", Eco_VM_Builtin_Clone);

    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.flow.value", Eco_VM_Builtin_Value);
    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.flow.reset", Eco_VM_Builtin_Reset);
    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.flow.throw", Eco_VM_Builtin_Throw);
    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.flow.set_exception_handler", Eco_VM_Builtin_SetExceptionHandler);

    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.object.code.new", Eco_VM_Builtin_Code_New);
    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.object.code.value", Eco_VM_Builtin_Code_Value);
    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.object.code.bytecodes", Eco_VM_Builtin_Code_Bytecodes);
    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.object.code.constants", Eco_VM_Builtin_Code_Constants);
    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.object.code.arguments", Eco_VM_Builtin_Code_ArgCount);
    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.object.code.registers", Eco_VM_Builtin_Code_RegisterCount);
    
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
    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.math.sqrt", Eco_VM_Builtin_Sqrt);
    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.math.sin", Eco_VM_Builtin_Sin);
    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.math.cos", Eco_VM_Builtin_Cos);
    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.math.tan", Eco_VM_Builtin_Tan);

    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.int.as_char", Eco_VM_Builtin_IntAsChar);
    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.int.as_float", Eco_VM_Builtin_IntAsFloat);
    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.float.as_int", Eco_VM_Builtin_FloatAsInt);
    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.char.as_int", Eco_VM_Builtin_CharAsInt);

    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.object.key.as_string", Eco_VM_Builtin_KeyAsString);
    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.object.key.gensym", Eco_VM_Builtin_Gensym);

    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.object.string.byte_at", Eco_VM_Builtin_StringByteAt);
    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.object.string.at_byte", Eco_VM_Builtin_StringCodepointAtByte);
    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.object.string.step_at_byte", Eco_VM_Builtin_StringStepAtByte);
    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.object.string.byte_length", Eco_VM_Builtin_StringByteLength);
    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.object.string.char_length", Eco_VM_Builtin_StringCharLength);
    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.object.string.as_key", Eco_VM_Builtin_StringAsKey);

    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.object.array.array", Eco_VM_Builtin_Array);
    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.object.array.new", Eco_VM_Builtin_ArrayNew);
    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.object.array.size", Eco_VM_Builtin_ArraySize);
    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.object.array.at", Eco_VM_Builtin_ArrayAt);
    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.object.array.at_put", Eco_VM_Builtin_ArrayAtPut);
    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.object.array.to_string", Eco_VM_Builtin_ArrayToString);

    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.object.blob.new", Eco_VM_Builtin_BlobNew);
    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.object.blob.size", Eco_VM_Builtin_BlobSize);
    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.object.blob.at_int8", Eco_VM_Builtin_BlobAtInt8);
    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.object.blob.at_int16", Eco_VM_Builtin_BlobAtInt16);
    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.object.blob.at_int32", Eco_VM_Builtin_BlobAtInt32);
    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.object.blob.at_ptr", Eco_VM_Builtin_BlobAtPtr);
    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.object.blob.at_any", Eco_VM_Builtin_BlobAtAny);
    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.object.blob.at_put_int8", Eco_VM_Builtin_BlobAtPutInt8);
    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.object.blob.at_put_int16", Eco_VM_Builtin_BlobAtPutInt16);
    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.object.blob.at_put_int32", Eco_VM_Builtin_BlobAtPutInt32);
    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.object.blob.at_put_ptr", Eco_VM_Builtin_BlobAtPutPtr);
    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.object.blob.at_put_any", Eco_VM_Builtin_BlobAtPutAny);
    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.object.blob.read_from", Eco_VM_Builtin_BlobReadFrom);
    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.object.blob.write_to", Eco_VM_Builtin_BlobWriteTo);
    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.object.blob.open_file", Eco_VM_Builtin_BlobOpenFile);

    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.object.interface.get_parents", Eco_VM_Builtin_InterfaceGetParents);
    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.object.interface.get_implementing_types", Eco_VM_Builtin_InterfaceGetImplementingTypes);
    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.object.interface.get_entry_info", Eco_VM_Builtin_InterfaceGetEntryInfo);
    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.object.interface.add_parent", Eco_VM_Builtin_InterfaceAddParent);
    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.object.interface.add_entry", Eco_VM_Builtin_InterfaceAddEntry);
    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.object.interface.implements_message", Eco_VM_Builtin_InterfaceImplementsMessage);
    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.object.interface.implements_interface", Eco_VM_Builtin_InterfaceImplementsInterface);
    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.object.interface.get_all_interfaces", Eco_VM_Builtin_InterfaceGetAllInterfaces);

    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.reflect.is_molecule", Eco_VM_Builtin_IsMolecule);
    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.reflect.is_interface", Eco_VM_Builtin_IsInterface);
    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.reflect.is_code", Eco_VM_Builtin_IsCode);
    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.reflect.slot_value", Eco_VM_Builtin_GetSlotValue);
    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.reflect.slot_flags", Eco_VM_Builtin_GetSlotFlags);
    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.reflect.type_get_private_interface", Eco_VM_Builtin_GetInterface);
    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.reflect.type_get_public_interface", Eco_VM_Builtin_GetPublicInterface);
    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.reflect.get_type", Eco_VM_Builtin_GetType);
    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.reflect.get_type_slot_names", Eco_VM_Builtin_GetTypeSlotNames);
    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.reflect.get_type_slot_info", Eco_VM_Builtin_GetTypeSlotInfo);
    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.reflect.get_type_supers", Eco_VM_Builtin_GetTypeSupers);
    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.reflect.get_type_subs", Eco_VM_Builtin_GetTypeSubs);
    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.reflect.get_type_proxy", Eco_VM_Builtin_GetTypeProxy);
    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.reflect.add_value_slot", Eco_VM_Builtin_AddValueSlot);
    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.reflect.add_code_slot", Eco_VM_Builtin_AddCodeSlot);
    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.reflect.remove_slot", Eco_VM_Builtin_RemoveSlot);

#ifdef ECO_CONFIG_USE_FFI
    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.ffi.type.new_struct", Eco_VM_Builtin_FFIType_NewStruct);
    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.ffi.type.new_union", Eco_VM_Builtin_FFIType_NewUnion);
    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.ffi.type.new_array", Eco_VM_Builtin_FFIType_NewArray);
    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.ffi.type.pointer_to", Eco_VM_Builtin_FFIType_PointerTo);
    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.ffi.type.pointee_or_self", Eco_VM_Builtin_FFIType_PointeeOrSelf);
    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.ffi.type.by_index", Eco_VM_Builtin_FFIType_GetForIndex);
    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.ffi.type.sizeof", Eco_VM_Builtin_FFIType_GetSizeInBytes);
    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.ffi.type.typeof", Eco_VM_Builtin_FFIType_GetTypeOf);
    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.ffi.type.nameof", Eco_VM_Builtin_FFIType_GetNameOf);
    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.ffi.type.offsetof", Eco_VM_Builtin_FFIType_GetOffsetOf);
    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.ffi.type.member_count", Eco_VM_Builtin_FFIType_GetMemberCount);
    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.ffi.function.new", Eco_VM_Builtin_FFIFunction_New);
    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.ffi.function.arg_count", Eco_VM_Builtin_FFIFunction_ArgCount);
    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.ffi.function.return_type", Eco_VM_Builtin_FFIFunction_ReturnType);
    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.ffi.function.arg_type", Eco_VM_Builtin_FFIFunction_ArgType);
    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.ffi.function.call", Eco_VM_Builtin_FFIFunction_Call);
    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.ffi.function.eco_call", Eco_VM_Builtin_FFIFunction_EcoCall);
    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.ffi.object.alloc", Eco_VM_Builtin_FFIObject_Alloc);
    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.ffi.object.cast_to", Eco_VM_Builtin_FFIObject_Cast);
    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.ffi.object.get_type", Eco_VM_Builtin_FFIObject_GetType);
    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.ffi.object.get_size", Eco_VM_Builtin_FFIObject_GetSize);
    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.ffi.object.assign_null", Eco_VM_Builtin_FFIObject_AssignNull);
    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.ffi.object.address", Eco_VM_Builtin_FFIObject_Address);
    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.ffi.object.fetch", Eco_VM_Builtin_FFIObject_Fetch);
    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.ffi.object.fetch_with_offset", Eco_VM_Builtin_FFIObject_FetchWithOffset);
    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.ffi.object.store", Eco_VM_Builtin_FFIObject_Store);
    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.ffi.object.store_with_offset", Eco_VM_Builtin_FFIObject_StoreWithOffset);
    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.ffi.object.access", Eco_VM_Builtin_FFIObject_Access);
  #ifdef ECO_CONFIG_USE_DLOPEN
    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.ffi.object.dlopen", Eco_VM_Builtin_FFIObjectDLOpen);
    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.ffi.object.dlsym", Eco_VM_Builtin_FFIObjectDLSym);
  #endif
    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.ffi.lib.open", Eco_VM_Builtin_FFILib_Open);
    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.ffi.lib.at", Eco_VM_Builtin_FFILib_At);
#endif

    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.io.fd.open_file", Eco_VM_Builtin_OpenFile);
    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.io.file_exists", Eco_VM_Builtin_FileExists);
    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.io.file_is_directory", Eco_VM_Builtin_FileIsDirectory);
    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.io.list_files", Eco_VM_Builtin_ListFiles);

    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.os.getenv", Eco_VM_Builtin_GetEnv);

    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.init.version_string", Eco_VM_Builtin_GetVersionString);
    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.init.os_version_id", Eco_VM_Builtin_GetOsVersionId);
    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.init.set_lobby", Eco_VM_Builtin_SetLobby);
    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.init.set_true", Eco_VM_Builtin_SetTrue);
    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.init.set_false", Eco_VM_Builtin_SetFalse);
    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.init.set_proxy", Eco_VM_Builtin_SetProxy);

    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.constant.type_type", Eco_VM_Builtin_GetTypeType);
    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.constant.array_type", Eco_VM_Builtin_GetArrayType);
    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.constant.blob_type", Eco_VM_Builtin_GetBlobType);
    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.constant.code_type", Eco_VM_Builtin_GetCodeType);
    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.constant.integer_type", Eco_VM_Builtin_GetIntegerType);
    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.constant.float_type", Eco_VM_Builtin_GetFloatType);
    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.constant.character_type", Eco_VM_Builtin_GetCharacterType);
    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.constant.block_type", Eco_VM_Builtin_GetBlockType);
    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.constant.key_type", Eco_VM_Builtin_GetKeyType);
    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.constant.string_type", Eco_VM_Builtin_GetStringType);
    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.constant.interface_type", Eco_VM_Builtin_GetInterfaceType);
    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.constant.any", Eco_VM_Builtin_GetDefaultInterface);
    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.constant.ffitype_type", Eco_VM_Builtin_GetFFITypeType);
    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.constant.ffiobject_type", Eco_VM_Builtin_GetFFIObjectType);
    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.constant.ffifunc_type", Eco_VM_Builtin_GetFFIFuncType);
    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.constant.ffilib_type", Eco_VM_Builtin_GetFFILibType);

    Eco_VM_Builtins_AddBuiltin("ecosphere.ecore.debug.shortlog", Eco_VM_Builtin_Print);
}

void Eco_VM_Builtins_Terminate()
{
}

#include "objects.h"

#include "base/object.h"
#include "molecule/molecule.h"
#include "io/port.h"
#include "misc/array/array.h"
#include "misc/blob/blob.h"
#include "misc/key/key.h"
#include "misc/string/string.h"
#include "pseudo/character.h"
#include "pseudo/integer.h"
#include "vm/code/code.h"
#include "vm/code/closure.h"
#include "vm/interface/interface.h"
#include "vm/ffi/ffitype.h"
#include "vm/ffi/ffifunc.h"


void Eco_Objects_Init()
{
    Eco_Object_Init();
    Eco_Molecule_Init();
    Eco_Key_Init();
    Eco_Interface_Init();
    Eco_Code_Init();
    Eco_Integer_Init();
    Eco_Character_Init();
    Eco_Blob_Init();
    Eco_String_Init();
    Eco_Closure_Init();
    Eco_Array_Init();
    Eco_Port_Init();
    Eco_FFIType_Init();
    Eco_FFIFunc_Init();
}

void Eco_Objects_Terminate()
{
    Eco_FFIFunc_Terminate();
    Eco_FFIType_Terminate();
    Eco_Port_Terminate();
    Eco_Array_Terminate();
    Eco_Closure_Terminate();
    Eco_String_Terminate();
    Eco_Blob_Init();
    Eco_Character_Terminate();
    Eco_Integer_Terminate();
    Eco_Code_Terminate();
    Eco_Interface_Terminate();
    Eco_Key_Terminate();
    Eco_Molecule_Terminate();
    Eco_Object_Terminate();
}

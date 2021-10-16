#ifndef ECO_IO_ECONNECT_READER_RESULT_H
#define ECO_IO_ECONNECT_READER_RESULT_H

#include <ecore/eco.h>


enum Eco_EConnect_Result_Type
{
    Eco_EConnect_Result_Type_NONE,
    Eco_EConnect_Result_Type_ERROR,
    Eco_EConnect_Result_Type_ANY
};

enum Eco_EConnect_Result_ErrorType
{
    Eco_EConnect_Result_ErrorType_UNDEFINED,
    Eco_EConnect_Result_ErrorType_RESOURCE_NOT_FOUND,
    Eco_EConnect_Result_ErrorType_INVALID_MESSAGE_HEADER,
    Eco_EConnect_ErrorType_KEY_NOT_FOUND,
    Eco_EConnect_ErrorType_TYPE_ERROR,
    Eco_EConnect_ErrorType_UNBOUND_OBJECT
};

struct Eco_EConnect_Result
{
    enum Eco_EConnect_Result_Type  type;

    union {
        Eco_Any                             any;
        enum Eco_EConnect_Result_ErrorType  error;
    } body;
};

void Eco_EConnect_Result_Create_None(struct Eco_EConnect_Result*);
void Eco_EConnect_Result_Create_Error(struct Eco_EConnect_Result*, enum Eco_EConnect_Result_ErrorType);
void Eco_EConnect_Result_Create_Any(struct Eco_EConnect_Result*, Eco_Any*);
void Eco_EConnect_Result_Create_Object(struct Eco_EConnect_Result*, struct Eco_Object*);
void Eco_EConnect_Result_Create_Integer(struct Eco_EConnect_Result*, Eco_Integer);
void Eco_EConnect_Result_Copy(struct Eco_EConnect_Result*, struct Eco_EConnect_Result*);
void Eco_EConnect_Result_Destroy(struct Eco_EConnect_Result*);

bool Eco_EConnect_Result_ExpectAny(struct Eco_EConnect_Result*, Eco_Any*);
bool Eco_EConnect_Result_ExpectObject(struct Eco_EConnect_Result*, struct Eco_Object**);

#endif

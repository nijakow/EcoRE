#include <ecore/objects/base/type.h>

#include "code.h"

static struct eco_type ECO_CODE_TYPE =
{

};

struct eco_code* eco_code_new(eco_byte*        bytecodes,
                              eco_size_t       bytecodes_length,
                              eco_any*         constants,
                              eco_size_t       constants_length,
                              struct eco_code* subcodes,
                              eco_size_t       subcodes_length)
{
    const eco_size_t bytecodes_offset = 0;
    const eco_size_t constants_offset = bytecodes_offset + bytecodes_length * sizeof(eco_byte);
    const eco_size_t subcodes_offset  = constants_offset + constants_length * sizeof(eco_any);
    const eco_size_t end_offset       = subcodes_offset  + subcodes_length  * sizeof(struct eco_code*);

    struct eco_code*  code;

    code = eco_object_new(&ECO_CODE_TYPE,
                          sizeof(struct eco_code) + end_offset);
    if (code != NULL) {
        code->bytecodes        = (eco_byte*) &code->payload[bytecodes_offset];
        code->bytecodes_length = bytecodes_length;
        code->constants        = (eco_any*) &code->payload[constants_offset];
        code->constants_length = constants_length;
        code->subcodes         = (struct eco_code**) &code->payload[subcodes_offset];
        code->subcodes_length  = subcodes_length;
    }

    return code;
}

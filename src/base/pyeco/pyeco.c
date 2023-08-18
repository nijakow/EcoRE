#include "pyeco.h"

void PyEco_Initialize()
{
    static bool initialized = false;

    if (initialized) {
        printf("PyEco already initialized.\n");
        return;
    }

    printf("Initializing...\n");
    
    Py_Initialize();

    initialized = true;
}

void PyEco_Finalize()
{
    Py_FinalizeEx();
}


bool PyEco_TypeCheck(PyObject* obj, PyTypeObject* type)
{
    printf("Typechecking %p against %p\n", obj, type);
    printf("Module type is: %p\n", &PyModule_Type);
    
    if (obj == NULL || type == NULL) {
        return false;
    }

    return PyObject_TypeCheck(obj, type);
}

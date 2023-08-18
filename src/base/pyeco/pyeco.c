#include "pyeco.h"

void PyEco_Initialize()
{
    static bool initialized = false;

    if (!initialized) {
        Py_Initialize();
        initialized = true;   
    }
}

void PyEco_Finalize()
{
    Py_FinalizeEx();
}


bool PyEco_TypeCheck(PyObject* obj, PyTypeObject* type)
{
    if (obj == NULL || type == NULL) {
        return false;
    }

    return PyObject_TypeCheck(obj, type);
}

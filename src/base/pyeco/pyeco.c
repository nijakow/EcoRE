#include "pyeco.h"

void PyEco_Initialize()
{
    printf("Initializing...\n");
    Py_Initialize();
}

void PyEco_Finalize()
{
    Py_FinalizeEx();
}


bool PyEco_TypeCheck(PyObject* obj, PyTypeObject* type)
{
    return PyObject_TypeCheck(obj, type);
}

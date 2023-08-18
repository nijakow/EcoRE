#ifndef PYECO_H
#define PYECO_H

#include <python3.11/Python.h>
#include <stdbool.h>

void PyEco_Initialize();
void PyEco_Finalize();

bool PyEco_TypeCheck(PyObject* obj, PyTypeObject* type);

#endif

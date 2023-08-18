#ifndef PYECO_H
#define PYECO_H

#include <python3.11/Python.h>
#include <stdbool.h>

void PyEco_Initialize();
void PyEco_Finalize();

bool PyEco_TypeCheck(PyObject* obj, PyTypeObject* type);

PyObject* PyEco_GetNone();

bool PyEco_IsNone(PyObject* obj);
bool PyEco_IsTrue(PyObject* obj);
bool PyEco_IsFalse(PyObject* obj);

const char* PyEco_PyUnicode_GrabString(PyObject* unicode);

#endif

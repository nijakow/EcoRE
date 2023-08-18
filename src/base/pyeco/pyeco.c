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


PyObject* PyEco_GetNone()
{
    return Py_None;
}


bool PyEco_IsNone(PyObject* obj)
{
    return obj == Py_None;
}

bool PyEco_IsTrue(PyObject* obj)
{
    return obj == Py_True;
}

bool PyEco_IsFalse(PyObject* obj)
{
    return obj == Py_False;
}


int PyEco_PyLong_GrabInt(PyObject* pyLong)
{
    return (int) PyLong_AsLong(pyLong);
}

const char* PyEco_PyUnicode_GrabString(PyObject* unicode)
{
    return PyUnicode_AsUTF8(unicode);
}

int PyEco_PyTuple_GrabSize(PyObject* tuple)
{
    return PyTuple_Size(tuple);
}


PyObject* PyEco_CallMethodTupleArgs(PyObject* obj, const char* method, PyObject* args)
{
    size_t     tuple_args;
    size_t     i;
    PyObject*  name;
    PyObject*  result;

    name = PyUnicode_FromString(method);

    tuple_args = PyTuple_Size(args);

    PyObject*  arglist[tuple_args + 1];

    arglist[0] = obj;

    for (i = 0; i < tuple_args; i++) {
        arglist[i + 1] = PyTuple_GetItem(args, i);
    }

    result = PyObject_VectorcallMethod(name, arglist, tuple_args + 1, NULL);

    Py_DECREF(name);

    return result;
}

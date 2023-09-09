#define PY_SSIZE_T_CLEAN
#include <Python.h>

static PyObject *cvarint_encode(PyObject *self, PyObject *args) {
    // use PyErr_NoMemory() to throw an error on malloc
    unsigned long long n;
    char part, out[10];

    if (!PyArg_ParseTuple(args, "K", &n)) return NULL;

    int i = 0;
    while(n > 0) {
        part = n & 0x7f;
        n >>= 7;
        if (n > 0) {
            part |= 0x80;
        }
        out[i++] = part;

    }
    return PyBytes_FromStringAndSize(out, 10);
}

static PyObject *cvarint_decode(PyObject *self, PyObject *args) {
}

static PyMethodDef CVarintMethods[] = {
    {"encode", cvarint_encode, METH_VARARGS, "Encode an integer as varint."},
    {"decode", cvarint_decode, METH_VARARGSi,
     "Decode varint bytes to an integer."},
    {NULL, NULL, 0, NULL}};

static struct PyModuleDef cvarintmodule = {
    PyModuleDef_HEAD_INIT, "cvarint",
    "A C implementation of protobuf varint encoding", -1, CVarintMethods};

PyMODINIT_FUNC PyInit_cvarint(void) { return PyModule_Create(&cvarintmodule); }

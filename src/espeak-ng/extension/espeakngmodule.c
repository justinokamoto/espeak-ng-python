#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include "espeak/speak_lib.h"

static PyObject *
espeak_ng_py_initialize(PyObject *self, PyObject *args)
{
    // TODO: Have initialization values be customizable
    int res = espeak_Initialize(AUDIO_OUTPUT_SYNCH_PLAYBACK, // synchronous mode
				400,                         // buffer length
				NULL,                        // default data directory
				0);                          // no flags
    if (res) {
	Py_INCREF(Py_True);
	return Py_True;
    } else {
	Py_INCREF(Py_False);
	return Py_False;
    }
}

// Module methods
static PyMethodDef EspeakNgMethods[] = {
    {"initialize", espeak_ng_py_initialize, METH_VARARGS,
     "configure speech synthesizer"},
    {NULL, NULL, 0, NULL} // Sentinel
};
    
// Module definition
static struct PyModuleDef espeakngmodule = {
    PyModuleDef_HEAD_INIT,
    "espeak-ng",    // name
    NULL,           // module documentation
    -1,             // -1 since the module keeps state in global variables
    EspeakNgMethods
};

// Module initialization
PyMODINIT_FUNC
PyInit_espeak_ng(void)
{
    // Cannot initialize! Requires module (duh!)
    // espeak_ng_py_initialize();

    // TODO: Set callbacks? Do we need here?
    return PyModule_Create(&espeakngmodule);
}

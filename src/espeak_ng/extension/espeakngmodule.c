#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include "espeak/speak_lib.h"

// Proxy callback that will handle calling the user-specified Python callback
// TODO: Experiment with 'wave' file (I think it's for RETRIEVAL)
int espeak_ng_proxy_callback(short* wave, int num_samples, espeak_EVENT* event)
{
    // TODO: Call the callable
    return 0;
}

static PyObject *
espeak_ng_py_Initialize(PyObject *self, PyObject *args)
{
    // TODO: Have initialization values be customizable
    // TODO: Is it better to automatically use synchronous mode?
    // TODO: What is buflength doing?
    int res = espeak_Initialize(AUDIO_OUTPUT_SYNCH_PLAYBACK, // plays audio in synchronous mode
				400,                         // buffer length
				NULL,                        // default data directory
				0);                          // no flags
    if (res) {
	espeak_SetSynthCallback(espeak_ng_proxy_callback);
	Py_INCREF(Py_True);
	return Py_True;
    } else {
	Py_INCREF(Py_False);
	return Py_False;
    }
}

// Pass it a function pointer...Interesting. I guess we need to parse args and find the pointer then?
static PyObject *
espeak_ng_py_SetSynthCallback(PyObject *self, PyObject *args)
{
    // Parse out callback
    // Verify it is callable
    // Store it and call within proxy callback
}

// Module methods
static PyMethodDef EspeakNgMethods[] = {
    {"initialize", espeak_ng_py_Initialize, METH_VARARGS,
     "configure speech synthesizer"},
    {NULL, NULL, 0, NULL} // Sentinel
};
    
// Module definition
static struct PyModuleDef espeakngmodule = {
    PyModuleDef_HEAD_INIT,
    "espeak_ng",    // name
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

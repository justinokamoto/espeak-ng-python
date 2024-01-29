#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include "espeak/speak_lib.h"

// Proxy callback that will handle calling the user-specified Python callback
// TODO: Experiment with 'wave' file (I think it's for RETRIEVAL)
int espeak_ng_proxy_callback(short* wave, int num_samples, espeak_EVENT* event)
{
    // TODO: Call the callable
    printf("Callback ran.\n");
    return 0;
}

static PyObject *
espeak_ng_py_Synth(PyObject *self, PyObject *args)
{
    // TODO: Parse out options like speech string
    char hello[] = "Hello, world. It is I, Justin Okamoto.\0";
    // TODO: Should take type espeak_ERROR
    int res = espeak_Synth(&hello, 39, 0, POS_CHARACTER, 0, 0, NULL, NULL);
    printf("Synth failed! CODE: %d\n", res);
    if (res != EE_OK) {
	Py_INCREF(Py_False);
	return Py_False;
    }
    Py_INCREF(Py_True);
    return Py_True;
}

static PyObject *
espeak_ng_py_SetVoiceByProperties(PyObject *self, PyObject *args)
{
    // TODO: Parse out options
    espeak_ERROR res;
    espeak_VOICE voice_spec;
    voice_spec.name = "default";
    voice_spec.languages = "en";
    voice_spec.gender = 0;
    voice_spec.age = 0;
    voice_spec.variant = 0;

    res = espeak_SetVoiceByProperties(&voice_spec);

    if (res != EE_OK) {
	Py_INCREF(Py_False);
	return Py_False;
    }
    Py_INCREF(Py_True);
    return Py_True;
}

static PyObject *
espeak_ng_py_ListVoices(PyObject *sef, PyObject *args)
{
    // List all available voices (it's a NULL terminated list)
    const espeak_VOICE **voices_list = espeak_ListVoices(NULL);
    // Create Python list to return
    PyObject *py_list = PyList_New(0); // TODO: Ref count?

    for (int i = 0; voices_list[i]; i++) {
	const espeak_VOICE *item = voices_list[i];
	// TODO: Define and return a voice object (instead of list)
	PyObject *voice_list = Py_BuildValue("{s:s,s:s,s:s,s:i,s:i,s:i}",
					     "name", item->name,
					     "languages", item->languages,
					     "identifier", item->identifier,
					     "gender", item->gender,
					     "age", item->age,
					     "variant", item->variant);
	PyList_Append(py_list, voice_list);
    }
    return py_list;
}
   

static PyObject *
espeak_ng_py_Initialize(PyObject *self, PyObject *args)
{
    // TODO: Have initialization values be customizable
    // TODO: Is it better to automatically use synchronous mode?
    // TODO: What is buflength doing?
    int res = espeak_Initialize(AUDIO_OUTPUT_RETRIEVAL, // plays audio in synchronous mode
				400,                         // buffer length
				NULL,                        // default data directory
				0);                          // no flags
    if (res) {
	espeak_SetSynthCallback(espeak_ng_proxy_callback);
	 // TODO: Probably shouldn't be set here...Should be kept in Python world
	if (espeak_ng_py_SetVoiceByProperties(NULL, NULL) != 0) {
	    Py_INCREF(Py_False);
	    return Py_False;
	}

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
    {"initialize", espeak_ng_py_Initialize, METH_VARARGS, "configure speech synthesizer"},
    {"synth", espeak_ng_py_Synth, METH_VARARGS, "synthesize text to speech"},
    {"list_voices", espeak_ng_py_ListVoices, METH_VARARGS, "list all available voices"},
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

#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include "espeak/speak_lib.h"

// Proxy callback that will handle calling the user-specified Python callback
// TODO: Experiment with 'wave' file (I think it's for RETRIEVAL)
// TODO: Multiple callbacks? Not sure if this is useful...
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
    char hello[] = "Hi\0";
    // TODO: Should take type espeak_ERROR
    int res = espeak_Synth(&hello, 2, 0, POS_CHARACTER, 0, 0, NULL, NULL);
    printf("Synth failed! CODE: %d\n", res);
    if (res != EE_OK) {
	Py_RETURN_FALSE;
    }
    Py_RETURN_TRUE;
}

// TODO: SET VOICE PROPERTIES!!!!!!!!!!!!!!!!!! (Or is it needed?)

static PyObject *
espeak_ng_py_SetVoiceByProperties(PyObject *self, PyObject *args, PyObject *kwargs)
{
    // TODO: Create Python wrapper class around espeak_VOICE
    const char *name=NULL;
    const char *languages=NULL;
    int gender=0, age=0, variant=0;

    static char *kwlist[] = {"name", "languages", "gender", "age", "variant", NULL};

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "|ssiii", kwlist,
				     &name, &languages, &gender, &age, &variant))
	return NULL; // Throw exception (it's already set)

    espeak_ERROR res;
    espeak_VOICE voice_spec;
    voice_spec.name = name;
    voice_spec.languages = languages;
    voice_spec.gender = gender;
    voice_spec.age = age;
    voice_spec.variant = variant;

    res = espeak_SetVoiceByProperties(&voice_spec);

    if (res != EE_OK) {
	Py_RETURN_FALSE;
    }
    Py_RETURN_TRUE;
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
espeak_ng_py_Initialize(PyObject *self, PyObject *args, PyObject *kwargs)
{
    const char *path = NULL;
    int output=0, buflength=0, options=0;

    static char *kwlist[] = {"output", "buflength", "options", "path", NULL};

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "|iiis", kwlist,
				     &output, &buflength, &options, &path))
	return NULL; // Throw exception (it's already set)

    if (path != NULL && access(path, F_OK) == -1) {
	PyErr_SetString(PyExc_RuntimeError, "_espeak_ng.initialize(): path is invalid");
	return NULL; // Throw exception
    }

    // TODO: Make sure this cast is safe! (case switch enums or < >)
    int res = espeak_Initialize((espeak_AUDIO_OUTPUT) output, buflength, path, options);

    // res is either sample rate in Hz, or -1 (EE_INTERNAL_ERROR)
    if (res != -1) {
	espeak_SetSynthCallback(espeak_ng_proxy_callback);
	// TODO: Probably shouldn't be set here...Should be kept in Python world
	// espeak_ng_py_SetVoiceByProperties(NULL, NULL, NULL);

	// Returns sampling rate in Hz
	PyObject *res_py = Py_BuildValue("i", res);
	return res_py;
    } else {
	// TODO: decrement?
	PyObject *res_py = Py_BuildValue("i", res);
	return res_py;
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
    {"initialize", espeak_ng_py_Initialize, METH_VARARGS | METH_KEYWORDS, "configure speech synthesizer"},
    {"set_voice_by_properties", espeak_ng_py_SetVoiceByProperties, METH_VARARGS | METH_KEYWORDS, "set voice by properties"},
    {"synth", espeak_ng_py_Synth, METH_VARARGS, "synthesize text to speech"},
    {"list_voices", espeak_ng_py_ListVoices, METH_VARARGS, "list all available voices"},
    {NULL, NULL, 0, NULL} // Sentinel
};

// Module definition
static struct PyModuleDef espeakngmodule = {
    PyModuleDef_HEAD_INIT,
    "_espeak_ng",    // name
    NULL,            // module documentation
    -1,              // -1 since the module keeps state in global variables
    EspeakNgMethods
};

// Module initialization
PyMODINIT_FUNC
PyInit__espeak_ng(void)
{
    return PyModule_Create(&espeakngmodule);
}

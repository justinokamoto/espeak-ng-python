#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include "espeak/speak_lib.h"


// ***********************************************************
// Python Type Definitions
// ***********************************************************
// TODO: Split into separate .c file?

/*
 * Python wrapper object for espeak_EVENT
 */
typedef struct {
    PyObject_HEAD
    PyObject *type;
    PyObject *unique_identifier;
    PyObject *text_position;
    PyObject *length;
    PyObject *audio_position;
    PyObject *sample;
    PyObject *user_data;
    PyObject *number;
    PyObject *name;
    PyObject *string;
} EspeakNgPyEventObject;

// TODO: Create setters/getters
static PyTypeObject ESpeakNgPyEventObjectType = {
    .ob_base = PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "_espeak_ng.Event",
    .tp_doc = PyDoc_STR("Python wrapper type for espeak_EVENT"),
    .tp_basicsize = sizeof(EspeakNgPyEventObject),
    .tp_itemsize = 0,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_new = PyType_GenericNew,
};

// ***********************************************************
// Helper functions
// ***********************************************************

// Remember: Parsed arguments are BORROWED, so we don't decrement them
// https://docs.python.org/3/c-api/arg.html

// TODO: To enable Pythonic default args, use static vars here

// TODO: Do parsed params from _Build need to be decremented?
// TODO: Test...Do default values get overwritten in parsing, even if arg is a
// keyword with no value provided?

// TODO: Create error enum that can be interpretted

// Python function handle to be executed during synthesization
static PyObject *SynthCallback = NULL;

// Proxy callback that will handle calling the user-specified Python callback
// TODO: Experiment with 'wave' file (I think it's for RETRIEVAL)
// TODO: Multiple callbacks? Not sure if this is useful...
int espeak_ng_proxy_callback(short* wave, int num_samples, espeak_EVENT* event)
{
    // TODO: Does caller own wave lifetime? See if wave is copied in CallFunction

    // TODO: short to char doesn't matter since it's all bytes, right?
    if (SynthCallback == NULL)
	return 0;

    // Result should be either 0 or 1
    PyObject *res_py = PyObject_CallFunction(SynthCallback, "y#ii",
					     wave, num_samples, num_samples, event);
    // TODO: Check if res_py is NULL

    if (!PyLong_Check(res_py)) {
	PyErr_SetString(PyExc_RuntimeError, "espeak_ng_proxy_callback: Callback did not return integer value");
    }

    // Convert the Python integer return code to C int
    int res = PyLong_AsLong(res_py);

    // Check for errors during conversion
    if (res == -1 && PyErr_Occurred()) {
	// TODO: What happens when error occurs in async mode? This
	// routine is called by C lib directly
        PyErr_SetString(PyExc_TypeError, "espeak_ng_proxy_callback: Could not parse callback integer value");
    }

    return res;
}

// ***********************************************************
// Wrapper functions
// ***********************************************************

/*
 * Wrapper function for espeak_Synth
 */
static PyObject *
espeak_ng_py_Synth(PyObject *self, PyObject *args, PyObject *kwargs)
{
    // Null-terminated text to be spoken
    const char *text;
    // Equal-or-greater than size of text data
    size_t size; // TODO: Get this from the string length
    // Position to start speaking
    unsigned int position = 0;
    // Determines whether 'position' denotes chars, words, or
    // sentences.
    espeak_POSITION_TYPE position_type = POS_CHARACTER;
    // Position to end speaking. 0 signifies no end.
    unsigned int end_position = 0;
    // TODO: Char encoding is mute since Python 3 strings are unicode
    // (default UTF-8)? We will apply espeakCHARS_AUTO after parsing args.
    unsigned int flags = 0;
    unsigned int *unique_identifier = NULL;
    PyObject *user_data = NULL;

    static const char *kwlist[] = {"text", "size", "position", "position_type", "end_position",
				   "flags", "unique_identifier", "user_data", NULL};

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "sI|IiIIOO", kwlist,
				     &text, &size, &position, &position_type, &end_position,
				     &flags, &unique_identifier, &user_data)) {
	return NULL; // Throw exception (it's already set)
    }

    int res = espeak_Synth(text, size, position, POS_CHARACTER, end_position,
			   flags | espeakCHARS_AUTO, unique_identifier, (void *) user_data);

    // Returns espeak_ERROR code
    // TODO: Python wrapper module should parse to enum
    PyObject *res_py = Py_BuildValue("i", res); // Caller responsible for decrementing
    return res_py;
}

/*
 * Wrapper function for espeak_SetVoiceByProperties
 */
static PyObject *
espeak_ng_py_SetVoiceByProperties(PyObject *self, PyObject *args, PyObject *kwargs)
{
    // TODO: Create Python wrapper class around espeak_VOICE?
    const char *name=NULL;
    const char *languages=NULL;
    int gender=0, age=0, variant=0;

    static const char *kwlist[] = {"name", "languages", "gender", "age", "variant", NULL};

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

/*
 * Wrapper function for espeak_ListVoices
 */
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
	// TODO: Looks like List doesn't steal reference to
	// voice_list, so it's decremented here? We should double
	// check this.
	Py_DECREF(voice_list);
    }
    return py_list; // Caller's responsiblity for decrementing
}

/*
 * Wrapper function for espeak_Initialize
 */
static PyObject *
espeak_ng_py_Initialize(PyObject *self, PyObject *args, PyObject *kwargs)
{
    const char *path = NULL;
    int output=0, buflength=0, options=0;

    static const char *kwlist[] = {"output", "buflength", "options", "path", NULL};

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "|iiis", kwlist,
				     &output, &buflength, &options, &path))
	return NULL; // Throw exception (it's already set)

    if (path != NULL && access(path, F_OK) == -1) {
	PyErr_SetString(PyExc_RuntimeError, "espeak_ng_py_Initialize: Path parameter is invalid");
	return NULL; // Throw exception
    }

    // TODO: Make sure this cast is safe! (case switch enums or < >)
    // TODO: This blows up when another option is set
    int res = espeak_Initialize(output, buflength, path, options);

    // res is either sample rate in Hz, or -1 (EE_INTERNAL_ERROR)
    if (res != -1) {
	espeak_SetSynthCallback(espeak_ng_proxy_callback);

	// Returns sampling rate in Hz
	PyObject *res_py = Py_BuildValue("i", res); // Caller responsible for decrementing
	return res_py;
    } else {
	PyObject *res_py = Py_BuildValue("i", res); // Caller responsible for decrementing
	return res_py;
    }
}

/*
 * Wrapper function for espean_SetSynthCallback
 */
static PyObject *
espeak_ng_py_SetSynthCallback(PyObject *self, PyObject *args)
{
    // TODO: Is it better to make this static (could get wonky though
    // if PyArg_ doesn't set it every time)
    PyObject *callback = NULL;

    if (!PyArg_ParseTuple(args, "O", &callback))
	return NULL; // Throw exception (it's already set)

    if (!PyCallable_Check(callback)) {
	PyErr_SetString(PyExc_TypeError, "espeak_ng_py_SetSynthCallback: Expected parameter to be callable");
	return NULL; // Throw exception
    }

    // TODO: Validate that the callback accepts certain params?

    // Store callback
    Py_XDECREF(SynthCallback); // Release borrowed reference
    SynthCallback = callback;
    Py_IncRef(SynthCallback); // Borrow reference

    Py_RETURN_NONE;
}

// ***********************************************************
// Python Module
// ***********************************************************

// Module methods
static PyMethodDef EspeakNgMethods[] = {
    {"initialize", espeak_ng_py_Initialize, METH_VARARGS | METH_KEYWORDS, "configure speech synthesizer"},
    {"set_voice_by_properties", espeak_ng_py_SetVoiceByProperties, METH_VARARGS | METH_KEYWORDS, "set voice by properties"},
    {"set_synth_callback", espeak_ng_py_SetSynthCallback, METH_VARARGS, "set synth callback"},
    {"synth", espeak_ng_py_Synth, METH_VARARGS | METH_KEYWORDS, "synthesize text to speech"},
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
    PyObject *m;

    // Initialize type
    if (PyType_Ready(&ESpeakNgPyEventObjectType) < 0)
        return NULL;

    // Create module
    m = PyModule_Create(&espeakngmodule);
    if (m == NULL)
        return NULL;

    // Add type to module
    Py_INCREF(&ESpeakNgPyEventObjectType);
    if (PyModule_AddObject(m, "Event", (PyObject *) &ESpeakNgPyEventObjectType) < 0) {
	// If failure occurs, cleanup refs
        Py_DECREF(&ESpeakNgPyEventObjectType);
        Py_DECREF(m);
        return NULL;
    }
    return m;
}

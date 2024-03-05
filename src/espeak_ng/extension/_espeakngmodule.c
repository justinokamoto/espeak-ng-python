#include <string.h>
#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include "espeak-ng/speak_lib.h"


// ***********************************************************
// Python Type Definitions
// ***********************************************************

/*
 * Python wrapper object for espeak_EVENT
 *
 * It's a simple POJO with getters for all attributes.
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
    PyObject *id;
} EspeakNgPyEventObject;

/*
 * Macro to generate getter methods for EspeakNgPyEventObject
 */
#define EspeakNgPyEventObject_GETTER(name)                                                        \
  static PyObject *EspeakNgPyEventObject_get_##name(EspeakNgPyEventObject *self, void *closure) { \
      Py_INCREF(self->name);                                                                      \
      return self->name;	            	                                                  \
  }

// Define getter methods using macro
EspeakNgPyEventObject_GETTER(type);
EspeakNgPyEventObject_GETTER(unique_identifier);
EspeakNgPyEventObject_GETTER(text_position);
EspeakNgPyEventObject_GETTER(length);
EspeakNgPyEventObject_GETTER(audio_position);
EspeakNgPyEventObject_GETTER(sample);
EspeakNgPyEventObject_GETTER(user_data);
EspeakNgPyEventObject_GETTER(id);

/*
 * Define ESpeakNgPyEventObjectType attributes getter and setter
 */
static PyGetSetDef EspeakNgPyEventObject_getsetters[] = {
    {"type", (getter)EspeakNgPyEventObject_get_type, NULL, "The 'type' attribute", NULL},
    {"unique_identifier", (getter)EspeakNgPyEventObject_get_unique_identifier, NULL, "The 'unique_identifier' attribute", NULL},
    {"text_position", (getter)EspeakNgPyEventObject_get_text_position, NULL, "The 'text_position' attribute", NULL},
    {"length", (getter)EspeakNgPyEventObject_get_length, NULL, "The 'length' attribute", NULL},
    {"audio_position", (getter)EspeakNgPyEventObject_get_audio_position, NULL, "The 'audio_position' attribute", NULL},
    {"sample", (getter)EspeakNgPyEventObject_get_sample, NULL, "The 'sample' attribute", NULL},
    {"user_data", (getter)EspeakNgPyEventObject_get_user_data, NULL, "The 'user_data' attribute", NULL},
    {"id", (getter)EspeakNgPyEventObject_get_id, NULL, "The 'id' attribute, NULL"},
    {NULL} // Sentinel
};

// TODO: Need an init method that sets attributes to None (to avoid
// crashes if other modules attempt to instantiate Event class)
/*
 * Create new Python type '_espeak_ng.Event'
 */
static PyTypeObject ESpeakNgPyEventObjectType = {
    .ob_base = PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "_espeak_ng.Event",
    .tp_doc = PyDoc_STR("Python wrapper type for espeak_EVENT"),
    .tp_basicsize = sizeof(EspeakNgPyEventObject),
    .tp_itemsize = 0,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_new = PyType_GenericNew,
    .tp_getset = EspeakNgPyEventObject_getsetters,
};

// ***********************************************************
// Helper functions
// ***********************************************************

// TODO: To enable Pythonic default args, use static vars here

// Python function handle to be executed during synthesization
static PyObject *SynthCallback = NULL;

/*
 * Proxy callback that will handle calling the user-specified Python callback
 */
int espeak_ng_proxy_callback(short* wave, int num_samples, espeak_EVENT* event)
{
    // TODO: Does caller own wave lifetime? See if wave is copied in CallFunction

    // TODO: short to char doesn't matter since it's all bytes, right?
    if (SynthCallback == NULL)
	return 0;

    // Callback may be called from non-Python created thread, so
    // ensure to register threads w/ interpretter and acquire the GIL
    // TODO: In synchronous mode for espeak (not espeak-ng) callback
    // is run without spawning a new thread (at least in the Homebrew
    // espeak version) which will cause deadlock if we attempt to
    // Ensure()
    PyGILState_STATE state = PyGILState_Ensure();

    PyObject *wave_py = Py_BuildValue("y#", wave);
    PyObject *num_samples_py = Py_BuildValue("i", num_samples);
    EspeakNgPyEventObject *event_py = PyObject_New(EspeakNgPyEventObject, &ESpeakNgPyEventObjectType);

    // TODO: Extract this building to a separate function
    event_py->type = Py_BuildValue("i", event->type);
    event_py->unique_identifier = Py_BuildValue("I", event->unique_identifier);
    event_py->text_position = Py_BuildValue("i", event->text_position);
    event_py->length = Py_BuildValue("i", event->length);
    event_py->audio_position = Py_BuildValue("i", event->audio_position);
    event_py->sample = Py_BuildValue("i", event->sample);

    if (event->user_data != NULL)
	event_py->user_data = Py_BuildValue("O", event->user_data);
    else
	Py_INCREF(Py_None);
	event_py->user_data = Py_None;

	event_py->id = Py_BuildValue("s", "test");
    if (event->type == espeakEVENT_WORD || event->type == espeakEVENT_SENTENCE)
	event_py->id = Py_BuildValue("i", event->id.number);
    else if (event->type == espeakEVENT_MARK || event->type == espeakEVENT_PLAY)
	event_py->id = Py_BuildValue("s", event->id.name);
    // TODO: Not certain if this is only set during PHONEME events?
    else if (event->type == espeakEVENT_PHONEME)
	// TODO: Idk if s# is correct here...
	event_py->id = Py_BuildValue("#s", event->id.string, 8);
    else {
	Py_INCREF(Py_None);
	event_py->id = Py_None;
    }

    // Result should be either 0 or 1
    // TODO: We should probably hide the need to return a value from users
    PyObject *res_py = PyObject_CallFunctionObjArgs(SynthCallback, wave_py, num_samples_py, event_py, NULL);

    Py_DECREF(event_py);
    Py_DECREF(wave_py);
    Py_DECREF(num_samples_py);

    if (res_py == NULL || !PyLong_Check(res_py)) {
	PyErr_SetString(PyExc_RuntimeError, "espeak_ng_proxy_callback: Callback did not return integer value. Remember callback should return 0 on success.");
    }

    // Convert the Python integer return code to C int
    int res = PyLong_AsLong(res_py);

    // Check for errors during conversion
    if (res == -1 && PyErr_Occurred()) {
	// TODO: What happens when error occurs in async mode? This
	// routine is called by C lib directly
        PyErr_SetString(PyExc_TypeError, "espeak_ng_proxy_callback: Could not parse callback integer value. Remember callback should return 0 on success.");
    }

    PyGILState_Release(state);

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
    size_t size = 0;
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

    // Use 'n' instead of 'I' since (on Darwin) size_t is unsigned long long
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "s|nIiIIOO", kwlist,
				     &text, &size, &position, &position_type, &end_position,
				     &flags, &unique_identifier, &user_data)) {
	return NULL; // Throw exception (it's already set)
    }

    // If no size provided, default to the length of the text.
    if (size == 0)
	size = strlen(text);

    int res = espeak_Synth(text, size, position, POS_CHARACTER, end_position,
			   flags | espeakCHARS_AUTO, unique_identifier, (void *) user_data);

    switch (res) {
    case EE_OK:
	break;
    case EE_INTERNAL_ERROR:
	PyErr_SetString(PyExc_RuntimeError, "espeak_ng_py_Synth: espeak-ng returned EE_INTERNAL_ERROR");
	return NULL; // Throw exception
    case EE_BUFFER_FULL:
	PyErr_SetString(PyExc_RuntimeError, "espeak_ng_py_Synth: espeak-ng returned EE_BUFFER_FULL");
	return NULL;
    case EE_NOT_FOUND:
	PyErr_SetString(PyExc_RuntimeError, "espeak_ng_py_Synth: espeak-ng returned EE_NOT_FOUND");
	return NULL;
    default:
	PyErr_SetString(PyExc_RuntimeError, "espeak_ng_py_Synth: espeak-ng returned unknown error code");
	return NULL;
    }

    Py_RETURN_NONE;
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
	// List doesn't steal reference to voice_list, so we decrement
	// here
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

    int res = espeak_Initialize(output, buflength, path, options);

    if (res == -1) {
	PyErr_SetString(PyExc_RuntimeError, "espeak_ng_py_Initialize: espeak-ng returned EE_INTERNAL_ERROR");
	return NULL; // Throw exception
    }
    // res is sample rate in Hz
    espeak_SetSynthCallback(espeak_ng_proxy_callback);

    // Returns sampling rate in Hz
    PyObject *res_py = Py_BuildValue("i", res); // Caller responsible for decrementing
    return res_py;
}

/*
 * Wrapper function for espean_SetSynthCallback
 */
static PyObject *
espeak_ng_py_SetSynthCallback(PyObject *self, PyObject *args)
{
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
    Py_INCREF(SynthCallback); // Borrow reference

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

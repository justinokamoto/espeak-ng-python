import queue
import sys
import time # TODO: Remove
import threading
import unittest
from unittest import mock
import warnings
from espeak_ng import espeak_ERROR, espeak_AUDIO_OUTPUT
import _espeak_ng as espeak_ng

# TODO: Does the lifetime of a function change if refcount is increased? Or do we need to have:
# if (SynthCallback == NULL || Py_REFCNT(SynthCallback) == 0)?

def dummy_callback_wrapper(queue):
    def dummy_callback(wave, num_samples, event):
        if not wave:
            # TODO: We can't use threading.Condition here b/c segfault
            # occurs when non-Python thread tries to `notify` threads
            # using condition variable
            if queue:
                queue.put(None) # Send sentinel object
        return 0
    
    return dummy_callback

class Test__EspeakNg_Asynchronous(unittest.TestCase):
    def setUp(self):
        # Initialize in asynchronous mode without playback
        espeak_ng.initialize(output=espeak_AUDIO_OUTPUT.AUDIO_OUTPUT_RETRIEVAL)
        assert espeak_ng.set_voice_by_properties()
        # Set dummy callback
        espeak_ng.set_synth_callback(dummy_callback_wrapper(None))

    def test_asynchronous_mode(self):
        text_to_synthesize = "test proxy"
        # Queue to be used as a notification mechanism for when
        # callback is called
        synth_queue = queue.Queue()
        # Set dummy callback that uses queue
        espeak_ng.set_synth_callback(dummy_callback_wrapper(synth_queue))
        # Attempt synthesis
        res = espeak_ng.synth(text_to_synthesize, len(text_to_synthesize))
        # Verify synthesis returns successfully
        assert res == espeak_ERROR.EE_OK, \
            f"Expected {espeak_ERROR.EE_OK.name} but received {espeak_ERROR(res).name}"

        # Wait for callback to send 'sentinel' object to signify synth
        # completed successfully
        synth_queue.get(timeout=3)

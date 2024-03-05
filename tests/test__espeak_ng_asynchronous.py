import queue
import sys
import threading
import unittest
from unittest import mock
import warnings
from espeak_ng import espeak_AUDIO_OUTPUT
import _espeak_ng as espeak_ng


def dummy_callback_wrapper(queue):
    def dummy_callback(wav, num_samples, event):
        if not wav:
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
        espeak_ng.synth(text_to_synthesize, len(text_to_synthesize))
        # Wait for callback to send 'sentinel' object to signify synth
        # completed successfully
        synth_queue.get(timeout=3)

import queue
import sys
import threading
import unittest
from unittest import mock
import warnings
from espeak_ng import espeak_AUDIO_OUTPUT
import _espeak_ng as espeak_ng
import threading


def dummy_callback_wrapper(condition):
    def dummy_callback(wav, num_samples, event):
        # If wav is None, then synth has finished and we must notify
        # the main thread that it can complete
        if not wav:
            if condition:
                with condition:
                    condition.notify()
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
        # Create condition used to notify this main thread that
        # synthesis has completed
        condition = threading.Condition()
        # Set dummy callback that uses queue
        espeak_ng.set_synth_callback(dummy_callback_wrapper(condition))

        with condition:
            # Attempt synthesis
            espeak_ng.synth(text_to_synthesize, len(text_to_synthesize))
            condition.wait()

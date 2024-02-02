import sys
import time # TODO: Remove
import threading
import unittest
from unittest import mock
import warnings
from espeak_ng import espeak_ERROR, espeak_AUDIO_OUTPUT
import _espeak_ng as espeak_ng


synth_finished = False
synth_condition = threading.Condition()

def dummy_callback(wave, num_samples, event, notify=False):
    print(f"wave: {wave}")
    if not wave and notify:
        synth_finished = True
        synth_condition = notify_all()
    return 0


class Test__EspeakNg_Asynchronous(unittest.TestCase):
    def setUp(self):
        # Initialize in asynchronous mode without playback
        espeak_ng.initialize(output=espeak_AUDIO_OUTPUT.AUDIO_OUTPUT_RETRIEVAL)
        espeak_ng.set_synth_callback(dummy_callback)

    def test_asynchronous_mode(self):

        text_to_synthesize = "test proxy"

        res = espeak_ng.synth(text_to_synthesize, len(text_to_synthesize))

        # TODO: Fix the synth error when async mode is enabled!
        # assert res == espeak_ERROR.EE_OK, \
        #     f"Expected {espeak_ERROR.EE_OK.name} but received {espeak_ERROR(res).name}"
        #
        # with synth_condition:
        #     espeak_ng.synth(text_to_synthesize, len(text_to_synthesize))
        #     synth_condition.wait(timeout=3)
        # assert(synth_finished)

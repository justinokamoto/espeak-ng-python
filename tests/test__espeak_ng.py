import sys
import tempfile
import time
import unittest
from unittest import mock
import warnings
from espeak_ng import espeak_ERROR
import _espeak_ng as espeak_ng

class Test__EspeakNg(unittest.TestCase):
    def dummy_callback():
        """Dummy callback that can be used when testing synth callback
        functionality"""
        pass

    def setUp(self):
        espeak_ng.initialize()
        # espeak_ng.set_synth_callback(self.dummy_callback)

    def test_list_voices(self):
        res = espeak_ng.list_voices()
        assert isinstance(res, list)
        assert len(res) > 0, "No voices found. Verify espeak installation."

    """Test _espeak_ng (extension module)"""
    def test_set_voice_by_properties(self):
        # Pick an available voice installed on the system and attempt
        # to configure espeak to use this voice using its properties
        voice = espeak_ng.list_voices()[0]

        # Test using defaults
        assert espeak_ng.set_voice_by_properties()

        # Test setting the various properties
        assert espeak_ng.set_voice_by_properties(name=voice["name"])
        # TODO: Cannot set language?
        # assert(espeak_ng.set_voice_by_properties(languages=voice["languages"]))
        assert espeak_ng.set_voice_by_properties(gender=voice["gender"])
        assert espeak_ng.set_voice_by_properties(age=voice["age"])
        assert espeak_ng.set_voice_by_properties(variant=voice["variant"])

    def test_synth(self):
        text_to_synthesize = "What a wonderful test."
        mock_callback = mock.Mock()

        self.assertRaises(TypeError, espeak_ng.set_synth_callback)
        self.assertRaises(TypeError, espeak_ng.set_synth_callback, 0)

        # Set callback to mock
        espeak_ng.set_synth_callback(mock_callback)

        res = espeak_ng.synth(text_to_synthesize, len(text_to_synthesize))

        # Assert synth works
        assert res == espeak_ERROR.EE_OK
        # Assert that callback was called
        mock_callback.assert_called()

        time.sleep(4) # Remove this! Parse sentinel in callback to
                      # know when synth has completed.


        # TODO: Test unique_identifier and user_data

        

import sys
import time
import unittest
from unittest import mock
from espeak_ng import espeak_ERROR, espeak_AUDIO_OUTPUT
import _espeak_ng as espeak_ng


def dummy_callback(wave, num_samples, event):
    return 0

class Test__EspeakNg(unittest.TestCase):
    def setUp(self):
        # Initialize in a synchronous mode without playback
        espeak_ng.initialize(output=espeak_AUDIO_OUTPUT.AUDIO_OUTPUT_SYNCHRONOUS)
        # Make sure callback is set to function available within
        # module scope (as other tests may have set it)
        espeak_ng.set_synth_callback(dummy_callback)
        assert espeak_ng.set_voice_by_properties()

    def test_list_voices(self):
        res = espeak_ng.list_voices()
        assert isinstance(res, list)
        assert len(res) > 0, "No voices found. Verify espeak installation."

        assert sys.getrefcount(res) == 2, f"Expected ref count of 2 but got {sys.getrefcount(res)}"
        assert sys.getrefcount(res[0]) == 2, f"Expected ref count of 2 but got {sys.getrefcount(res[0])}"

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
        text_to_synthesize = "test synth"

        res = espeak_ng.synth(text_to_synthesize, len(text_to_synthesize))
        # TODO: Test unique_identifier and user_data

    def test_proxy_callback(self):
        text_to_synthesize = "test callback"
        mock_callback = mock.Mock(return_value=0)

        # Assert that arg is required to be callable
        self.assertRaises(TypeError, espeak_ng.set_synth_callback)
        self.assertRaises(TypeError, espeak_ng.set_synth_callback, 0)

        # Set callback to mock
        espeak_ng.set_synth_callback(mock_callback)

        res = espeak_ng.synth(text_to_synthesize, len(text_to_synthesize))

        # Assert that callback was called
        mock_callback.assert_called()

    def test_proxy_callback_parsing_event(self):
        def callback(wave, num_samples, event):
            # Assert all attributes are populated with expected types
            assert isinstance(event.type, int)
            assert isinstance(event.unique_identifier, int)
            assert isinstance(event.text_position, int)
            assert isinstance(event.length, int)
            assert isinstance(event.audio_position, int)
            assert isinstance(event.sample, int)

            return 0

        text_to_synthesize = "test callback"

        espeak_ng.set_synth_callback(callback)

        res = espeak_ng.synth(text_to_synthesize, len(text_to_synthesize))

    def test_event_object(self):
        # Test instantiation
        assert espeak_ng.Event() is not None
        # Test expected attributes exist
        # TODO: Make more efficient
        assert 'audio_position' in dir(espeak_ng.Event)
        assert 'id' in dir(espeak_ng.Event)
        assert 'length' in dir(espeak_ng.Event)
        assert 'sample' in dir(espeak_ng.Event)
        assert 'text_position' in dir(espeak_ng.Event)
        assert 'type' in dir(espeak_ng.Event)
        assert 'unique_identifier' in dir(espeak_ng.Event)
        assert 'user_data' in dir(espeak_ng.Event)

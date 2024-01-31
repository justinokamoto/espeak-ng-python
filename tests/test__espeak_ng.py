import tempfile
import unittest
import warnings
import _espeak_ng as espeak_ng

class Test__EspeakNg_Initialize(unittest.TestCase):
    def setUp(self):
        espeak_ng.initialize()

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


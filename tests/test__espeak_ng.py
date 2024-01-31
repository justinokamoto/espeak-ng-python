import tempfile
import unittest
import _espeak_ng as espeak_ng

class Test_EspeakNg(unittest.TestCase):
    # TODO: in setup load module as global

    """Test _espeak_ng (extension module)"""
    def test_initialize(self):
        res = espeak_ng.initialize()
        assert(isinstance(res, int)) # Verify sampling rate is returned
        assert(res != -1) # Verify error did not occur

    def test_initialize_throws_for_invalid_path(self):
        self.assertRaises(RuntimeError, espeak_ng.initialize, 0, 0, 0, path="")

    def test_set_voice_by_properties(self):
        pass
    


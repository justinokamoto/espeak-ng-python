import tempfile
import unittest
import _espeak_ng as espeak_ng

class Test_EspeakNg(unittest.TestCase):
    """Test _espeak_ng (extension module)"""
    def test_initialize_returns_sampling_rate(self):
        res = espeak_ng.initialize(0, 0, 0)
        assert(isinstance(res, int))
        assert(res != -1) # Default rate is 60Hz

    def test_initialize_throws_for_missing_arguments(self):
        self.assertRaises(TypeError, espeak_ng.initialize)

    def test_initialize_throws_for_invalid_path(self):
        self.assertRaises(RuntimeError, espeak_ng.initialize, 0, 0, 0, "")


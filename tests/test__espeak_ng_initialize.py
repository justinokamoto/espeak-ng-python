import tempfile
import unittest

class Test__EspeakNg_Initialize(unittest.TestCase):
    """Test importing and initializing _espeak_ng (extension
    module). This test module is kept separate since we want to
    exclicitly import _espeak_ng within our tests"""

    def test_import(self):
        import _espeak_ng

    def test_initialize(self):
        import _espeak_ng as espeak_ng

        res = espeak_ng.initialize()
        assert isinstance(res, int) # Verify sampling rate is returned
        assert res != -1 # Verify error did not occur

        # Verify exception is thrown for invalid path
        self.assertRaises(RuntimeError, espeak_ng.initialize, 0, 0, 0, path="")

import unittest

class TestEspeakNgExtension(unittest.TestCase):
    def test_import(self):
        # Test that importing is functional
        import espeak_ng
        assert(espeak_ng.initialize())

import unittest

"""
Convenient entrypoint script for running all unit tests.
Can be run with:

```
$ python -m runtests
```
"""

if __name__ == "__main__":
    loader = unittest.TestLoader()
    suite = loader.discover("tests")
    runner = unittest.TextTestRunner()
    runner.run(suite)

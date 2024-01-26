import unittest

"""
Convenient entrypoint script for running all unit tests.
Can be run with:

```
$ python -m runtests
```
"""

# TODO: How to run with paths set correctly?
if __name__ == "__main__":
    loader = unittest.TestLoader()
    suite = loader.discover("tests")
    runner = unittest.TextTestRunner()
    runner.run(suite)

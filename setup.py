import os
from os import path
from setuptools import setup, Extension
import subprocess

# TODO: Learn distribution of wheel files (for MacOS, Linux, etc.)

# TODO: Warn about submodules missing?
# TODO: Build espeak-ng submodule?
# TODO: Cross-comp for linux, macos, windows
# TODO: Use bear to generate compile_commands.json

# OR should just use the loader path (and have runtime_library_dirs set correctly?)
import distutils

distutils.log.set_verbosity(1)

extension = Extension('_espeak_ng',
                      # TODO: Walk file tree for these files?
                      sources=[path.join('src','espeak_ng','extension','_espeakngmodule.c')],
                      include_dirs=[path.join('espeak-ng', 'src', 'include')],
                      library_dirs=[path.join(os.sep, 'opt','homebrew','lib')],
                      libraries=['espeak'])

setup(ext_modules=[extension])

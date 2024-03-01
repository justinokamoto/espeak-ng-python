import os
from os import path
import platform
from setuptools import setup, Extension
import subprocess

# TODO: Warn about submodules missing?
# TODO: Build espeak-ng submodule within wheel or package prebuild libraries?
library_dirs = []

if platform.system() == "Darwin":
    # TODO: Figure out why homebrew version creates segfaults in
    # tests and remove this path
    library_dirs.append(path.join(path.expanduser("~"), ".local", "lib"))
    # library_dirs.append(path.join(os.sep, "opt", "homebrew", "lib"))

extension = Extension('_espeak_ng',
                      sources=[path.join('src','espeak_ng','extension','_espeakngmodule.c')],
                      include_dirs=[path.join('espeak-ng', 'src', 'include')],
                      library_dirs=library_dirs,
                      libraries=['espeak-ng'])

setup(ext_modules=[extension])

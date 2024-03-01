import os
from os import path
import platform
from setuptools import setup, Extension
import subprocess

# TODO: Warn about submodules missing?
# TODO: Build espeak-ng submodule within wheel or package prebuild libraries?
static_libs = []
library_dirs = []

if platform.system() == "Darwin":
    # TODO: Figure out why homebrew version creates segfaults in
    # tests and remove this path
    static_libs.append(path.join(path.expanduser("~"), ".local", "lib", "libespeak-ng.a"))
    # library_dirs.append(path.join(os.sep, "opt", "homebrew", "lib"))
elif platform.system() == "Windows":
    raise Exception("Windows platform not yet supported.")

extension = Extension("_espeak_ng",
                      sources=[path.join("src","espeak_ng","extension","_espeakngmodule.c")],
                      include_dirs=[path.join(path.dirname(__file__), "espeak-ng", "src", "include")],
                      extra_objects=static_libs,
                      library_dirs=library_dirs)

setup(ext_modules=[extension])

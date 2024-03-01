import os
from os import path
import platform
from setuptools import setup, Extension

# TODO: Warn about submodules missing?
# TODO: Build espeak-ng submodule within wheel or package prebuild libraries?
static_libs = []
dynamic_libs = []
library_dirs = []
if platform.system() == "Darwin":
    # TODO: Figure out why homebrew version creates segfaults in
    # tests and remove this path
    # static_libs.append(path.join(path.expanduser("~"), ".local", "lib", "libespeak-ng.a"))
    # library_dirs.append(path.join(os.sep, "opt", "homebrew", "lib"))
    dynamic_libs.append("espeak")
if platform.system() == "Linux":
    # We can expect that this .so will be installed within one of the
    # default linker search paths
    dynamic_libs.append("espeak")
elif platform.system() == "Windows":
    raise Exception("Windows platform not yet supported.")

extension = Extension("_espeak_ng",
                      sources=[path.join("src","espeak_ng","extension","_espeakngmodule.c")],
                      include_dirs=[path.join(path.dirname(__file__), "espeak-ng", "src", "include")],
                      library_dirs=library_dirs,
                      extra_objects=static_libs,
                      libraries=dynamic_libs)

setup(ext_modules=[extension])

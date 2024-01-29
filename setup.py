from os import path
from setuptools import setup, Extension
import subprocess

# TODO: Warn about submodules missing?
# TODO: Build espeak-ng submodule?
# TODO: Cross-comp for linux, macos, windows
# TODO: Use bear to generate compile_commands.json

# OR should just use the loader path (and have runtime_library_dirs set correctly?)
import distutils

distutils.log.set_verbosity(1)

extension = Extension('espeak_ng',
                      # TODO: Walk file tree for this
                      sources=[path.join('src','espeak_ng','extension','espeakngmodule.c')],
                      include_dirs=[path.join('espeak-ng', 'src', 'include')],
                      # library_dirs=[path.join('espeak-ng', 'src', '.libs')],
                      library_dirs=[path.join('/usr','local','lib')],
                      libraries=['espeak-ng'])

setup(ext_modules=[extension])

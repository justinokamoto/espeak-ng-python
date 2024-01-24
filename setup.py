from os import path
from setuptools import setup, Extension

# TODO: Build the extension (and warn about submodules missing?)

extension = Extension('espeak-ng',
                            # TODO: Walk file tree for this
                            sources=[path.join('src','espeak-ng','extension','espeakngmodule.c')],
                            # TODO: Do better
                            include_dirs=[path.join('espeak-ng', 'src', 'include')])
setup(ext_modules=[extension])

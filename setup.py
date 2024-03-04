import os
from os import path
import platform
import subprocess
from setuptools import setup, Extension

include_dirs = []
library_dirs = []
static_libs = []
dynamic_libs = []

if platform.system() == "Darwin":
    # Attempt to add brew paths if installed on the system
    res = subprocess.run(["brew", "--prefix", "espeak-ng"], capture_output=True)

    if not res.returncode:
        espeak_ng_prefix_path = res.stdout.decode('utf-8').strip()
        include_dirs.append(path.join(espeak_ng_prefix_path, "include"))
        library_dirs.append(path.join(espeak_ng_prefix_path, "lib"))

    dynamic_libs.append("espeak-ng")

if platform.system() == "Linux":
    # We can expect that this .so will be installed within one of the
    # default linker search paths
    # TODO: Replace with espeak-ng?
    dynamic_libs.append("espeak")
elif platform.system() == "Windows":
    raise Exception("Windows platform not yet supported.")

extension = Extension("_espeak_ng",
                      sources=[path.join("src","espeak_ng","extension","_espeakngmodule.c")],
                      include_dirs=include_dirs,
                      library_dirs=library_dirs,
                      extra_objects=static_libs,
                      libraries=dynamic_libs)

setup(ext_modules=[extension])

import os
from os import path
import platform
import subprocess
from setuptools import find_packages, setup, Extension

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
    dynamic_libs.append("espeak-ng")
elif platform.system() == "Windows":
    raise Exception("Windows platform not yet supported.")

extension = Extension("_espeak_ng",
                      sources=[path.join("src","espeak_ng","extension","_espeakngmodule.c")],
                      include_dirs=include_dirs,
                      library_dirs=library_dirs,
                      extra_objects=static_libs,
                      libraries=dynamic_libs)

setup(
    name="espeak-ng-python",
    version="1.0.5",
    author="Justin Okamoto",
    description="Python wrapper for espeak-ng.",
    long_description=open("README.md", "r", encoding="utf-8").read(),
    long_description_content_type="text/markdown",
    keywords="espeak pico festival tts text-to-speech",
    license="GPLv3",
    python_requires=">=3.8",
    classifiers=[
        "Programming Language :: Python :: 3",
        "License :: OSI Approved :: GNU General Public License v3 or later (GPLv3+)",
        "Operating System :: MacOS",
        "Operating System :: POSIX :: Linux"
    ],
    url="https://github.com/justinokamoto/espeak-ng-python",
    package_dir={"": "src"},
    packages=find_packages("src"),
    ext_modules=[extension]
)

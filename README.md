> [!CAUTION]
> This project is still in active development and is minimally functional.


# espeak-ng

A Python bindings library for the [eSpeak NG](https://github.com/espeak-ng/espeak-ng?tab=readme-ov-file) utility, which is a compact open source software text-to-speech synthesizer with distributions for all major operating systems.

## Supported Platforms

TODO: Matrix

## Supported Features

TODO

## Dependencies

### `espeak` library

**Installing MacOS**
```
brew install espeak
```

**Linux**
```
apt install espeak
```

## Installation

After installing dependencies from the above section, you can download this Python package from the Pythin Package Index:

TODO: Upload package to PyPI!

```
pip install espeak-ng-python
```

## Usage

### Quick Start

```
import espeak_ng

# Initialize the espeak library
espeak_ng.initialize()

espeak_ng.synth("Hello, world.")
```

#### Asynchronous Mode

TODO

```
# Set callback to some user-defined callback function
# with the following signature, where the return value
# is 0 on success and 1 on failure:

def my_callback_func(wave: bytes, num_samples: int, event: espeak_ng.Event) -> int:
  # ... my code here
  return 0

espeak_ng.set_synth_callback(my_callback_func)
```

### Configuration

TODO

## Building Locally

Install dependencies from the above "Dependencies" section. Then build this package using `pip`:

```
# Use pip to build in development (`-e` is for 'editable install')
pip install -e .
```

To run tests, use the helper `runtests` script:

```
python3 -m runtests
```

To build a local distribution archive, run:

```
pip wheel .
```


-----------------------------

* TODO: Static link for portability (better than the other implementation)
* TODO: emacs dired configurations for building and lsp stuff
* TODO: Need __init__.py to implement all defaults
* TODO: WAV support
* TODO: Explain in docs that submodule is to retrieve headers
* TODO: Docstrings
* TODO: Try installing and running MacOS and Linux, then upload to PyPI
* TODO: Describe the different responsibilities of \_espeak\_ng and espeak\_ng

```
install_name_tool -change /usr/lib/libpcaudio.0.dylib /usr/local/lib/libpcaudio.0.dylib /Users/justin/workspaces/espeak-ng-python/espeak-ng/src/.libs/espeak-ng
```

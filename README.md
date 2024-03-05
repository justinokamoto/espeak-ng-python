> [!CAUTION]
> This project is still in active development and is minimally functional.


# espeak-ng

A Python bindings library for the [eSpeak NG](https://github.com/espeak-ng/espeak-ng?tab=readme-ov-file) utility, which is a compact open source software text-to-speech synthesizer with distributions for all major operating systems.

This library has been tested for the following platforms:

✅ MacOS Sonoma M1

## Supported Features

TODO

## Dependencies

This library provides Python bindings for the underlying `espeak-ng`
(or `espeak`), so obviously, you must have this library
installed. Follow instructions below for platform-specific
instructions.

**MacOS (using `brew`)**
```
$ brew tap justinokamoto/espeak-ng
$ brew espeak-ng
```

**Linux**
```
$ apt install espeak
```

## Installation

Provided you have installed the required dependencies described from
the previous section, you can download this Python package from the
Pythin Package Index:

TODO: Upload package to PyPI!

```
pip install espeak-ng-python
```

## Usage

### Quick Start

```python
import espeak_ng

# Initialize the espeak library
espeak_ng.initialize()

espeak_ng.synth("Hello, world.")
```

#### Asynchronous Mode


TODO

```python
def my_callback_func(wave: bytes, num_samples: int, event: espeak_ng.Event) -> int:
  # ... my code here
  return 0

# Initialize the espeak library in an asynchronous output mode
espeak_ng.initialize(output=espeak_AUDIO_OUTPUT.AUDIO_OUTPUT_PLAYBACK)

# This function will return immediately and the callback will be triggered as the synthesis
progresses
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

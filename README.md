# espeak-python

TODO: Read and obey espeak license!

TODO: Static link for portability (better than the other implementation)
TODO: emacs dired configurations for building and lsp stuff

TODO: Need __init__.py to implement all defaults

TODO: Test everything

TODO: Make hack for 3rd party lib:
```
install_name_tool -change /usr/lib/libpcaudio.0.dylib /usr/local/lib/libpcaudio.0.dylib /Users/justin/workspaces/espeak-ng-python/espeak-ng/src/.libs/espeak-ng
```

TODO: Remove the espeak submodule (and include its header somewhere)

# Dependencies

https://espeak.sourceforge.net/

## Install MacOS:

Homebrew: https://formulae.brew.sh/formula/espeak

## Install Linux:

## Install Windows:

# Supports
* PLAYBACK mode, not RETRIEVAL. If you just want to play the sound instead of handling the audio data and synthesizing yourself, it works just fine.

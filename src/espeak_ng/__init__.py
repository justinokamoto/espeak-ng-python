from _espeak_ng import *

from enum import IntEnum


class espeak_AUDIO_OUTPUT(IntEnum):
    """For documentation see espeak_AUDIO_OUTPUT enum in espeak-ng's
    public header speak_lib.h

    This enum can be used as a parameter to
    `espeak_ng.initialization()`
    """
    AUDIO_OUTPUT_PLAYBACK=0,
    AUDIO_OUTPUT_RETRIEVAL=1,
    AUDIO_OUTPUT_SYNCHRONOUS=2,
    AUDIO_OUTPUT_SYNCH_PLAYBACK=3


class espeak_EVENT_TYPE(IntEnum):
    """For documentation see espeak_EVENT_TYPE enum in espeak-ng's
    public header speak_lib.h

    The `Event` object passed back within the synth callback contains
    an int field `type` which corresponds to this enum type

    TODO: Define within extension and use within Event object
    """
    espeakEVENT_LIST_TERMINATED = 0, # Retrieval mode: terminates the event list.
    espeakEVENT_WORD = 1,            # Start of word
    espeakEVENT_SENTENCE = 2,        # Start of sentence
    espeakEVENT_MARK = 3,            # Mark
    espeakEVENT_PLAY = 4,            # Audio element
    espeakEVENT_END = 5,             # End of sentence or clause
    espeakEVENT_MSG_TERMINATED = 6,  # End of message
    espeakEVENT_PHONEME = 7,         # Phoneme, if enabled in espeak_Initialize()
    espeakEVENT_SAMPLERATE = 8       # internal use, set sample rate

from enum import IntEnum

"""
Python equivalents of the enums defined within speak_lib.h

TODO: Define within extension?
"""

class espeak_AUDIO_OUTPUT(IntEnum):
    """For documentation see espeak_AUDIO_OUTPUT enum in espeak-ng's
    public header speak_lib.h"""
    AUDIO_OUTPUT_PLAYBACK=0,
    AUDIO_OUTPUT_RETRIEVAL=1,
    AUDIO_OUTPUT_SYNCHRONOUS=2,
    AUDIO_OUTPUT_SYNCH_PLAYBACK=3


class espeak_ERROR(IntEnum):
    """For documentation see espeak_ERROR enum in espeak-ng's public
    header speak_lib.h"""
    EE_OK=0,
    EE_INTERNAL_ERROR=-1,
    EE_BUFFER_FULL=1,
    EE_NOT_FOUND=2
    

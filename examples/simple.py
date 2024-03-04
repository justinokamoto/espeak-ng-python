from espeak_ng import espeak_AUDIO_OUTPUT
import _espeak_ng as espeak_ng
import time


def main():
    text_to_synthesize = "Hello, world!"
    espeak_ng.initialize(output=espeak_AUDIO_OUTPUT.AUDIO_OUTPUT_SYNCH_PLAYBACK)

    espeak_ng.synth(text_to_synthesize, len(text_to_synthesize))


if __name__ == "__main__":
    main()

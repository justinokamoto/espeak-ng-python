import queue
from espeak_ng import espeak_AUDIO_OUTPUT
import espeak_ng
import time

synth_queue = queue.Queue()

def callback(wave, num_samples, event):
    if wave is None:
        synth_queue.put(None)
    return 0

def main():
    text_to_synthesize = "Hello, world!"
    espeak_ng.initialize(output=espeak_AUDIO_OUTPUT.AUDIO_OUTPUT_RETRIEVAL)
    espeak_ng.set_synth_callback(callback)

    espeak_ng.synth(text_to_synthesize)

    # Wait for synth to complete
    synth_queue.get(timeout=3)


if __name__ == "__main__":
    main()

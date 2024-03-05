import queue
from espeak_ng import espeak_AUDIO_OUTPUT
import espeak_ng
import tempfile
import time

"""
This example provides a minimal example of both the asynchronous
interface, as well as the 'retrieval' interface (where audio data is
passed into the callback rather than being synthesized).
"""

synth_queue = queue.Queue()

def callback(wav, num_samples, event):
    with tempfile.NamedTemporaryFile(delete=False, delete_on_close=False) as fp:
        if wav is not None:
            with open(fp.name, mode='ab') as f:
                f.write(wav)
    if wav is None:
        print(f"Wrote wav data to {fp.name}")
        synth_queue.put(None)
    return 0


def main():
    text_to_synthesize = "Hello, world!"
    espeak_ng.initialize(output=espeak_AUDIO_OUTPUT.AUDIO_OUTPUT_RETRIEVAL)
    espeak_ng.set_synth_callback(callback)

    espeak_ng.synth(text_to_synthesize)

    # Wait for synth to complete
    synth_queue.get(timeout=5)


if __name__ == "__main__":
    main()

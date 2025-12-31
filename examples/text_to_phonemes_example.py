import espeak_ng

'''
This example demonstrates how to use text_to_phonemes to convert text to phonemes, without synthetizing the sound.
'''

def main():
    espeak_ng.initialize()
    espeak_ng.set_voice_by_properties(name="pl")
    USE_IPA = 0x02
    SEPARATOR = ord('_') << 8
    phonemes = espeak_ng.text_to_phonemes("żółw", phonememode=USE_IPA | SEPARATOR)
    print(phonemes)
    assert(phonemes == 'ʒ_ˈu_w_f')

if __name__ == "__main__":
    main()


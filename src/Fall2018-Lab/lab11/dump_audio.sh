#!/bin/bash

# The WAV file must be 8-bit audio:
# 
# zpmarvel@phyllite ~/src/Spring2017-lab/lab11 $ file thermo.wav
#   thermo.wav: RIFF (little-endian) data, WAVE audio, Microsoft PCM, 8 bit, mono 11025 Hz
# zpmarvel@phyllite ~/src/Spring2017-lab/lab11 $ file thermo-short-8.wav 
#   thermo-short-8.wav: RIFF (little-endian) data, WAVE audio, Microsoft PCM, 8 bit, mono 11025 Hz
# zpmarvel@phyllite ~/src/Spring2017-lab/lab11 $ file 500hz.wav
#   500hz.wav: RIFF (little-endian) data, WAVE audio, Microsoft PCM, 16 bit, mono 44100 Hz
# zpmarvel@phyllite ~/src/Spring2017-lab/lab11 $ file 500hz-8.wav
#   500hz-8.wav: RIFF (little-endian) data, WAVE audio, Microsoft PCM, 8 bit, mono 44100 Hz


sox 500hz.wav -r 11025 -b 8 500hz-8.wav vol 0.8
xxd -i 500hz-8.wav 500hz.c

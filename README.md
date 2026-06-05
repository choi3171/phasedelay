# PhaseDelay VST3

PhaseDelay - Inspired by enummusic, AllPassPhase.  
It is an optimized version of AllPassPhase.  

A JUCE based VST3 plugin for a less phase-coherent sound.  
There is no custom GUI, so parameter control is handled by the host.  

# Parameters
## Filter Type
Biquad is the default filter of PhaseDelay.  
However, it makes some noises when parameters are changed quickly.  
SVF doesn't have such noises, but is more CPU-intensive than Biquad.  
In most cases, Biquad is sufficient.

## Frequency
Controls the frequency of the filters.

## Q
Controls 'sharpness' of the filters.

## Iterations
Controls how many times the sound passes through the filters.

## Mix
Controls dry/wet mix.

# Installation
Supported on Windows only.  
Download the prebuilt .vst3 from the releases.

# Build

Requires CMake and Git.

```bash
# 1. Configure
cmake -B build

# 2. Build
cmake --build build --config Release
``` 

Output: build/PhaseDelay_artefacts/Release/VST3/PhaseDelay.vst3

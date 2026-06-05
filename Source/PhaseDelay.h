#pragma once
#include <vector>
#include <algorithm>
#include "SVF.h"
#include "Biquad.h"

class PhaseDelay {
private:
    static constexpr int MAX_ITERATIONS = 50;
    static constexpr int MAX_CHANNELS = 8;
    
    std::vector<std::vector<Biquad>> biquads;
    std::vector<std::vector<SVF>> svfs;
    
    bool useSVF = false;
    int currentIterations = 1;

    template <typename FilterType>
    void processInternal(float* const* channelData, int numChannels, int numSamples, float mix,
                         std::vector<std::vector<FilterType>>& filters) {
        const int channelsToProcess = std::min(numChannels, MAX_CHANNELS);

            for (int ch = 0; ch < channelsToProcess; ++ch) {
                float* currentChannel = channelData[ch];
                if (currentChannel == nullptr) continue;

                if (mix >= 0.999f) {
                    for (int s = 0; s < numSamples; ++s) {
                        float sample = currentChannel[s];
                        for (int i = 0; i < currentIterations; ++i) {
                            sample = filters[ch][i].process(sample);
                        }
                        currentChannel[s] = sample; 
                    }
                } 
                else if (mix <= 0.001f) {
                    continue; 
                } 
                else {
                    for (int s = 0; s < numSamples; ++s) {
                        float dry = currentChannel[s]; 
                        float wet = dry;
                        for (int i = 0; i < currentIterations; ++i) {
                            wet = filters[ch][i].process(wet);
                        }
                        currentChannel[s] = dry * (1.0f - mix) + wet * mix;
                    }
                }
            }
        }

public:
    PhaseDelay() {
        biquads.resize(MAX_CHANNELS, std::vector<Biquad>(MAX_ITERATIONS));
        svfs.resize(MAX_CHANNELS, std::vector<SVF>(MAX_ITERATIONS));
    }

    void prepare(double sampleRate) {
        for (int ch = 0; ch < MAX_CHANNELS; ++ch) {
            for (int i = 0; i < MAX_ITERATIONS; ++i) {
                biquads[ch][i].prepare(sampleRate);
                svfs[ch][i].prepare(sampleRate);
            }
        }
    }

    void setConfig(bool svfMode, int iterations) {
        useSVF = svfMode;
        currentIterations = std::max(1, std::min(MAX_ITERATIONS, iterations));
    }

    void updateCoefficients(double freq, double q, int numChannels) {
        int updateChannels = std::min(numChannels, MAX_CHANNELS);

        for (int ch = 0; ch < updateChannels; ++ch) {
            for (int i = 0; i < currentIterations; ++i) {
                if (useSVF) svfs[ch][i].update(freq, q);
                else        biquads[ch][i].update(freq, q);
            }
        }
    }
    
    void processBlock(float* const* channelData, int numChannels, int numSamples, float mix) {
        if (useSVF) {
            processInternal(channelData, numChannels, numSamples, mix, svfs);
        } else {
            processInternal(channelData, numChannels, numSamples, mix, biquads);
        }
    }
};

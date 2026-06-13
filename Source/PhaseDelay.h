#pragma once

#include "Biquad.h"
#include "SVF.h"
#include <JuceHeader.h>

class PhaseDelay {
public:
  static constexpr int MAX_ITERATIONS = 50;
  static constexpr int MAX_CHANNELS = 8;

  PhaseDelay();

  void prepare(double sampleRate);
  void setConfig(bool svfMode, int iterations);
  void updateCoefficients(double freq, double q, int numChannels);
  void processBlock(float *const *channelData, int numChannels, int numSamples,
                    float mix);

private:
  template <typename Filter>
  void processInternal(float *const *channelData, int numChannels,
                       int numSamples, float mix,
                       std::vector<std::vector<Filter>> &filters);

  std::vector<std::vector<Biquad>> biquads;
  std::vector<std::vector<SVF>> svfs;

  bool useSVF = false;
  int currentIterations = 0;
};

#include "PhaseDelay.h"
#include <JuceHeader.h>

#include <algorithm>
#include <chrono>
#include <iostream>
#include <random>
#include <vector>

static void runBench(const char *name, bool useSVF, int iterations,
                     int numChannels, int blockSize, int numBlocks,
                     double sampleRate) {
  PhaseDelay dsp;

  dsp.prepare(sampleRate);
  dsp.setConfig(useSVF, iterations);
  dsp.updateCoefficients(1000.0, 0.707, numChannels);

  std::vector<std::vector<float>> storage(numChannels,
                                          std::vector<float>(blockSize, 0.0f));

  std::vector<float *> channels(numChannels, nullptr);

  std::mt19937 rng(12345);
  std::uniform_real_distribution<float> dist(-0.25f, 0.25f);

  for (int ch = 0; ch < numChannels; ++ch) {
    for (int i = 0; i < blockSize; ++i)
      storage[ch][i] = dist(rng);

    channels[ch] = storage[ch].data();
  }

  {
    juce::ScopedNoDenormals noDenormals;

    for (int i = 0; i < 1000; ++i)
      dsp.processBlock(channels.data(), numChannels, blockSize, 1.0f);
  }

  const auto start = std::chrono::steady_clock::now();

  {
    juce::ScopedNoDenormals noDenormals;

    for (int i = 0; i < numBlocks; ++i)
      dsp.processBlock(channels.data(), numChannels, blockSize, 1.0f);
  }

  const auto end = std::chrono::steady_clock::now();

  const auto elapsedNs =
      std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();

  const double elapsedSeconds = static_cast<double>(elapsedNs) / 1.0e9;
  const double audioSeconds =
      static_cast<double>(numBlocks * blockSize) / sampleRate;

  const double realtimeFactor = elapsedSeconds / audioSeconds;
  const double microsecondsPerBlock =
      elapsedSeconds * 1.0e6 / static_cast<double>(numBlocks);

  const double nsPerSampleChannel =
      static_cast<double>(elapsedNs) /
      static_cast<double>(numBlocks * blockSize * numChannels);

  float checksum = 0.0f;

  for (int ch = 0; ch < numChannels; ++ch)
    for (int i = 0; i < blockSize; ++i)
      checksum += storage[ch][i];

  std::cout << name << " | type=" << (useSVF ? "SVF" : "Biquad")
            << " | iter=" << iterations << " | ch=" << numChannels
            << " | block=" << blockSize
            << " | us/block=" << microsecondsPerBlock
            << " | ns/sample/ch=" << nsPerSampleChannel
            << " | realtime=" << realtimeFactor * 100.0 << "%"
            << " | checksum=" << checksum << "\n";
}

int main() {
  constexpr double sampleRate = 48000.0;
  constexpr int numBlocks = 200000;

  runBench("PhaseDelay", false, 1, 2, 64, numBlocks, sampleRate);
  runBench("PhaseDelay", false, 10, 2, 64, numBlocks, sampleRate);
  runBench("PhaseDelay", false, 50, 2, 64, numBlocks, sampleRate);

  runBench("PhaseDelay", true, 1, 2, 64, numBlocks, sampleRate);
  runBench("PhaseDelay", true, 10, 2, 64, numBlocks, sampleRate);
  runBench("PhaseDelay", true, 50, 2, 64, numBlocks, sampleRate);

  runBench("PhaseDelay", false, 50, 8, 64, numBlocks, sampleRate);
  runBench("PhaseDelay", true, 50, 8, 64, numBlocks, sampleRate);

  runBench("PhaseDelay", false, 50, 2, 512, numBlocks / 4, sampleRate);
  runBench("PhaseDelay", true, 50, 2, 512, numBlocks / 4, sampleRate);

  return 0;
}

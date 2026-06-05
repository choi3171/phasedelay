#pragma once
#include <cmath>
#include <JuceHeader.h>

class Biquad {
private:
    double a1 = 0.0, a2 = 0.0;
    double z1 = 0.0, z2 = 0.0;
    double sampleRate = 44100.0;

public:
    void prepare(double sr) {
        sampleRate = sr;
        z1 = 0.0;
        z2 = 0.0;
    }

    void update(double freqHz, double q) {
        double w0 = juce::MathConstants<double>::twoPi * freqHz / sampleRate;
        double alpha = std::sin(w0) / (2.0 * q);
        double cosw0 = std::cos(w0);
        
        double a0_inv = 1.0 / (1.0 + alpha);
        a1 = -2.0 * cosw0 * a0_inv;
        a2 = (1.0 - alpha) * a0_inv;
    }

    inline float process(float input) {
        double in = static_cast<double>(input);
        double out = in * a2 + z1;
        z1 = in * a1 - out * a1 + z2;
        z2 = in - out * a2;
        return static_cast<float>(out);
    }
};
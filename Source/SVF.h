#pragma once
#include <cmath>
#include <JuceHeader.h>

class SVF {
private:
    double ic1eq = 0.0, ic2eq = 0.0;
    double g = 0.0, k = 0.0, a1 = 0.0, a2 = 0.0, a3 = 0.0;
    double sampleRate = 44100.0;

public:
    void prepare(double sr) {
        sampleRate = sr;
        ic1eq = 0.0;
        ic2eq = 0.0;
    }

    void update(double freqHz, double q) {
        double w0 = juce::MathConstants<double>::pi * freqHz / sampleRate;
        g = std::tan(w0);
        k = 1.0 / q;
        
        a1 = 1.0 / (1.0 + g * (g + k));
        a2 = g * a1;
        a3 = g * a2;
    }

    float process(float input) {
        double v3 = input - ic2eq;
        double v1 = a1 * ic1eq + a2 * v3;
        double v2 = ic2eq + a2 * ic1eq + a3 * v3;
        
        ic1eq = 2.0 * v1 - ic1eq;
        ic2eq = 2.0 * v2 - ic2eq;
       
        double output = input - 2.0 * k * v1;

        return static_cast<float>(output);
    }
};
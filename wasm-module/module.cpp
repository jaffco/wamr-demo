class Phasor {
private:
  float phase = 0.f;
  float frequency = 220.f; // Default to A3
  const float sampleRate = 48000.f;
  float phaseInc = 0.f;

public:
  void setFrequency(float freq) {
    frequency = freq;
    phaseInc = frequency / sampleRate;
  }

  float process() {
    phase += phaseInc;
    if (phase >= 1.f)
      phase -= 1.f;
    return phase;
  }
};

// Buffer-based audio processing function
// This is exported to the host and called with blocks of audio samples
extern "C" void process(const float* input, float* output, int num_samples) {
  static Phasor phasor;
  static bool initialized = false;
  if (!initialized) {
    phasor.setFrequency(1000.f); // 1000 Hz LFO
    initialized = true;
  }

  // Process each sample in the buffer
  for (int i = 0; i < num_samples; i++) {
    output[i] = phasor.process();
  }
}

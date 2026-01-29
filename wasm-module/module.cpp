// Base classes needed by FAUST-generated code
class Meta {
public:
    virtual void declare(const char* key, const char* value) = 0;
    virtual ~Meta() {}
};

class UI {
public:
    virtual void openHorizontalBox(const char* label) = 0;
    virtual void openVerticalBox(const char* label) = 0;
    virtual void closeBox() = 0;
    virtual void declare(float* zone, const char* key, const char* val) = 0;
    virtual void addVerticalSlider(const char* label, float* zone, float init, float min, float max, float step) = 0;
    virtual void addNumEntry(const char* label, float* zone, float init, float min, float max, float step) = 0;
    virtual ~UI() {}
};

class dsp {
public:
    virtual ~dsp() {}
    virtual int getNumInputs() = 0;
    virtual int getNumOutputs() = 0;
    virtual void init(int sample_rate) = 0;
    virtual void compute(int count, float** inputs, float** outputs) = 0;
    virtual void buildUserInterface(UI* ui_interface) = 0;
};

#include "springreverb.cpp"

// Sample rate constant - adjust based on your audio setup
#define SAMPLE_RATE 48000

extern "C" {    

void process(float* input, float* output, int num_samples) {
    static mydsp mDSP;
    static bool firstRun = true;
    if (firstRun) {
        mDSP.init(SAMPLE_RATE);
        firstRun = false;
    }

    // compute() expects an array of channel pointers (float**)
    // For mono input/output, create arrays with single pointers
    float* inputs[1] = {input};
    float* outputs[1] = {output};
    mDSP.compute(num_samples, inputs, outputs);
}

}
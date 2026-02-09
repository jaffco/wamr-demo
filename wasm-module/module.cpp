#include "MarshallModel.h"

// Sample rate constant - adjust based on your audio setup
#define SAMPLE_RATE 48000

// Global state - will be initialized in init function
static bool initialized = false;
static MarshallModelWeights* weights_ptr = nullptr;
static wavenet::RTWavenet<1, 1, Layer1, Layer2>* model_ptr = nullptr;

extern "C" {

// Initialize function that can be called from host if needed
void init() {
    if (!initialized) {
        weights_ptr = new MarshallModelWeights();
        model_ptr = new wavenet::RTWavenet<1, 1, Layer1, Layer2>();
        model_ptr->loadModel(weights_ptr->weights);
        initialized = true;
    }
}

void process(float* input, float* output, int num_samples) {
    // Lazy initialization on first call
    if (!initialized) {
        init();
    }
    
    // Process samples one at a time using the underlying model's forward method
    for (int i = 0; i < num_samples; i++) {
        output[i] = model_ptr->model.forward(input[i]);
    }
}

}
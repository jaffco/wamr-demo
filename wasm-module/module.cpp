#include "Phhhsrrr/gen_exported.cpp"

#define SAMPLE_RATE 48000

extern "C" {

void process(float* input, float* output, int num_samples) {
    static CommonState* state = nullptr;
    if (!state) {
        state = (CommonState*)gen_exported::create(SAMPLE_RATE, num_samples);
    }

    // gen_exported::perform expects arrays of channel pointers
    // Phhhsrrr has 2 inputs (in1, in2) and 1 output (out1)
    t_sample* ins[2] = {(t_sample*)input, (t_sample*)input};
    t_sample* outs[1] = {(t_sample*)output};
    gen_exported::perform(state, ins, 2, outs, 1, num_samples);
}

}

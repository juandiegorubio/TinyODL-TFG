#ifndef LAYER
#define LAYER

#include <Arduino.h>

class Layer {

protected:

    int InputSize, OutputSize;
    float* Output;

    float InitialWeightMax = 0.5;
    float LearningRate;
    float Momentum;

    float sigmoid(float value) {
        return (1.0 / (1.0 + exp(-value)));
    }

    float ReLU(float value) {
        return max(0,value);
    }
    float ReLU_prime(float value) {
        return value > 0 ? 1 : 0;
    }
    
    virtual float activation_function(float value) {
        return ReLU(value);
    }

    virtual float activation_function_prime(float value) {
        return ReLU_prime(value);
    }

public:
    int randomInt(int range=20) {
        int rand = random(range);
        return rand - range/2;
    }
    float randomFloat(int range=100) {
        float rand = float(random(range))/float(range);
        return 2.0 * (rand - 0.5);
    }
    
    virtual float* forward(const float Input[], bool verbose=false);
    virtual float* backward(float gradient[], const float Input[], bool verbose=false);

    virtual void printWeights();
    int getInputSize() { return InputSize; }
    int getOutputSize() { return OutputSize; }
};


/*
class MaxPooling : public Layer {

public:
    MaxPooling(int InputDim[3], int PoolDim[2], int stride[2]) {

    }

    void forward(const float Input[], int InputDim[3]) {

    }

private:
    int PoolHeight, PoolWidth, PoolSize;
    int StrideHeight, StrideWidth;

    float* Output;

};
*/

class Dense : public Layer {

public:
    Dense(int InputSize, int OutputSize, float LearningRate=0.3, float Momentum=0.6);
    void initWeights();
    void printWeights();

    float* forward(const float Input[], bool verbose=false);
    float* backward(float gradient[], const float Input[], bool verbose=false);

private:
    float* Weights;
    float* ChangeWeights;
    float* InputGradient;

};



class Convolutional : public Layer {

public:
    Convolutional(int InputDim[3], int KernelDim[2], int FilterSize, float LearningRate=0.3, float Momentum=0.9);
    void initKernels();

    float* forward(const float Input[], bool verbose=false);
    float* backward(float gradient[], const float Input[], bool verbose=false);

private:
    int InputHeight, InputWidth, InputDepth;
    int KernelHeight, KernelWidth, KernelSize, FilterSize;
    int OutputHeight, OutputWidth;

    float* Bias;
    float* Kernels;
    float* ChangeBias;
    float* ChangeKernels;

    void convolve(const float gradient[], const float Kernel[], float* InputGradient);
    void correlate( float* Output, const float Input[], const float Kernel[], 
                    const int InputHeight, const int KernelHeight, 
                    const int InputWidth, const int KernelWidth);

};

#endif
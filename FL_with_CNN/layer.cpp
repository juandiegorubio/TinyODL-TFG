#include "layer.hpp"
/*
 * Function:  approx_pi 
 * --------------------
 * computes an approximation of pi using:
 *    pi/6 = 1/2 + (1/2 x 3/4) 1/5 (1/2)^3  + (1/2 x 3/4 x 5/6) 1/7 (1/2)^5 +
 *
 *  n: number of terms in the series to sum
 *
 *  returns: the approximate value of pi obtained by suming the first n terms
 *           in the above series
 *           returns zero on error (if n is non-positive)
 */
Dense::Dense(int InputSize, int OutputSize, float LearningRate, float Momentum) 
{
    this->InputSize = InputSize;
    this->OutputSize = OutputSize;
    this->LearningRate = LearningRate;
    this->Momentum = Momentum;

    Weights       = new float[(InputSize+1) * OutputSize];
    ChangeWeights = new float[(InputSize+1) * OutputSize];
    Output        = new float[OutputSize];
    InputGradient = new float[InputSize];

    initWeights();
}
void Dense::initWeights() 
{
    for(int i=0; i < (InputSize+1)*OutputSize; ++i) {
        ChangeWeights[i] = 0.0;
        Weights[i] = randomFloat() * InitialWeightMax;
    }
}
void Dense::printWeights() 
{
    for(int i=0; i< InputSize+1; ++i) {
        for(int o=0; o< OutputSize; ++o) {
            Serial.print("\tWeight "); Serial.print(i);
            Serial.print(","); Serial.print(o);
            Serial.print(": "); Serial.println(Weights[i*OutputSize + o]);
        }
    }
}

float* Dense::forward(const float Input[], bool verbose) 
{
    if(verbose) Serial.println("\t----------Starting dense forward----------");
    for(int o=0; o<OutputSize; ++o) {
        // Bias value
        float Accum = Weights[InputSize*OutputSize + o];
        if(verbose) {
            Serial.print("Accum "); Serial.print(o); Serial.print(": "); 
            Serial.print(double(Weights[InputSize*OutputSize + o]));
        }
        // Weights
        for(int i=0; i<InputSize; ++i) {
            Accum += Input[i] * Weights[i*OutputSize + o];
            if(verbose) {
                Serial.print(" + "); Serial.print(Input[i]);
                Serial.print("*"); Serial.print(double(Weights[i*OutputSize + o]));
            }
        }
        
        Output[o] = activation_function(Accum);
        if(verbose) {
            Serial.print(" = "); Serial.println(Accum);
            Serial.print("Output "); Serial.print(o); 
            Serial.print(": "); Serial.println(Output[o]);
        }
    }
    return Output;
}

float* Dense::backward(float gradient[] , const float Input[], bool verbose) 
{
    for(int i=0; i<InputSize; ++i) {
        InputGradient[i] = 0.0;
        for(int o=0; o<OutputSize; ++o) {
            // Backward the activation function
            gradient[o] = activation_function_prime(gradient[o]);
            // Calc input gradient
            InputGradient[i] += Weights[i*OutputSize + o] * gradient[o]; 
            // Update weights  
            if(verbose) {
                Serial.print("Weight "); Serial.print(i); Serial.print(","); Serial.print(o); Serial.print(": ");
                Serial.print(double(Weights[i*OutputSize + o])); Serial.print(" + ");
                Serial.print(LearningRate); Serial.print("*"); Serial.print(double(Input[i]));
                Serial.print("*"); Serial.print(double(gradient[o]));
                Serial.print(" + "); Serial.print(Momentum); Serial.print("*"); 
                Serial.print(double(ChangeWeights[i*OutputSize + o])); Serial.print(" = ");
            }             
            ChangeWeights[i*OutputSize + o] = LearningRate * Input[i] * gradient[o] + Momentum * ChangeWeights[i*OutputSize + o]; 
            Weights[i*OutputSize + o] = ChangeWeights[i*OutputSize + o];
            if(verbose) {
                Serial.println(double(Weights[i*OutputSize + o]));
            }
        }
        //InputGradient[i] *= Input[i] * (1.0 - Input[i]);
    }

    // Update bias
    for(int o=0; o<OutputSize; ++o) {
        if(verbose) {
            Serial.print("Bias "); Serial.print(o); Serial.print(": ");
            Serial.print(Weights[InputSize*OutputSize + o]); Serial.print(" + ");
            Serial.print(LearningRate); Serial.print("*"); Serial.print(double(gradient[o]));
            Serial.print(" + "); Serial.print(Momentum); Serial.print("*"); 
            Serial.print(ChangeWeights[InputSize*OutputSize + o]); Serial.print(" = ");
        }
        ChangeWeights[InputSize*OutputSize + o] = LearningRate * gradient[o] + Momentum * ChangeWeights[InputSize*OutputSize + o]; 
        Weights[InputSize*OutputSize + o] = ChangeWeights[InputSize*OutputSize + o];
        if(verbose) {
            Serial.println(Weights[InputSize*OutputSize + o]);
        }
    }
    return InputGradient;
}



Convolutional::Convolutional(int InputDim[3], int KernelDim[2], int FilterSize, float LearningRate, float Momentum) 
{
    this->InputHeight = InputDim[0];
    this->InputWidth  = InputDim[1];
    this->InputSize   = InputHeight*InputWidth;
    this->InputDepth  = InputDim[2];

    this->KernelHeight = KernelDim[0];
    this->KernelWidth  = KernelDim[1];
    this->KernelSize   = KernelHeight*KernelWidth;
    this->FilterSize   = FilterSize;

    this->OutputHeight = InputHeight - KernelHeight + 1;
    this->OutputWidth  = InputWidth - KernelWidth + 1;
    this->OutputSize = OutputHeight*OutputWidth;

    this->LearningRate = LearningRate;
    this->Momentum = Momentum;

    Bias          = new float[FilterSize * OutputSize];
    Kernels       = new float[FilterSize * InputDepth * KernelSize];
    ChangeBias    = new float[FilterSize * OutputSize];
    ChangeKernels = new float[FilterSize * InputDepth * KernelSize];
    Output        = new float[FilterSize * OutputSize];

    initKernels();
}
void Convolutional::initKernels() 
{
    for(int i=0; i < FilterSize * OutputSize; ++i) {
        float rand = float(random(100))/100.0;
        Bias[i] = 2.0 * (rand - 0.5) * InitialWeightMax;
    }
    for(int i=0; i < FilterSize * InputDepth * KernelSize; ++i) {
        ChangeKernels[i] = 0.0;
        float rand = float(random(100))/100.0;
        Kernels[i] = 2.0 * (rand - 0.5) * InitialWeightMax;    
    }
}

float* Convolutional::forward(const float Input[], bool verbose) 
{
    // For each output matrix
    for(int f=0; f<FilterSize; ++f) {
        // Fore each output value
        for(int o=0; o<OutputSize; ++o) {
            // First add the bias value
            float Accum = Bias[f*OutputSize + o];
            int rowO = o/OutputWidth;
            int colO = o%OutputWidth;
            // For each input matrix
            for(int d=0; d<InputDepth; ++d) {
                // For each kernel value
                for(int k=0; k<KernelSize; ++k) {
                    int rowK = k/KernelWidth;
                    int colK = k%KernelWidth;
                    int inputPos = d*InputSize + (rowO+rowK)*InputWidth + (colO+colK);  
                    // Add input value times kernel value
                    Accum += Input[inputPos] * Kernels[f*InputDepth*KernelSize + d*KernelSize + k];
                }
            }
            Output[f*OutputSize + o] = Accum;
        }
    }
    return Output;
}
float* Convolutional::backward(float gradient[], const float Input[], bool verbose) 
{
    float* InputGradient = new float[InputDepth * InputSize];
    // For each Output matrix
    for(int f=0; f<FilterSize; ++f) {
        // For each Input matrix
        for(int d=0; d<InputDepth; ++d) {
            // Calculate Input gradient
            convolve(&gradient[f*OutputSize], &Kernels[f*InputDepth*KernelSize + d*KernelSize], &InputGradient[d*InputSize]);
            // Calculate Kernel gradient
            for(int k=0; k<KernelSize; ++k) {
                int rowK = k/KernelWidth;
                int colK = k%KernelWidth;
                float Accum = 0.0;
                for(int o=0; o<OutputSize; ++o) {
                    int rowO = o/OutputWidth;
                    int colO = o%OutputWidth;
                    int whichInput = d*InputSize + (rowO+rowK)*InputWidth + (colO+colK);
                    Accum += Input[whichInput] * gradient[f*OutputSize + o];
                }
                ChangeKernels[f*InputDepth*KernelSize + d*KernelSize + k] = LearningRate * Accum + Momentum * ChangeKernels[f*InputDepth*KernelSize + d*KernelSize + k];
                Kernels[f*InputDepth*KernelSize + d*KernelSize + k] = ChangeKernels[f*InputDepth*KernelSize + d*KernelSize + k];
            }   
        }
        // Calculate Bias gradient
        for(int o=0; o<OutputSize; ++o) {
            ChangeBias[f*OutputSize + o] = LearningRate * gradient[f*OutputSize + o] + Momentum * ChangeBias[f*OutputSize + o];
            Bias[f*OutputSize + o] = ChangeBias[f*OutputSize + o];
        }
    }
    return InputGradient;
}

void Convolutional::convolve(const float gradient[], const float Kernel[], float* InputGradient) 
{
    // Calc new Output Gradient matrix
    int DimOffset = InputHeight - (KernelHeight-1);
    float newOG[(OutputHeight + 2*DimOffset) * (OutputWidth + 2*DimOffset)];
    // For every outer value before the inner matrix => 0
    for(int o=0; o < (DimOffset)*(OutputWidth + 2*DimOffset); ++o)
        newOG[o] = 0;
    // The inner matrix is the OutputGradient matrix, the values before and after inner values => 0
    for(int o=0; o < (OutputHeight)*(OutputWidth + 2*DimOffset); ++o) {
        if(o%(OutputWidth + 2*DimOffset) < DimOffset || o%(OutputWidth + 2*DimOffset) > (OutputWidth + 2*DimOffset - 1)-DimOffset)
            newOG[o] = 0;
        else newOG[o] = gradient[o-DimOffset];
    }
    // For every outer value after the inner matrix => 0
    for(int o=0; o < (DimOffset)*(OutputWidth + 2*DimOffset); ++o)
        newOG[(OutputWidth + 2*DimOffset - 1) - o] = 0;

    // Correlate the Output Gradient matrix with the 180º rotated Kernel matrix
    for(int i=0; i<InputSize; ++i) {
        InputGradient[i] = 0;
        int rowI = i/InputWidth;
        int colI = i%InputWidth;
        for(int k=0; k<KernelSize; ++k) {
            int rowK = k/InputWidth;
            int colK = k%InputWidth;
            int outputPos = (rowI + rowK)*(OutputWidth + 2*DimOffset) + (colI + colK); 
            InputGradient[i] += newOG[outputPos] * Kernel[(KernelSize-1)-k];
        }
    }


}
void Convolutional::correlate(  float* Output, const float Input[], const float Kernel[], 
                                const int InputHeight, const int KernelHeight, 
                                const int InputWidth, const int KernelWidth) 
{
    int OutputHeight = InputHeight - KernelHeight + 1;
    int OutputWidth = InputWidth - KernelWidth + 1;
    int OutputSize = OutputHeight * OutputWidth;
    for(int o=0; o<OutputSize; ++o) {
        float Accum = 0.0;
        int rowO = o/OutputWidth;
        int colO = o%OutputWidth;
        for(int k=0; k<KernelSize; ++k) {
            int rowK = k/KernelWidth;
            int colK = k%KernelWidth;
            int inputPos = (rowO + rowK) * InputWidth + (colO + colK);
            Accum += Input[inputPos] * Kernel[k];
        }
        Output[o] = Accum;
    }
}
#include <Arduino.h>
#include "ModelNN.h"

ModelNN::ModelNN(int maxNumLayers) {
    layers = new Layer*[maxNumLayers];
}

void ModelNN::add(Layer* layer) {
    layers[nLayers] = layer;
    if(nLayers == 0) InputSize = layer -> getInputSize();
    OutputSize = layer -> getOutputSize();
    nLayers++;
}

float* ModelNN::forward(float Input[], float* batchInputs[]) {
    float* output = Input;
    bool store_inputs = batchInputs != nullptr;
    for(int l=0; l < nLayers; ++l) {
        if(store_inputs) batchInputs[l] = output;
        output = layers[l] -> forward(output);
    }
}

void ModelNN::backpropagate(float* gradient, float* Inputs[]) {
    for(int l=nLayers-1; l >= 0; --l) {
        // Serial.print("----- Layer "); Serial.print(nLayers - (l+1)); Serial.println(" -----");
        // for(int o=0; o < OutputSize; ++o) {
        //     Serial.print("Output gradient "); Serial.print(o);
        //     Serial.print(": ");
        //     Serial.println(gradient[o]);
        // }
        gradient = layers[l] -> backward(gradient, Inputs[nLayers - (l+1)], false);
    }
}

void ModelNN::train(int nItems, float X[], float Y[], int epochs, int batch_size, bool verbose) 
{
    float OutputGradient[OutputSize];
    float* batchInputs[batch_size*nLayers];

    for(int epoch=0; epoch < epochs; ++epoch) {
        Error = 0.0;
        for(int i=0; i < nItems; ++i) {
            // Forward input
            int batch = i%batch_size;
            float* input = X + i*InputSize;
            float* output = forward(input, batchInputs + batch*nLayers);

            // Calculate error 
            float* target = Y + i*OutputSize;
            float error = loss -> loss(output, target, OutputSize);
            Error += error;
            // if(verbose) {
            //     Serial.print("\tInput: "); Serial.print(*input);
            //     Serial.print(" ==> Error: "); Serial.print(error);
            //     Serial.print(" = "); Serial.print(*target); 
            //     Serial.print(" - "); Serial.println(*output);
            // }

            // Calculate gradient
            float* gradient = loss -> loss_prime(output, target, OutputSize);
            for(int o=0; o < OutputSize; ++o) 
                OutputGradient[o] += gradient[o];

            // for(int o=0; o < OutputSize; ++o) {
            //     Serial.print("Output gradient "); Serial.print(o);
            //     Serial.print(": ");
            //     Serial.println(gradient[o]);
            // }
            // if(epoch + i == 0) {
            //     for(int l=0; l < nLayers; ++l) {
            //         Serial.print(" ---- Layer "); Serial.print(l); Serial.println(" ---- ");
            //         layers[l] -> printWeights();
            //     }
            // }
            // Backpropagate when batch is completed
            if(batch+1 == batch_size || i+1 == nItems) {
                for(int o=0; o < OutputSize; ++o) 
                    OutputGradient[o] /= batch+1;
                for(int k=0; k < batch+1; ++k)
                    backpropagate(OutputGradient, batchInputs + k*nLayers);
            }

            // if(epoch + i == 0) {
            //     for(int l=0; l < nLayers; ++l) {
            //         Serial.print(" ---- Layer "); Serial.print(l); Serial.println(" ---- ");
            //         layers[l] -> printWeights();
            //     }
            // }
            // Reset output gradient
            if(batch == 0)
                for(int o=0; o < OutputSize; ++o) OutputGradient[o] = 0.0;
        }

        // Calculate the mean errors
        Error /= 2*nItems;
        if(verbose) {
            Serial.print("Epoch: "); Serial.print(epoch); 
            Serial.print(" ==> Error: "); Serial.println(double(Error));
        }
    }
}

float* ModelNN::test(int nItems, float X[], float Y[], bool verbose) {
    float* errors = new float[nItems];
    for(int i=0; i < nItems; ++i) {
        float* input = X + i*InputSize;
        float* output = forward(input, nullptr);

        // Calculate error 
        float* target = Y + i*OutputSize;
        errors[i] = loss -> loss(output, target, OutputSize);
        if(verbose) {
            Serial.print("Input: "); Serial.print(*input);
            Serial.print("\tOutput: "); Serial.print(*output);
            Serial.print("\tTarget: "); Serial.print(*target);
            Serial.print("\tError: "); Serial.println(errors[i]);
        }
    }
    return errors;
}

float* ModelNN::predict(float Input[]) {
    return forward(Input, nullptr);
}
#ifndef ModelNN_H
#define ModelNN_H

#include "loss.hpp"
#include "layer.hpp"


static MSE loss_function = MSE();

class ModelNN {
    public:
        int nLayers=0;

        ModelNN(int maxNumLayers);
        void add(Layer* layer);

        void train( int nItems, float X[], float Y[], 
                    int epochs=1, int batch_size=1, bool verbose=false );
        float* test(int nItems, float X[], float Y[], bool verbose=false);
        float* predict(float Input[]);

    private:
        float Error;

        int InputSize;
        int OutputSize;

        Loss* loss = &(loss_function);
        Layer** layers;

        float* forward(float Input[], float* batchInputs[]);
        void backpropagate(float* gradient, float* Input[]);                
};


#endif
#ifndef LOSS
#define LOSS

class Loss {
public:
    virtual float loss(const float output[], const float target[], int numItems) = 0;
    virtual float* loss_prime(const float output[], const float target[], int numItems) = 0;

};

class MSE : public Loss {
public:
    MSE() {};
    float loss(const float output[], const float target[], int numItems) {
        float loss = 0.0;
        for(int i=0; i<numItems; ++i)
            loss += (target[i]-output[i])*(target[i]-output[i]);
        loss /= numItems;
        return loss;
    }
    float* loss_prime(const float output[], const float target[], int numItems) {
        float* gradient = new float;
        for(int i=0; i<numItems; ++i)
            gradient[i] = 2 * (target[i]-output[i]) / numItems;
        return gradient;
    }
};
#endif
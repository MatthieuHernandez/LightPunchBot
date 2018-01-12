#ifndef NEURON_H
#define NEURON_H

#include <iostream>

using namespace std;


class Neuron
{
    public:

        float *weight;
        float learning_rate;
        int numberOfWeight;
        float voltage;
        float threshold;


        Neuron(int numberOfWeight, float w[], float lr);

        bool receiveSpike(int weightNumber);

   private:
        void changeWeight(int lastWeightThatReceiveSpike);


};

#endif // NEURON_H

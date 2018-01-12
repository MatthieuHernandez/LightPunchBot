#include "neuron.h"


Neuron::Neuron(int nbw, float w[], float lr)
{
    numberOfWeight = nbw;
    weight = new float[numberOfWeight];
    for(int i = 0; i < numberOfWeight; i++)
    {
        weight[i] = w[i];
    }
    learning_rate = lr;
    voltage = 0;
    threshold = 1;
}

bool Neuron::receiveSpike(int weightNumber)
{
    voltage += weight[weightNumber];

    if(voltage >= threshold)
    {
        changeWeight(weightNumber);
        return true;
    }
    else
        return false;
}

void Neuron::changeWeight(int lastWeightThatReceiveSpike)
{
    for(int i = 0; i < numberOfWeight; i++)
    {
        if(i == lastWeightThatReceiveSpike)
            weight[i] += learning_rate;
        else
            weight[i] -= learning_rate;
    }
}

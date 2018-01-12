#include "neuralNetworkForCPU.h"
#include "neuralNetwork.h"

//=====================================================================
//  Getters and setters
//=====================================================================

void NeuralNetworkForCPU::setLearningRate(float learningRate)
{
    this->learningRate = learningRate;
}
float NeuralNetworkForCPU::getLearningRate() const
{
    return learningRate;
}

void NeuralNetworkForCPU::setMomentum(float value)
{
    if(value >= 0.0f && value <= 1.0f)
    {
        this->momentum = value;
    }
    else
    {
        std::cout << "ERROR : Momentum must be in 0 and 1, 0 for don't use momentum." << std::endl;
    }
}
float NeuralNetworkForCPU::getMomentum() const
{
    return this->momentum;
}

void NeuralNetworkForCPU::setNumberOfSameClusteringAfterReset(unsigned int number)
{
    numberOfSameClusteringAfterReset = number;
}
int NeuralNetworkForCPU::getNumberOfSameClusteringAfterReset() const
{
   return numberOfSameClusteringAfterReset;
}

void NeuralNetworkForCPU::setLenghtOfShortRuns(unsigned int lenght)
{
    lenghtOfShortRuns = lenght;
}

int NeuralNetworkForCPU::getLenghtOfShortRuns() const
{
    return lenghtOfShortRuns;
}


void NeuralNetwork::setUseGPU(bool value)
{
    if(useCUDA != value)
    {
        useCUDA = value;
        if(useCUDA == true)
            loadNeuralNetworkOnGPU();
    }
}

bool NeuralNetwork::getUseGPU() const
{
    return useCUDA;
}

//=====================================================================
//  Only getters
//=====================================================================

int NeuralNetworkForCPU::getShortRunCounter() const
{
    return shortRunCounter;
}

int NeuralNetworkForCPU::getNumberOfInputs() const
{
    return numberOfInput;
}

int NeuralNetworkForCPU::getNumberOfHiddenLayers() const
{
    return numberOfHiddenLayers;
}

int NeuralNetworkForCPU::getNumberOfNeuronsInHiddenLayers() const
{
    return numberOfNeuronsInHiddenLayers;
}

int NeuralNetworkForCPU::getNumberOfResultsClassifiedWell() const
{
    return numberOfResultsClassifiedWell;
}

int NeuralNetworkForCPU::getNumberOfNegativeResultsMisclassefied() const
{
    return numberOfResultsMisclassefied;
}

int NeuralNetworkForCPU::getNumberOfOutputs() const
{
    return numberOfOutput;
}

float NeuralNetworkForCPU::getClusteringRate() const
{
    return clusteringRate;
}

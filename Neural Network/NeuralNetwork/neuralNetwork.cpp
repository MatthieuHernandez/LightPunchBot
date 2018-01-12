#include "neuralNetwork.h"

NeuralNetwork::NeuralNetwork()
{

}

NeuralNetwork::~NeuralNetwork()
{
}

NeuralNetwork::NeuralNetwork(int numberOfInputs, int numberOfHiddenLayers, int numberOfNeuronsInHiddenLayers, int numberOfOutputs)
   : NeuralNetworkForCPU(numberOfInputs, numberOfHiddenLayers,numberOfNeuronsInHiddenLayers, numberOfOutputs)
{
    this->setUseGPU(false);
}

void NeuralNetwork::loadNeuralNetworkOnGPU()
{
    cudaMalloc((void**)&outputsInInputLayerGPU,          sizeof(float) * this->numberOfNeuronsInHiddenLayers);
    cudaMalloc((void**)&outputsInHiddenLayersGPU,        sizeof(float) * this->numberOfNeuronsInHiddenLayers * (this->numberOfHiddenLayers-1));
    cudaMalloc((void**)&outputsInOutputLayerGPU,         sizeof(float) * this->numberOfOutput);

    cudaMalloc((void**)&errorsInInputLayerGPU,           sizeof(float) * this->numberOfNeuronsInHiddenLayers);
    cudaMalloc((void**)&errorsInHiddenLayersGPU,         sizeof(float) * this->numberOfNeuronsInHiddenLayers * (this->numberOfHiddenLayers-1));
    cudaMalloc((void**)&errorsInOutputLayerGPU,          sizeof(float) * this->numberOfOutput);

    cudaMalloc((void**)&weightsInInputLayerGPU,          sizeof(float) * this->numberOfNeuronsInHiddenLayers * this->numberOfInput);
    cudaMalloc((void**)&weightsInHiddenLayersGPU,        sizeof(float) * this->numberOfNeuronsInHiddenLayers * this->numberOfNeuronsInHiddenLayers * (this->numberOfHiddenLayers-1));
    cudaMalloc((void**)&weightsInOutputLayerGPU,         sizeof(float) * this->numberOfOutput * this->numberOfNeuronsInHiddenLayers);

    outputsInOutputLayerCPU = (float*)malloc(sizeof(float) * this->numberOfOutput);

    unsigned int l, n, w;

    for(n = 0; n < numberOfNeuronsInHiddenLayers; n++)
    {
        for(w = 0; w < numberOfInput; w++)
        {
            CUDA::setWeightInInputLayerFromCPU(weightsInInputLayerGPU, numberOfInput, n, w, neurons[0][n].getWeight(w));
        }
    }
    for(l = 1; l < numberOfHiddenLayers; l++)
    {
        for(n = 0; n < numberOfNeuronsInHiddenLayers; n++)
        {
            for(w = 0; w < numberOfNeuronsInHiddenLayers; w++)
            {
                CUDA::setWeightInHiddenLayersFromCPU(weightsInHiddenLayersGPU, numberOfNeuronsInHiddenLayers, l, n, w, neurons[l][n].getWeight(w));
            }
        }
    }
    for(n = 0; n < numberOfOutput; n++)
    {
        for(w = 0; w < numberOfNeuronsInHiddenLayers; w++)
        {
            CUDA::setWeightInOutputLayerFromCPU(weightsInOutputLayerGPU, numberOfNeuronsInHiddenLayers, n, w, neurons[numberOfHiddenLayers][n].getWeight(w));
        }
    }
}

void NeuralNetwork::loadAllDatabaseOnGPU()
{

}

void NeuralNetwork::loadneuralNetworkOnCPU()
{
    ;
}

vector<float> NeuralNetwork::outputFloat(const vector<float> &inputs)
{
    if(useCUDA == false)
        return NeuralNetworkForCPU::outputFloat(inputs);
    else
    {
        CUDA::calculOutput(weightsInInputLayerGPU,
                           weightsInHiddenLayersGPU,
                           weightsInOutputLayerGPU,
                           outputsInInputLayerGPU,
                           outputsInHiddenLayersGPU,
                           outputsInOutputLayerGPU,
                           numberOfHiddenLayers,
                           numberOfNeuronsInHiddenLayers,
                           numberOfInput,
                           numberOfOutput,
                           (float*)&inputs[0]);

        CUDA::returnOutputOnCPU(outputsInOutputLayerCPU, outputsInOutputLayerGPU, numberOfOutput);

        vector<float> result(numberOfOutput);
        thrust::copy(&outputsInOutputLayerCPU[0], &outputsInOutputLayerCPU[numberOfOutput], result.begin()); // It works well.

        return result;
    }
}

void NeuralNetwork::train(const vector<float> &inputs, const vector<float> &desired)
{
    if(useCUDA == false)
    {
        NeuralNetworkForCPU::train(inputs, desired);
    }
    else
    {
        CUDA::backpropagation(weightsInInputLayerGPU,
                              weightsInHiddenLayersGPU,
                              weightsInOutputLayerGPU,
                              outputsInInputLayerGPU,
                              outputsInHiddenLayersGPU,
                              outputsInOutputLayerGPU,
                              errorsInInputLayerGPU,
                              errorsInHiddenLayersGPU,
                              errorsInOutputLayerGPU,
                              numberOfHiddenLayers,
                              numberOfNeuronsInHiddenLayers,
                              numberOfInput,
                              numberOfOutput,
                              learningRate,
                              (float*)&inputs[0],
                              (float*)&desired[0]);
    }
}

void NeuralNetwork::train(const vector<vector<float>> &inputs, const vector<vector<float>> &desired)
{
    if(useCUDA == false)
    {
        NeuralNetworkForCPU::train(inputs, desired);
    }
    else
    {
        cout << "train with multiple data doesn't work for the moment ..." << endl;
    }
}

void NeuralNetwork::trainOnAllDataBase(const vector<vector<float>> &inputsLearning, const vector<vector<float>> &desiresLearning,
                                       const vector<vector<float>> &inputsTesting, const vector<vector<float>> &desiresTesting,
                                       const int numberOfTrain, const int frequencyOfClusteringRateVerification)
{
    if(useCUDA == true)
    {
        if(inputsLearning.size() != desiresLearning.size()
        && inputsTesting.size() != desiresTesting.size())
        {
            cout << "size of sets error" << endl;
        }
        else
        {
            loadAllDatabaseOnGPU();

            int sizeOfLearningSet = inputsLearning.size();
            int sizeOfTestingSet = inputsTesting.size();

            CUDA::TrainWithAllDatabase(weightsInInputLayerGPU,
                                       weightsInHiddenLayersGPU,
                                       weightsInOutputLayerGPU,
                                       outputsInInputLayerGPU,
                                       outputsInHiddenLayersGPU,
                                       outputsInOutputLayerGPU,
                                       errorsInInputLayerGPU,
                                       errorsInHiddenLayersGPU,
                                       errorsInOutputLayerGPU,
                                       numberOfHiddenLayers,
                                       numberOfNeuronsInHiddenLayers,
                                       numberOfInput,
                                       numberOfOutput,
                                       learningRate,
                                       (float*)&inputsLearning[0],
                                       (float*)&inputsTesting[0],
                                       (float*)&desiresLearning[0],
                                       (float*)&desiresTesting[0],
                                       sizeOfLearningSet,
                                       sizeOfTestingSet,
                                       clusteringRate,
                                       numberOfTrain,
                                       frequencyOfClusteringRateVerification);
        }
}
    else
    {
        cout << "train with all data doesn't work on GPU for the moment ..." << endl;
        /*int random = randomBetween(0, numberOfTrain);

        for(int i = 0; i < numberOfTrain; i++)
        {
            for(j = 0; j < num; i++, j++)
            {
                train(inputsLearning[random], desiresLearning[random]);
            }

            calculateClusteringRate();

            if(newClusteringRate > clusteringRate)
            {
                clusteringRate = newClusteringRate;
                //save();
                printf("Clustering rate = %f", clusteringRate);
            }
        }*/
    }
}

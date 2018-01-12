#ifndef NEURALNETWORKWITHCUDA_H
#define NEURALNETWORKWITHCUDA_H

#include "neuralNetworkForCPU.h"
#include "optimization.cuh"

class NeuralNetwork : public NeuralNetworkForCPU
{
    public :

        ~NeuralNetwork();
        NeuralNetwork();
        NeuralNetwork(int numberOfInput, int numberOfHiddenLayers, int numberOfNeuronsInHiddenLayers, int numberOfOutput);

        vector<float> outputFloat(const vector<float> &inputs);

        void train(const vector<float> &inputs, const vector<float> &desired);
        void train(const vector<vector<float>> &inputs, const vector<vector<float>> &desired);
        void trainOnAllDataBase(const vector<vector<float>> &inputsLearning, const vector<vector<float>> &desiresLearning,
                                const vector<vector<float>> &inputsTesting, const vector<vector<float>> &desiresTesting,
                                const int numberOfTrain, const int frequencyOfClusteringRateVerification);

        bool getUseGPU() const;
        void setUseGPU(bool value);


//=====================================================================
// GPU varaibles
//=====================================================================

    private :

        float* weightsInInputLayerGPU;
        float* weightsInHiddenLayersGPU;
        float* weightsInOutputLayerGPU;
        float* outputsInInputLayerGPU;
        float* outputsInHiddenLayersGPU;
        float* outputsInOutputLayerGPU;
        float* errorsInInputLayerGPU;
        float* errorsInHiddenLayersGPU;
        float* errorsInOutputLayerGPU;

        float* outputsInOutputLayerCPU;

        bool useCUDA;

        void loadNeuralNetworkOnGPU();
        void loadneuralNetworkOnCPU();
        void loadAllDatabaseOnGPU();
};

#endif // NEURALNETWORKWITHCUDA_H

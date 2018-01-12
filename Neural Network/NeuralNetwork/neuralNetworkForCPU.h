#ifndef NEURAL_NETWORK_H
#define NEURAL_NETWORK_H

#include "perceptron.h"
#include <time.h>

class NeuralNetworkForCPU
{
    private :

        static bool isTheFirst;

        int lastError;
        float previousClusteringRate;
        float error;
        float previousError;
        float momentum;

        int numberOfResultsClassifiedWell;
        int numberOfResultsMisclassefied;

        int lenghtOfShortRuns;
        int shortRunCounter;
        int numberOfSameClusteringAfterReset;
        int counterOfSameClusteringAfterReset;

        vector<vector<float>> results;
        vector<float> binaryResult;
        vector<vector<float>> errors;

        void backpropagationAlgorithm(const vector<vector<float>> &inputs, const vector<vector<float> > &desired);
        void calculateError(const int neuronNumberLayer, const int neuronNumber);
        void geneticModification(float probability = 1.0f);

        void calculateOutput(const vector<float> &inputs);
        vector<float> binaryOutput(const vector<float> &inputs);

        bool train(const vector<vector<float>> &inputs, const vector<vector<float>> &desired, const bool isTrainnig);

        void resetAllNeurons();

        vector<float> averageInputs;
        vector<float> averageDesired;
        vector<float> countDesired;
        vector<float> averageOutput;
        vector<float> out;


    protected :

        unsigned int numberOfInput;
        unsigned int numberOfHiddenLayers;
        unsigned int numberOfLayers;
        unsigned int numberOfNeuronsInHiddenLayers;
        unsigned int numberOfOutput;

        float learningRate;
        float clusteringRate;

        vector<vector<Perceptron>> neurons;

        NeuralNetworkForCPU();
        NeuralNetworkForCPU(int numberOfInput, int numberOfHiddenLayers, int numberOfNeuronsInHiddenLayers, int numberOfOutput = 1, float learningRate = 0.05f);

        bool train(const vector<float> &inputs, const vector<float> &desired); // return true if it has been trained
        bool train(const vector<vector<float>> &inputs, const vector<vector<float>> &desired); // return true if it has been trained


    public :

        vector<float> outputFloat(const vector<float> &inputs);
        bool calculateClusteringRate(const vector<float> &inputs, const vector<float> &desired); // return true if it has right
        void resetCalculationOfClusteringRate();
        void addANeuron(unsigned int layerNumber, bool isVirgin = true);
        string display();

        int isValid();

        int getLastError();

        void setLearningRate(float learningRate);
        float getLearningRate() const;
        void setMomentum(float value);
        float getMomentum() const;
        void setNumberOfSameClusteringAfterReset(unsigned int number);
        int getNumberOfSameClusteringAfterReset() const;
        void setLenghtOfShortRuns(unsigned int lenght);

        int getLenghtOfShortRuns() const;
        int getShortRunCounter() const;
        int getNumberOfInputs() const;
        int getNumberOfHiddenLayers() const;
        int getNumberOfNeuronsInHiddenLayers() const;
        int getNumberOfResultsClassifiedWell() const;
        int getNumberOfNegativeResultsMisclassefied() const;
        int getNumberOfOutputs() const;
        float getClusteringRate() const;

        bool operator==(const NeuralNetworkForCPU &neuralNetwork);
        bool operator!=(const NeuralNetworkForCPU &neuralNetwork);
        //void equals(const NeuralNetwork &neuralNetwork);
};

#endif // NEURAL_NETWORK_H

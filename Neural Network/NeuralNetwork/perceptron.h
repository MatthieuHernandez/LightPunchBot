#ifndef PERCEPTRON_H
#define PERCEPTRON_H

#include <vector>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <cmath>

using namespace std;

inline
static int randomBetween(int a, int b)  // WARNING TO : b excluded
{
    return rand()%(b-a)+a;
}

class Perceptron
{
    private :

        vector<float> weights;
        vector<float> deltaWeights;
        vector<float> previousDeltaWeights;

        float bias;
        int numberOfInputs;

        int layerNumber;
        int numberInLayer;
        float previousError;
        float sum;

        void train(const vector<float> &inputs, const float &error, const float &learningRate, const int &momentum);


    public :

        Perceptron(unsigned int numberOfInputs, int layerNumber, int numberInLayer, bool isVirgin = false);
        int output(const vector<float> &inputs);
        float outputFloat(const vector<float> &inputs);
        void trainWithDesired(const vector<float> &inputs, float &desired, const float &learningRate, const int &momentum = 0);
        void trainWithError(const vector<float> &inputs, float &error, const float &learningRate, const int &momentum = 0);

        void activationFunction(float &x);
        float inverseOfTheActivationFunction(const float &x);

        static float derivativeOfActivationFunction(float &x);

        void addAWeight(bool isVirgin = true);
        int isValid();
        string display();

        vector<float> getWeights();
        void setWeights(const vector<float> &weights);

        float getWeight(int i);
        void setWeight(int i, float weight);

        float getBias();
        void setBias(float bias);

        int getNumberOfInputs();
        //void setNumberOfInputs(int numberOfInputs);

        int getLayerNumber();
        int getNumberInLayer();

        bool operator==(const Perceptron &perceptron);
        bool operator!=(const Perceptron &perceptron);
        //void operator=(const Perceptron &perceptron);
};
#endif // PERCEPTRON_H

#include "perceptron.h"
#define MAX 1000000.0f


Perceptron::Perceptron(unsigned int numberOfInputs, int layerNumber, int numberInLayer, bool isVirgin)
{
    this->numberOfInputs = numberOfInputs;
    this->layerNumber = layerNumber;
    this->numberInLayer = numberInLayer;
    this->previousError = 0.0f;

    for (unsigned int i = 0; i < numberOfInputs; i++)
    {
        //if(isVirgin == false)
        //{
            weights.push_back(rand()/(float)RAND_MAX * 2.0f - 1.0f);
            deltaWeights.push_back(0);
            previousDeltaWeights.push_back(0);
        //}
        //else
        //{
            //weights.push_back( (rand()/(float)RAND_MAX * 4 - 2) /0.01 );// between -2/N and +2/N
        //}
    }
    this->bias = 1.0f;
}

//cannot be inline
inline
void Perceptron::activationFunction(float &x)
{
    x = tanh(x);
}

//cannot be inline and NO USED
float Perceptron::inverseOfTheActivationFunction(const float &x)
{
    return (float)log((1/x)-1)/(-2);
}

float Perceptron::derivativeOfActivationFunction(float &x)
{
    return 1-x*x;
}

int Perceptron::output(const vector<float> &inputs)
{
    sum = 0;
    for (int i = 0; i < numberOfInputs; i++)
    {
        sum += inputs[i] * weights[i];
    }
    sum += bias;
    activationFunction(sum);

    if(sum > 0)
        return 1;
    else
        return 0;
}

float Perceptron::outputFloat(const vector<float> &inputs)
{
    sum = 0;
    for (int i = 0; i < numberOfInputs; i++)
    {
        sum += inputs[i] * weights[i];
    }
    sum += bias;
    activationFunction(sum);
    return sum;
}

inline
void Perceptron::train(const vector<float> &inputs, const float &error, const float &learningRate, const int &momentum)
{
    for (int i = 0; i < numberOfInputs; i++)
    {
        deltaWeights[i] = learningRate * error * inputs[i] + momentum * previousDeltaWeights[i];
        weights[i] +=  deltaWeights[i];
        previousDeltaWeights[i] = deltaWeights[i];

        if(weights[i] < -MAX)
        {
             weights[i] = -MAX;
        }
        else if(weights[i] > MAX)
        {
             weights[i] = MAX;
        }
    }
}

void Perceptron::trainWithError(const vector<float> &inputs, float &error, const float &learningRate, const int &momentum)
{
    train(inputs, error, learningRate, momentum);
}

void Perceptron::trainWithDesired(const vector<float> &inputs, float &desired, const float &learningRate, const int &momentum)
{
    int guess = output(inputs);
    float error = desired - guess;
    train(inputs, error, learningRate, momentum);
}

void Perceptron::addAWeight(bool isVirgin)
{
    numberOfInputs ++;
    if(isVirgin)
        weights.push_back(0);
    else
        weights.push_back((rand()/(float)RAND_MAX * 2 - 1));
}

int Perceptron::isValid()
{
    if(bias != 1 && bias != 0)
    {
        cout << bias << endl;
        return 4;
    }
    if(numberOfInputs < 1 || numberOfInputs != weights.size())
    {
        cout << numberOfInputs << endl;
        cout << weights.size() << endl;
        cout << "" << endl;
        return 6;
    }
    for(int i = 0; i < numberOfInputs; i++)
    {
        if(weights[i] < -MAX || weights[i] > MAX)
        {
            return 7;
        }
    }

    return 0;
}

vector<float> Perceptron::getWeights()
{
    return weights;
}

void Perceptron::setWeights(const vector<float> &weights)
{
    this->weights = weights;
}

float Perceptron::getWeight(int i)
{
    return weights[i];
}

void Perceptron::setWeight(int i, float weight)
{
    this->weights[i] = weight;
}

float Perceptron::getBias()
{
    return bias;
}

void Perceptron::setBias(float bias)
{
    this->bias = bias;
}

int Perceptron::getNumberOfInputs()
{
    return numberOfInputs;
}

int Perceptron::getLayerNumber()
{
    return layerNumber;
}

int Perceptron::getNumberInLayer()
{
    return numberInLayer;
}

bool Perceptron::operator==(const Perceptron &perceptron)
{
    if(this->bias != perceptron.bias
    || this->numberOfInputs != perceptron.numberOfInputs
    || this->layerNumber != perceptron.layerNumber
    || this->numberInLayer != perceptron.numberInLayer
    || this->weights.size() != perceptron.weights.size())
        return false;
    for(int i = 0; i < numberOfInputs; i++)
        if(this->weights[i] != perceptron.weights[i])
            return false;
    return true;
}

bool Perceptron::operator!=(const Perceptron &perceptron)
{
    if(this->operator==(perceptron))
        return false;
    else
        return true;
}

string Perceptron::display()
{
    string str = "Weight : ";
    for(int i = 0; i < weights.size(); i++)
    {
        str += to_string(weights[i]) + " ";
    }
    str += "Bias : " + to_string(bias);
    return str;
}


/*void Perceptron::operator=(const Perceptron &perceptron)
{
    this->bias = perceptron.bias;
    this->learningRate = perceptron.learningRate;
    this->startingLearningRate = perceptron.startingLearningRate;
    this->numberOfInputs = perceptron.numberOfInputs;
    this->layerNumber = perceptron.layerNumber;
    this->numberInLayer = perceptron.numberInLayer;

    this->weights = new float[perceptron.numberOfInputs];
    for(int i = 0; i < perceptron.numberOfInputs; i++)
    {
        weights[i] = perceptron.weights[i];
    }
}
*/

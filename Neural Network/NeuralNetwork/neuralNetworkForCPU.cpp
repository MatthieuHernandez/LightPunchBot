#include "neuralNetworkForCPU.h"

bool NeuralNetworkForCPU::isTheFirst = true;

NeuralNetworkForCPU::NeuralNetworkForCPU()
{

}

NeuralNetworkForCPU::NeuralNetworkForCPU(int numberOfInputs, int numberOfHiddenLayers, int numberOfNeuronsInHiddenLayers, int numberOfOutputs, float learningRate)
{
    // IS VIRGIN
    if(isTheFirst == true)
    {
        srand(time(NULL));
        rand();
        isTheFirst = false;
    }
    this->learningRate = learningRate;
    this->lenghtOfShortRuns = 0;
    this->shortRunCounter = 0;
    this->lastError = 0;
    this->numberOfResultsClassifiedWell = 0;
    this->numberOfResultsMisclassefied = 0;
    this->clusteringRate = -1;
    this->numberOfInput = numberOfInputs;
    this->numberOfHiddenLayers = numberOfHiddenLayers;
    this->numberOfLayers = this->numberOfHiddenLayers+1;
    this->numberOfNeuronsInHiddenLayers = numberOfNeuronsInHiddenLayers;
    this->numberOfOutput = numberOfOutputs;
    this->numberOfSameClusteringAfterReset = 0;
    this->momentum = 0;
    error = 0;

    vector<Perceptron> temp;
    vector<float> temp2;
    neurons.push_back(temp);
    results.push_back(temp2);
    errors.push_back(temp2);
    for (unsigned int i = 0; i < this->numberOfNeuronsInHiddenLayers; i++) // layer 0
    {
        neurons[0].push_back(Perceptron(this->numberOfInput, 0, i)); // this->numberOfInputs
        results[0].push_back(0);
        errors[0].push_back(0);
    }
    for (unsigned int i = 1; i < this->numberOfHiddenLayers; i++) // layers
    {
        neurons.push_back(temp);
        results.push_back(temp2);
        errors.push_back(temp2);
        for(unsigned int j = 0;  j < this->numberOfNeuronsInHiddenLayers; j++) // output neuron
        {
            neurons[i].push_back(Perceptron(numberOfNeuronsInHiddenLayers, i, j));

            results[i].push_back(0);
            errors[i].push_back(0);
        }
    }
    neurons.push_back(temp);
    results.push_back(temp2);
    errors.push_back(temp2);
    for(unsigned int i = 0; i < this->numberOfOutput; i++)
    {
        neurons[numberOfHiddenLayers].push_back(Perceptron(this->numberOfNeuronsInHiddenLayers, numberOfHiddenLayers, i, false));
        results[numberOfHiddenLayers].push_back(0);
        errors[numberOfHiddenLayers].push_back(0);
    }
    countDesired.resize(numberOfOutputs, 0);
    averageInputs.resize(numberOfInputs, 0);
    averageDesired.resize(numberOfOutputs, 0);
    averageOutput.resize(numberOfOutputs, 0);
    binaryResult.resize(numberOfOutputs, 0);
}

vector<float> NeuralNetworkForCPU::binaryOutput(const vector<float> &inputs) // ALSO CHANGE BACKPROP // WARNING : calculateOutput(inputs) between 0 and 1 ???
{
    calculateOutput(inputs);
    for(unsigned int i = 0; i < numberOfOutput; i++)
    {
        if(results[numberOfHiddenLayers][i] > 0.5)
            binaryResult[i] = 1;
        else
            binaryResult[i] = 0;
    }
    return binaryResult;
}

vector<float> NeuralNetworkForCPU::outputFloat(const vector<float> &inputs)
{
    calculateOutput(inputs);
    return results[numberOfHiddenLayers];
}

void NeuralNetworkForCPU::calculateOutput(const vector<float> &inputs)
{
    for(unsigned int n = 0; n < neurons[0].size(); n++) // first layer
    {
       results[0][n] = neurons[0][n].outputFloat(inputs);
    }
    for(unsigned int l = 1; l < numberOfHiddenLayers; l++) // hidden layers
    {
        for(unsigned int n = 0; n < neurons[l].size(); n++)
        {
           results[l][n] = neurons[l][n].outputFloat(results[l-1]);
        }
    }
    for(int n = 0; n < neurons[numberOfHiddenLayers].size(); n++)
    {
       results[numberOfHiddenLayers][n] = neurons[numberOfHiddenLayers][n].outputFloat(results[numberOfHiddenLayers-1])/2.0f + 0.5f; // between 0 and 1
    }
}

void NeuralNetworkForCPU::resetCalculationOfClusteringRate()
{
    if(numberOfSameClusteringAfterReset > 0)
    {
        if(previousClusteringRate == clusteringRate)
        {
            counterOfSameClusteringAfterReset ++;
            if(counterOfSameClusteringAfterReset >= numberOfSameClusteringAfterReset)
            {
                resetAllNeurons();
                cout << "reset" << endl;
                counterOfSameClusteringAfterReset = 0;
            }
        }
        else
        {
            counterOfSameClusteringAfterReset = 0;
        }
    }
    numberOfResultsClassifiedWell = 0;
    numberOfResultsMisclassefied = 0;
    previousClusteringRate = clusteringRate;
    clusteringRate = 0;
}

bool NeuralNetworkForCPU::calculateClusteringRate(const vector<float> &inputs, const vector<float> &desired)
{
    vector<vector<float>> i; // take time ?
    vector<vector<float>> d;
    i.push_back(inputs);
    d.push_back(desired);
    return train(i, d, false);
}

bool NeuralNetworkForCPU::train(const vector<float> &inputs, const vector<float> &desired) // backpropagation
{
    vector<vector<float>> i;
    vector<vector<float>> d;
    i.push_back(inputs);
    d.push_back(desired);
    return !train(i, d, true);
}

bool NeuralNetworkForCPU::train(const vector<vector<float>> &inputs, const vector<vector<float>> &desired) // backpropagation
{
    if(inputs.size() != desired.size())
    {
        lastError = 8;
        return false;
    }
    return train(inputs, desired, true);
}

bool NeuralNetworkForCPU::train(const vector<vector<float>> &inputs, const vector<vector<float> > &desired, const bool isTrainnig)
{
    int numberOfMisclassified = 0;
    bool classifiedWell;
    if(isTrainnig == false)
    {
        for(int j = 0; j < inputs.size(); j++)
        {
            out = binaryOutput(inputs[j]);
            classifiedWell = true;
            for(int i = 0; i < out.size(); i++)
            {
                if(out[i] != desired[j][i] && desired[j][i] != -1.0)
                {
                    classifiedWell = false;
                    break;
                }
            }
            if(classifiedWell == true)
            {
                //numberOfClassifiedWell++;
                numberOfResultsClassifiedWell++;
            }
            else
            {
                numberOfMisclassified++;
                numberOfResultsMisclassefied++;
            }
        }
        clusteringRate = (float)(numberOfResultsClassifiedWell) / (numberOfResultsClassifiedWell + numberOfResultsMisclassefied);
    }
    else // if(isTrainnig == true)
    {
        backpropagationAlgorithm(inputs, desired); // THE MOST IMPORTANT
        //resetAllNeurons();
        geneticModification(0.004f); // 1/250


        if(lenghtOfShortRuns > 0)
        {
            shortRunCounter ++;
            if(shortRunCounter >= lenghtOfShortRuns)
            {
                shortRunCounter = 0;
                resetAllNeurons();
            }
        }
        return true;
    }
    if(numberOfMisclassified == 0)
        return true;
    else
        return false;
}

inline
void NeuralNetworkForCPU::backpropagationAlgorithm(const vector<vector<float>> &inputs, const vector<vector<float>> &desired)
{
    countDesired.assign(numberOfOutput, 0);
    averageInputs.assign(numberOfInput, 0);
    averageDesired.assign(numberOfOutput, 0);
    averageOutput.assign(numberOfOutput, 0);
    for(unsigned int i = 0; i < numberOfInput; i++) // average inputs
    {
        for(int j = 0; j < inputs.size();j++)
        {
            averageInputs[i] += inputs[j][i];
        }
        averageInputs[i] /= inputs.size();
    }
    for(unsigned int i = 0; i < numberOfOutput; i++) //average desired
    {
        for(int j = 0; j < desired.size(); j++)
        {
            if(desired[j][i] != -1.0f)
            {
                averageDesired[i] += desired[j][i];
                countDesired[i] ++;
            }
        }
        if(countDesired[i] > 0.0f)
            averageDesired[i] /= countDesired[i];
        else
           averageDesired[i] = -1.0f;
    }
    averageOutput = outputFloat(averageInputs);
    //error = 0;
    for(unsigned int i = 0; i < numberOfOutput; i++) // for each neurons in upper layer
    {
        if(averageDesired[i] != -1.0f)
        {
            errors[numberOfHiddenLayers][i] = (float)averageDesired[i] - (float)averageOutput[i];
            errors[numberOfHiddenLayers][i] = (float)errors[numberOfHiddenLayers][i] * (float)abs(errors[numberOfHiddenLayers][i]);
        }
        else
        {
            errors[numberOfHiddenLayers][i] = 0;
        }
    }
    for(int l = (int)(neurons.size()-1); l >= 0; l--) // from last hidden layer to first layer
    {
        for(unsigned int n = 0; n < neurons[l].size(); n++)
        {
            if(l < (int)numberOfHiddenLayers)
            {
                calculateError(l, n);
            }
            /*if(l == 0) // WARNING : to delete
            {
                cout << "error CPU : " << errors[l][n] << endl;
            }*/
        }
    }
    for(int l = (int)(neurons.size()-1); l >= 0; l--) // from last hidden layer to first layer
    {
        for(unsigned int n = 0; n < neurons[l].size(); n++)
        {
            if(l == 0)
                neurons[l][n].trainWithError(averageInputs, errors[l][n], learningRate, momentum);
            else
                neurons[l][n].trainWithError(results[l-1], errors[l][n], learningRate, momentum);
        }
    }
    //cout << endl; // TO REMOVE
}

inline
void NeuralNetworkForCPU::calculateError(const int neuronNumberLayer, const int neuronNumber) // TO RE READ
{
    float error = 0;
    for(int i = 0; i < neurons[neuronNumberLayer+1].size(); i++) // for each neurons in upper layer
    {
        error += errors[neuronNumberLayer+1][i] * neurons[neuronNumberLayer+1][i].getWeight(neuronNumber);
    }
    error /= neurons[neuronNumberLayer+1].size();
    errors[neuronNumberLayer][neuronNumber] = error;
}

inline
void NeuralNetworkForCPU::geneticModification(float probability)
{
    if(rand()/(float)RAND_MAX <= probability)
    {
        int a = randomBetween(0, (int)neurons.size());
        int b = randomBetween(0, (int)neurons[a].size());
        neurons[a][b] = Perceptron(neurons[a][b].getNumberOfInputs(), a, b, true);
    }

}

inline
void NeuralNetworkForCPU::resetAllNeurons()
{
    for(int i = 0; i < neurons.size(); i++)
    {
        for(int j = 0; j < neurons[i].size(); j++)
        {
            neurons[i][j] = Perceptron(neurons[i][j].getNumberOfInputs(), neurons[i][j].getLayerNumber(), neurons[i][j].getNumberInLayer(), false);
        }
    }
}

void NeuralNetworkForCPU::addANeuron(unsigned int layerNumber, bool isVirgin)
{
    results[layerNumber].push_back(0);
    errors[layerNumber].push_back(0);

    if(layerNumber == 0)
    {
        numberOfInput ++;
        averageInputs.resize(numberOfInput, 0);
        neurons[layerNumber].push_back(Perceptron(neurons[layerNumber][0].getWeights().size(), layerNumber, numberOfInput-1, isVirgin));
        for(unsigned int i = 0; i < neurons[layerNumber+1].size(); i++)
        {
            neurons[layerNumber+1][i].addAWeight(isVirgin);
        }

    }
    else if(layerNumber == numberOfHiddenLayers)
    {
        numberOfOutput ++;
        countDesired.resize(numberOfOutput, 0);
        averageDesired.resize(numberOfOutput, 0);
        averageOutput.resize(numberOfOutput, 0);
        binaryResult.resize(numberOfOutput, 0);
        neurons[layerNumber].push_back(Perceptron(neurons[layerNumber][0].getWeights().size(), layerNumber, numberOfOutput-1, isVirgin));

    }
    else if(layerNumber > 0 && layerNumber < numberOfHiddenLayers)
    {
        numberOfNeuronsInHiddenLayers ++;
        for(int j = 1;  j < numberOfHiddenLayers; j++) // output neuron
        {
            neurons[j].push_back(Perceptron(neurons[j][0].getWeights().size(), j, numberOfNeuronsInHiddenLayers-1, isVirgin));
            for(int i = 0; i < neurons[j+1].size(); i++)
            {
                neurons[j+1][i].addAWeight(isVirgin);
            }
        }
    }
    else
    {
        cout << "ERROR IN ADD NEURON" << endl;
        lastError = 8;
    }
    cout << "neuron added" << endl;
}

int NeuralNetworkForCPU::isValid()
{
    unsigned int real_sum = 0;
    unsigned int virtual_sum = 0;

    for(unsigned int i = 0; i < neurons.size(); i++)
    {
        if(((unsigned int)neurons[i].size() != numberOfNeuronsInHiddenLayers && i < numberOfHiddenLayers)
        || ((unsigned int)neurons[i].size() != numberOfOutput && i == numberOfHiddenLayers))
        {
            cout << "i = " << i << endl;
            cout << numberOfNeuronsInHiddenLayers << endl;
            cout << neurons[i].size() << endl;
            lastError = 10;
            return lastError;
        }
    }
    for(unsigned int i = 0; i < neurons.size(); i++)
    {
        for(unsigned int j = 0; j < neurons[i].size(); j++)
        {
           real_sum += neurons[i][j].getNumberOfInputs();
        }
    }

    virtual_sum += numberOfNeuronsInHiddenLayers*numberOfInput + numberOfNeuronsInHiddenLayers*numberOfOutput + (int)pow(numberOfNeuronsInHiddenLayers, 2) * (numberOfHiddenLayers-1);

    if(real_sum != virtual_sum)
    {
        lastError = 11;
        cout << real_sum << endl;
        cout << virtual_sum << endl;
        return lastError;
    }
    if(numberOfInput < 1 || numberOfInput > 2073600) // 1920 * 1080
    {
        lastError = 1;
        return lastError;
    }
    if(numberOfHiddenLayers < 1 || numberOfHiddenLayers > 100)
    {
        lastError = 2;
        cout << numberOfHiddenLayers << endl;
        return lastError;
    }
    if(neurons.size() != (unsigned)(numberOfHiddenLayers+1))
    {
       lastError = 3;
       return lastError;
    }
    if(learningRate < 0 || learningRate > 1 )
    {
        return 5;
        cout << learningRate << endl;
    }
    for(unsigned int i = 0; i < neurons.size(); i++)
    {
        for(unsigned int j = 0; j < neurons[i].size(); j++)
        {
            if(neurons[i][j].isValid() != 0)
            {
              lastError = neurons[i][j].isValid();
              return lastError;
            }
        }
    }
    return 0;
}

int NeuralNetworkForCPU::getLastError()
{
    return lastError;
}

bool NeuralNetworkForCPU::operator==(const NeuralNetworkForCPU &neuralNetwork)
{
    if(this->numberOfInput != neuralNetwork.numberOfInput
    || this->numberOfHiddenLayers != neuralNetwork.numberOfHiddenLayers
    || this->numberOfNeuronsInHiddenLayers != neuralNetwork.numberOfNeuronsInHiddenLayers
    || this->neurons.size() !=  neuralNetwork.neurons.size())
        return false;

    for(int i = 0; i < this->neurons.size(); i++)
    {
        if(this->neurons.size() !=  neuralNetwork.neurons.size())
            return false;
        for(int j = 0; j < this->neurons[i].size(); j++)
            if(this->neurons[i][j] != neuralNetwork.neurons[i][j])
                return false;
    }
    return true;
}

bool NeuralNetworkForCPU::operator!=(const NeuralNetworkForCPU &neuralNetwork)
{
    if(this->operator==(neuralNetwork))
        return false;
    else
        return true;
}

string NeuralNetworkForCPU::display()
{
    string str;
   str = "Learning rate : " + to_string(learningRate);
    for(int i = 0; i < neurons.size(); i++)
    {
        str += "Layer " + to_string(i) + " :\n";
        for(int j = 0; j < neurons[i].size(); j++)
        {
            str += neurons[i][j].display();
            str += "\n";
        }
    }
    return str;
}

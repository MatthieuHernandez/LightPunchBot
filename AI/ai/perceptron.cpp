/*#include "perceptron.h"

Perceptron::Perceptron(int number_input)
{
    rand();
    this->number_input = number_input;
    this->learning_rate = 0.01f;
    weights = new float[number_input];
    for (int i = 0; i < number_input; i++)
    { 
        do{
            weights[i] = ( rand()/(float)RAND_MAX ) * 2 - 1; // between -1 and 1
        }while(weights[i] <= -1 || weights[i] >= 1);
    }
    do{
        bias = ( rand()/(float)RAND_MAX ) * 2 - 1; // between -1 and 1
    }while(bias <= -1 || bias >= 1);
}

int Perceptron::output(float* inputs)
{
    float sum = 0;
    for (int i = 0; i < number_input; i++)
    {
        sum += inputs[i] * weights[i];
    }
    sum += bias;
    if(sum > 0)
        return 1;
    else
        return 0;
}

void Perceptron::train(float* inputs, int desired)
{ 
    int guess = output(inputs);
    float error = desired - guess;
    for (int i = 0; i < number_input; i++)
    {
        if(weights[i] + learning_rate * error * inputs[i] >= -1
          && weights[i] + learning_rate * error * inputs[i] <= 1)
        {
            weights[i] += learning_rate * error * inputs[i];
        }
    }
    float r = (rand()/RAND_MAX)* 2 - 1;
    if( r > 0 && bias < 1 - learning_rate)
        bias += learning_rate;

    if( r < 0 && bias > - 1 + learning_rate)
        bias -= learning_rate;
}

bool Perceptron::is_valid()
{
    if(bias <= -1 || bias >= 1 || bias == 0)
    {
        return false;
    }
    if(learning_rate <= 0 || learning_rate >= 1 )
    {
        return false;
    }
    if(number_input < 1)
    {
        return false;
    }
    for(int i = 0; i < number_input; i++)
    {
        if(weights[i] <= -1 || weights[i] >= 1 || weights[i] == 0)
        {
            return false;
        }
    }

    return true;
}

float* Perceptron::get_weights()
{
    return weights;
}

void Perceptron::set_weights(float* weights)
{;
    this->weights = new float[number_input];
    for(int i = 0; i < number_input; i++)
        this->weights[i] = weights[i];

}

float Perceptron::get_weight(int i)
{
    return weights[i];
}

void Perceptron::set_weight(int i, float weight)
{
    this->weights[i] = weight;
}

float Perceptron::get_learning_rate()
{
    return learning_rate;
}

void Perceptron::set_learning_rate(float learning_rate)
{
    this->learning_rate = learning_rate;
}

float Perceptron::get_bias()
{
    return bias;
}

void Perceptron::set_bias(float bias)
{
    this->bias = bias;
}

int Perceptron::get_number_input()
{
    return number_input;
}

void Perceptron::set_number_input(int number_input)
{
    this->number_input = number_input;
}
*/

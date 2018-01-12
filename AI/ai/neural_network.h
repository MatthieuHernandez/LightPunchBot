#ifndef NEURAL_NETWORK2_H
#define NEURAL_NETWORK2_H

#include "neuralNetwork.h"
#include "parameters.h"
#include <sstream>


class Neural_network : public NeuralNetwork
{
    private:

        Neural_network(int id);

    public:

        int numberOfPositiveResultsClassifiedWell = 0;
        int numberOfNegativeResultsClassifiedWell = 0;
        int numberOfPositiveResultsMisclassified = 0;
        int numberOfNegativeResultsMisclassified = 0;
        float score;
        int batch_size;

        string name = ""; // combo name
        int id = -1; // not unique
        float clustering_rate = -1;
        float multi_clustering_rate[NUMBER_ACTION];

        Neural_network();
        Neural_network(int number_input, int number_hidden_layer, int number_neuron_hidden_layer, int number_output, int id = -1);

        void save();
        void remove();
        static Neural_network load(const int &id);

        bool operator==(const Neural_network &neural_network);
        bool operator!=(const Neural_network &neural_network);

        void reset_classification()
        {
            numberOfPositiveResultsClassifiedWell = 0;
            numberOfNegativeResultsClassifiedWell = 0;
            numberOfPositiveResultsMisclassified = 0;
            numberOfNegativeResultsMisclassified = 0;
        }
};

#endif // NEURAL_NETWORK2_H

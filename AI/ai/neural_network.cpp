#include "neural_network.h"
#include "gui/thread.h"

Neural_network::Neural_network()
{

}

Neural_network::Neural_network(int id)
    : NeuralNetwork()
{
    this->id = id;
    this->batch_size = 1;
    this->score = 0;
    for(int i = 0; i < NUMBER_ACTION; i++)
        multi_clustering_rate[i] = -1;

}

Neural_network::Neural_network(int number_input, int number_hidden_layer, int number_neuron_hidden_layer, int number_output, int id)
    : NeuralNetwork(number_input, number_hidden_layer, number_neuron_hidden_layer, number_output)
{
    this->batch_size = 1;
    if(id == -1)
    {
        vector<string> request = Database::read("SELECT MAX(id) FROM neural_networks");
        if(request[0] != "NULL")
            this->id = stoi(request[0])+1;
        else
            this->id = 0;
    }
    else
        this->id = id;
}

void Neural_network::save()
{
    if(this->isValid() != 0)
    {
        Thread::instance()->write("error " + to_string(this->getLastError()) + " in save", "nn_information");
    }
    else
    {
        stringstream str_multi_clustering_rate;
        for(int i = 0; i < NUMBER_ACTION; i++)
        {
            str_multi_clustering_rate << to_string(this->multi_clustering_rate[i]);
            if(i < NUMBER_ACTION - 1)
            {
                    str_multi_clustering_rate << " ";
            }
        }

        if(!Database::write("INSERT OR REPLACE INTO neural_networks (id, clustering_rate, number_input, number_hidden_layer, number_neuron_layer, "
                            "positive_classifed_well, positive_misclassified, negative_classified_well, negative_misclassified, number_output, batch_size, score, other_information) "
                            "VALUES (" + to_string(id) + ", " + to_string(clustering_rate) + ", " + to_string(getNumberOfInputs())
                            + ", " + to_string(getNumberOfHiddenLayers()) + ", " + to_string(getNumberOfNeuronsInHiddenLayers())
                            + ", " + to_string(numberOfPositiveResultsClassifiedWell) + ", " + to_string(numberOfPositiveResultsMisclassified)
                            + ", " + to_string(numberOfNegativeResultsClassifiedWell) + ", " + to_string(numberOfNegativeResultsMisclassified)
                            + ", " + to_string(getNumberOfOutputs()) + ", " + to_string(batch_size) + ", " + to_string(score)
                            + ", '" + str_multi_clustering_rate.str() + "')"))
        {
            cout << "ERROR IN SAVE 2" << endl;
            return;

        }

        stringstream str_learning_rate;
        str_learning_rate << this->getLearningRate();

        for(int i = 0; i < this->neurons.size(); i++)
        {
            for(int j = 0; j < this->neurons[i].size(); j++)
            {
                stringstream str_perceptron;
                for(int k = 0; k < this->neurons[i][j].getNumberOfInputs(); k++)
                {
                    str_perceptron << this->neurons[i][j].getWeight(k);
                    if(k < this->neurons[i][j].getNumberOfInputs() - 1)
                    {
                            str_perceptron << " ";
                    }
                }

                stringstream str_bias;
                str_bias << this->neurons[i][j].getBias();

                Database::write("INSERT OR REPLACE INTO perceptrons (id, id_layer, id_nn, weights, bias, number_input, learning_rate) VALUES ("
                                + to_string(j) + ", "
                                + to_string(i) + ", "
                                + to_string(this->id) + ", "
                                + str_perceptron.str() + "', "
                                + str_bias.str() + ", "
                                + to_string(this->neurons[i][j].getNumberOfInputs()) + ", "
                                + str_learning_rate.str() + ")");
            }
        }
    }
}

void Neural_network::remove()
{
    Database::write("DELETE FROM neural_networks WHERE id = " + to_string(id));
    Database::write("DELETE FROM perceptrons WHERE id_nn = " + to_string(id));
}

Neural_network Neural_network::load(const int &id)
{
    vector<string> request = Database::read("SELECT number_input, number_hidden_layer, number_neuron_layer, number_output, id, batch_size, score, other_information FROM neural_networks "
                                "WHERE id = " + to_string(id));

    Neural_network result(stoi(request[0]), stoi(request[1]), stoi(request[2]),
                          stoi(request[3]), stoi(request[4]));

    result.batch_size = stoi(request[5]);
    result.score = stof(request[6]);

    string name = request[7];
    int j;
    for(j = 0; name != name.substr(name.find(' ')+1); j++)
    {
        result.multi_clustering_rate[j] = (stof(name.substr(0, name.find(' '))));
        name = name.substr(name.find(' ')+1);
    }
    result.multi_clustering_rate[j] = stof(name.substr(0, name.find(' ')));

    request = Database::read("SELECT id, id_layer, weights, bias, number_input, learning_rate FROM perceptrons "
                                "WHERE id_nn = " + to_string(result.id)); //ORDER BY id_layer ASC, id ASC

    if(request.size()/6 != result.getNumberOfOutputs() + result.getNumberOfHiddenLayers() * result.getNumberOfNeuronsInHiddenLayers())
    {
        Thread::instance()->write("error 100 during loading : " + to_string(request.size()/6), "nn_information");
    }
    for(int i = 0; i < request.size(); i += 6) // for each perceptron
    {
        int id = stoi(request[i]);
        int layer = stoi(request[i+1]);
        result.neurons[layer][id].setBias(stof(request[i+3]));
        //result.neurons[layer][id].setLearningRate(stof(request[i+5])); // WARNING : Why is it remove ?

        vector<float> weights;
        string name = request[i+2];

        for(int j = 0; name != name.substr(name.find(' ')+1); j++)
        {
            weights.push_back((stof(name.substr(0, name.find(' ')))));
            name = name.substr(name.find(' ')+1);
        }
        weights.push_back(stof(name.substr(0, name.find(' '))));

        result.neurons[layer][id].setWeights(weights);

        if(result.neurons[layer][id].getWeights().size() != stoi(request[i+4]))
        {
            Thread::instance()->write("error 101 during loading : ", "nn_information");
        }

    }

    request = Database::read("SELECT clustering_rate FROM neural_networks WHERE id = " + to_string(id));
    result.clustering_rate = stof(request[0]);

    return result;
}

bool Neural_network::operator==(const Neural_network &neural_network)
{

    if(this->NeuralNetwork::operator==(neural_network)
    && this->id == neural_network.id
    && this->name == neural_network.name
    && this->clustering_rate == neural_network.clustering_rate)
        return true;
    else
        return false;
}

bool Neural_network::operator!=(const Neural_network &neural_network)
{
    if(this->operator==(neural_network))
        return false;
    else
        return true;
}

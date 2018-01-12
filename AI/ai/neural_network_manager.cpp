#define RESET_NEURAL_NETWORKS false // false by default
#define RESETT 50000
#define KKK 1
#include "neural_network_manager.h"

using namespace Database_manager;

vector<Neural_network> Neural_network_manager::neural_networks;
vector<Neural_network> Neural_network_manager::neural_networks_temp;
QVector<double> Neural_network_manager::learning_rate;

Neural_network_manager *m_instance = 0;

Neural_network_manager* Neural_network_manager::instance()
{
    if(m_instance == NULL)
        m_instance = new Neural_network_manager();
    return m_instance;
}

Neural_network_manager::Neural_network_manager()
{

}

Neural_network_manager::~Neural_network_manager()
{

}

void Neural_network_manager::add()
{
    neural_networks.push_back(Neural_network(Player::number_inputs, 1, 100, Algorithm::number_action));
    neural_networks_temp = neural_networks;
    calcul_clustering_rate(neural_networks.back().id);
    Thread::instance()->write("add a new neural network", "nn_information");
}

void Neural_network_manager::remove(int id)
{
    for(int i = 0; i < neural_networks.size(); i++)
    {
        if(neural_networks[i].id == id)
        {
            neural_networks[i].remove();
            Thread::instance()->write("neral network " + to_string(neural_networks[i].id) + " delete", "nn_information");
            neural_networks.erase(neural_networks.begin()+i);
            break;
        }
    }
}

void Neural_network_manager::train(int id)
{
    learning_rate.clear();
    if(Parameters::shuffle_database == true)
        shuffle_states();
    unsigned int nb = 0;
    unsigned int shuffle_counter = 0;
    int i;
    for(i = 0; i < neural_networks.size(); i++)
    {
        neural_networks[i].setLenghtOfShortRuns(0);// INSIDE REQUEST
    }
    neural_networks_temp = neural_networks;
    do{
        for(i = 0; i < neural_networks_temp.size(); i++)
        {
            if(neural_networks_temp[id].id == id || id == -1)
            {
                if(neural_networks_temp[id] != neural_networks[i]
                || neural_networks_temp[id].isValid() != 0)
                {
                    Thread::instance()->write("ERROR FOUND : " + to_string(neural_networks_temp[id].getLastError()), "nn_information");
                    return;
                }
                // FOR OPTIMIZE
                /*Database::instance(1)->read((string)"SELECT score, deals_damage, states.id_combo, life, life_2, position_x, position_y, position_x_2, position_y_2, distance, orientation, state, state_2, "
                                                    "time_state, time_state_2, super, super_2, stun, stun_2, v_trigger, v_trigger_2, duration, duration_2 FROM states, combos"
                                                    " WHERE " + "states.id_combo = combos.id AND states.learning_set = 'FALSE' ORDER BY random");
                request_for_test = Database::instance(1)->result_request;
                cout << request_for_test[1] << endl;*/
                do
                {
                    //neural_networks_temp[id].batch_size = 1;//randomBetween(5, 10);// ////////////////////////////////////////////////////////////////////////
                    //neural_networks_temp[id].setLearningRate(0.002 * randomBetween(1, 199));


                    /* multithreading
                    id_combo_ccr = id_combo;
                    id_ccr = id;
                    is_training_ccr = true;
                    offset_crr = shuffle_counter;


                    for(int j = 0; j < Parameters::number_of_thread; j++)
                    {
                        QTimer *t = new QTimer(this);
                        t->singleShot(0, this, SLOT(calcul_clustering_rate()));
                        timers.push_back(t);
                        Thread::instance()->sleep(1);
                    }
                    bool is_end;
                    do
                    {
                        is_end = true;
                        for(int j = 0; j < timers.size(); j++)
                        {
                            if(timers[j]->isActive() == true)
                            {
                                is_end = false;
                            }

                        }
                    } while(is_end == false);

                    nb += timers.size();
                    timers.clear();

                    if(bool_ccr == false)
                    {*/

                    if(calcul_clustering_rate(id, true, shuffle_counter) == false)
                    {
                        shuffle_states();
                        shuffle_counter = 0;
                    }
                    else
                        shuffle_counter += neural_networks_temp[id].batch_size;

                    if(nb%RESETT == 0)
                    {
                        neural_networks_temp[id] = neural_networks[i];
                    }
                    learning_rate.push_back((double)neural_networks_temp[id].clustering_rate);
                    Thread::instance()->replot();
                    nb++;
                    if(Parameters::is_score == false)
                        Thread::instance()->write("Neural network train " + to_string(nb) + " times : " +  to_string(neural_networks_temp[id].clustering_rate), "current_nn_information");
                    else
                    Thread::instance()->write("Neural network train " + to_string(nb) + " times : " +  to_string(neural_networks_temp[id].score), "current_nn_information");
                }while(Parameters::choice_function == TRAIN_NN && train_all == false);
                //neural_networks_temp[id].save(); // WARNING : only for test
                break;
            }
        }
        /*if(train_all == true)
        {
            Thread::instance()->write("Neural network train " + to_string(nb) + " times.", "current_nn_information");
            nb++;
            if(nb%NUMBER_SHUFFLE_TRAIN_ALL == 0 && Parameters::shuffle_database == true)
            {
                shuffle_states();
            }
            if(shuffle_counter < number_of_states - neural_networks_temp[id].batch_size)
                shuffle_counter += neural_networks_temp[id].batch_size;
            else
            {
                shuffle_counter = 0;
                shuffle_states();
            }
            // WITH BATCH_SIZE MAX
        }*/
    }while(Parameters::choice_function == TRAIN_ALL_NN /*&& train_all == true*/);

    if(train_all == false)
       Thread::instance()->write("neral network " + neural_networks_temp[id].name + " " + to_string(neural_networks_temp[id].id) + " was train " + to_string(nb) + " time.", "nn_information");
    else
        Thread::instance()->write("All neral network were train " + to_string(nb) + " time.", "nn_information");
}

void Neural_network_manager::train_all()
{
    Neural_network_manager::train(-1);
}

void Neural_network_manager::calcul_clustering_rate()
{
    bool_ccr = calcul_clustering_rate(id_combo_ccr, id_ccr, is_training_ccr, offset_crr);
}

bool Neural_network_manager::calcul_clustering_rate(int id, bool is_training, const unsigned int &offset) // return false if it needs to shuffle database
{
    vector<vector<float>> multiple_desired_outputs;
    vector<vector<float>> multiple_inputs;
    //unsigned int i;

    for(int kk = 0; kk < KKK; kk++)////////////////////////////////////////////
    {
        if(is_training == true)
        {
            neural_networks_temp[id].resetCalculationOfClusteringRate(); // IMPORTANT

            for(int j = 0; j < states_for_learning.size(); j++)
            {
                multiple_desired_outputs.push_back(states_for_learning[j].desired_outputs);
                multiple_inputs.push_back(Player::get_inputs(result, j+3));
            }
            if(multiple_desired_outputs.size() == neural_networks_temp[id].batch_size)
            {
                neural_networks_temp[id].train(multiple_inputs, multiple_desired_outputs); // IMPORTANT
                multiple_desired_outputs.clear();
                multiple_inputs.clear();
            }
            else
            {
                cout << "data shuffled" << endl;
                return false;
            }
        }
        else // is_trainning == false
        {
            neural_networks_temp[id] == neural_networks[i];
        }
    }
    neural_networks_temp[id].resetCalculationOfClusteringRate(); // IMPORTANT
    neural_networks_temp[id].reset_classification();

    float array_of_positive[NUMBER_ACTION] = {0};           // Algorithm::number_action
    float array_of_negative[NUMBER_ACTION] = {0};           // Algorithm::number_action
    float array_of_sum[NUMBER_ACTION] = {0};                // Algorithm::number_action
    float array_of_sum_of_negative[NUMBER_ACTION] = {0};    // Algorithm::number_action
    float array_of_sum_of_positive[NUMBER_ACTION] = {0};    // Algorithm::number_action
    double score = 0;
    float x = 0;
    for(int j = 0; j < states_for_testing.size(); j ++)
    {
        array_of_sum[states_for_testing[j].id_combo-1] ++;

        if(neural_networks_temp[id].calculateClusteringRate(states_for_testing[j].inputs,  states_for_testing[j].desired_outputs) == true) // IMPORTANT
        {
            if((states_for_testing[j].score > 0)
            || (states_for_testing[j].score >= 0 && states_for_testing[j].deal_damage == false))
            {
                array_of_positive[states_for_testing[j].id_combo-1]++;
                neural_networks_.numberOfPositiveResultsClassifiedWell++;
            }
            else
            {
                array_of_negative[states_for_testing[j].id_combo-1]++;
                neural_networks_.numberOfNegativeResultsClassifiedWell++;
            }
        }
        else
        {
            if((states_for_testing[j].score > 0)
            || (states_for_testing[j].score >= 0 && states_for_testing[j].deal_damage == false))
            {
                neural_networks_.numberOfPositiveResultsMisclassified++;
            }
            else
            {
                neural_networks_temp[id].numberOfNegativeResultsMisclassified++;
            }
        }
        array_of_sum[states_for_testing[j].id_combo-1]++;
    }
    for(int j = 0; j < Algorithm::number_action; j++) // computation of the score
    {
        x = pow(
                    (sqrt(array_of_positive[j]) * pow(array_of_negative[j], 2) * 1/(1+sqrt(array_of_sum_of_positive[j]) * pow(array_of_sum_of_negative[j], 2)))
                    , 2); // WRONG : max score = 22*(0.5*0.5) = 5.5

        //cout << array_of_positive[j] << "/" << array_of_sum_of_positive[j] << " " << array_of_negative[j] << "/" << array_of_sum_of_negative[j] << " " << x << endl;

        score += x;
    }
    for(int j = 0; j < Algorithm::number_action; j++) // ((1+exp(-20*(1-0.2)))/(1+exp(-20*(x-0.2)))*x^0.0001)^15
    {
        neural_networks_temp[id].multi_clustering_rate[j] = (array_of_positive[j] + array_of_negative[j])/(1 + array_of_sum_of_positive[j] + array_of_sum_of_negative[j]);

        if(array_of_negative[j]/array_of_sum_of_negative[j] < 0.55)
        {

            x = (0.45+(array_of_negative[j]/array_of_sum_of_negative[j])) *0.5 + 0.5; // for reduce the effect of reduction
            score *= x;
            if(x > 1 || x < 0)
                cout << x << endl;
        }
    }
    //mutex.lock(); // VERY IMPORTANT

    neural_networks_temp[id].clustering_rate = neural_networks_temp[id].getClusteringRate() * 100;
    neural_networks_temp[id].score = score;
    if(is_training == true)
    {
        // WARNING : do warning to score
        if((neural_networks_temp[id].score > neural_networks[id].score && neural_networks_temp[id].id != 2)
        || (neural_networks_temp[id].clustering_rate > neural_networks[id].clustering_rate && neural_networks_temp[id].id == 2)) //score neural_networks_temp[id].clustering_rate > neural_networks[i].clustering_rate
        {
            neural_networks[id] = neural_networks_temp[id];
            neural_networks[id].save();
            Thread::instance()->write_nn(to_string(neural_networks[id].clustering_rate) + "%", neural_networks[id].id);
            if(Parameters::display_update == true)
            {
                string text = "Neural network " + to_string(neural_networks[id].id) + " has been updated.";
                Thread::instance()->write(text.c_str(), "nn_information");
            }
        }
    }
    else
    {
        neural_networks[id] = neural_networks_temp[id];
        neural_networks[id].save();
    }

    //mutex.unlock(); // VERY IMPORTANT

    return true;
}

void Neural_network_manager::save()
{
    for(int i = 0; i < neural_networks.size(); i++)
    {
        neural_networks[i].save();
    }
    if(neural_networks.size() < 1)
        Thread::instance()->write("All neural networks are saved", "nn_information");
}

void Neural_network_manager::load()
{
    Player::instance(1);
    Player::instance(2);
    bool test = false;
    bool test2 = database_has_been_modify();
    for(int i = 0; i < ids.size(); i ++)
    {
        neural_networks.push_back(Neural_network::load(ids[i]));
        neural_networks_temp.push_back(neural_networks.back());
        if(neural_networks.back().isValid() != 0 || neural_networks.back().clustering_rate < 30)
        {
            test = true;
            Thread::instance()->write("error " + to_string(neural_networks.back().isValid()) + " in neural network " + to_string(neural_networks.back().id), "nn_information");
        }
        else
        {
            if(test2 == true || RESET_NEURAL_NETWORKS)
                calcul_clustering_rate(neural_networks.back().id);
        }
    }
    neural_networks_temp = neural_networks;
    if(test == true)
        Thread::instance()->write("Loading error !", "nn_information");
    else
        Thread::instance()->write("Neural networks load.", "nn_information");
}

void Neural_network_manager::test()
{
    int count = 0;
    int i = 0;
    for(i = 0; i < neural_networks.size(); i++)
    {
        while(neural_networks[i].getNumberOfOutputs() != NUMBER_ACTION)
        {
            if(neural_networks[i].getNumberOfOutputs() < NUMBER_ACTION)
            {
                neural_networks[i].addANeuron(neural_networks[i].getNumberOfHiddenLayers()+1);
                Thread::instance()->write("Neuron added.","nn_information");
            }
            else if(neural_networks[i].getNumberOfOutputs() > NUMBER_ACTION)
            {
                count ++;
                break;
            }
        }
    }
    for(i = 0; i < neural_networks.size(); i++)
    {
        neural_networks_temp[id] = neural_networks[i];
        calcul_clustering_rate(neural_networks[i].id, false);
        if(neural_networks[i].isValid() != 0)
        {
            count ++;
        }
    }
    Thread::instance()->write("There are " + to_string(i) + " neural networks :","nn_information");
    if(count == 0)
    {
        Thread::instance()->write("All neural networks are valid","nn_information");
    }
    else
    {
        Thread::instance()->write(to_string(count) + " neural networks are invalid !","nn_information");
    }

}

void Neural_network_manager::clean()
{
    Thread::instance()->write("CLEAR","nn_information");
    Database::write("DELETE FROM perceptrons WHERE NOT EXISTS (SELECT 1 FROM neural_networks WHERE perceptrons.id_nn = neural_networks.id)");
    balance_states();
    calcul_average_damage();
    if(database_has_been_modify() == true)
    {
        for(int i = 0; i < neural_networks.size();i++)
        {
            calcul_clustering_rate(neural_networks[i].id, false);
        }
    }
    Thread::instance()->write("CLEAR","nn_information");
    Thread::instance()->write("Neural networks has been cleaned","current_nn_information");
}

vector<Neural_network> Neural_network_manager::select_and_sort() // WARNING TO REWORK WITH SCORE FOR SELECT THE BEST
{
    vector<Neural_network> result = neural_networks;
    /*for(int i = 0; i < result.size(); i++)
    {

        for(int j = i; j < result.size(); j++)
        {
            if(result[i].id_combo == result[j].id_combo)
            {
                if(result[i].clustering_rate >= result[j].clustering_rate)
                    result.erase(result.begin()+j);
                else
                    result.erase(result.begin()+i);
            }
        }
    }
    Neural_network temp;
    bool is_sorted = false;
    while(!is_sorted)
    {
        is_sorted = true;
        for(int i = 0; i < result.size()-1; i++)
        {
            if(result[i].clustering_rate < result[i+1].clustering_rate)
            {
                is_sorted = false;
                temp = result[i];
                result[i] = result[i+1];
                result[i+1] = temp;
            }
        }
    }*/
    return result;
}

unsigned int Neural_network_manager::get_number_of_neural_network()
{
    return neural_networks.size();
}

const Neural_network* const Neural_network_manager::get_neural_network(int id)
{
    return const_cast<Neural_network*>(NeuralNetwork*)&neural_networks[id];
}


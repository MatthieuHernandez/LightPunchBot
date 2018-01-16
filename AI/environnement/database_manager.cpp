#include "database_manager.h"
#include <environnement/database.h>

void Database_manager::initialize()
{
    Database::initialize(DATABASE);

    array_for_normalisation = new float[Player::number_inputs];
    array_for_normalisation[0] = Player::instance(1)->position_x.value_for_normalization;
    array_for_normalisation[1] = Player::instance(1)->position_y.value_for_normalization;
    array_for_normalisation[2] = Player::instance(2)->position_x.value_for_normalization;
    array_for_normalisation[3] = Player::instance(2)->position_y.value_for_normalization;
    array_for_normalisation[4] = Player::instance(1)->distance.value_for_normalization;
    array_for_normalisation[5] = Player::instance(1)->state.value_for_normalization;
    array_for_normalisation[6] = Player::instance(2)->state.value_for_normalization;
    array_for_normalisation[7] = Player::instance(1)->time_state.value_for_normalization;
    array_for_normalisation[8] = Player::instance(2)->time_state.value_for_normalization;

    initialize_states(states_for_learning, "TRUE");
    initialize_states(states_for_testing, "FALSE");
    initialize_basic_information();
    initialize_vector_of_ids();
}

void Database_manager::initialize_states(vector<state> v, string text)
{
    vector<string> request = Database::read("SELECT score, deals_damage, states.id_combo, position_x, position_y, position_x_2, position_y_2, distance, state, state_2, "
                                        "time_state, time_state_2, is_useful FROM states, combos"
                                        " WHERE states.id_combo = combos.id AND states.learning_set = '" + text + "' ORDER BY random");

    v.resize(request.size()/(Player::number_inputs+4));
    for(int i = 0, k = 0; i < v.size(); i++)
    {
        if(request[k] == "TRUE")
            v[i].deal_damage = true;
        else if(request[k] == "FALSE")
            v[i].deal_damage = false;
        else
            cout << "ERROR : line 37 - database_manager.h" << endl;
        k++;
        id_combo = stoi(request[k]);
        k++;
        v[i].inputs.resize(Player::number_inputs);
        for(int j = 0; j < Player::number_inputs; j++, k++)
        {
            v[i].inputs[j] = stof(request[k]);
        }
        if(request[k] == "1")
            v[i].is_useful = true;
        else if(request[k] == "0")
            v[i].is_useful = false;
        else
            cout << "ERROR : line 51 - database_manager.h" << endl;
        k++;

        v[i].desired_outputs.resize(Player::number_outputs, -1);

        if((v[i].score > 0)
        && (v[i].score >= 0 && v[i].is_useful = false))
            v[i].desired_outputs[v[i].id_combo] = 1;
        else
            v[i].desired_outputs[v[i].id_combo] = 0;

    }
}

void Database_manager::initialize_basic_information()
{
    vector<string> request = Database::read("SELECT combo FROM combos");
    for(unsigned int i = 0; i < request.size(); i++)
    {
        combos.push_back(request[i]);
    }
}

void Database_manager::initialize_vector_of_ids()
{
    ids.clear();
    vector<string> request = Database::read("SELECT id FROM neural_networks ORDER BY id");
    for(int i = 0; i < request.size(); i++)
    {
        ids.push_back(stoi(request[i]));
    }
}

void Database_manager::shuffle_states()
{
    Database::write("UPDATE states SET random = RANDOM()");
}

void Database_manager::balance_states()
{
    vector<string> request = Database::read("SELECT id FROM states");
    for(unsigned int i = 0; i <= request.size()-1; i += 1)
    {
        if(!Database::write("UPDATE states SET count = "
                            "(SELECT res FROM states sss, "
                            "(SELECT id_combo ic, state  st, learning_set ls, SUM( "
                            "CASE "
                            "WHEN score > 0 AND is_useful = 1 THEN 1 "
                            "WHEN score < 0 THEN -1 "
                            "WHEN score >= 0 AND (SELECT deals_damage FROM combos WHERE id = sts.id_combo) = 'FALSE' THEN 1 "
                            "WHEN score = 0 AND (SELECT deals_damage FROM combos WHERE id = sts.id_combo) = 'TRUE' THEN -1 "
                            "WHEN is_useful = 0 THEN -1 "
                            "ELSE 0.1 END) res "
                            "FROM states sts GROUP BY ic, st, ls)"
                            "WHERE sss.id = states.id AND sss.id_combo = ic AND sss.state = st AND sss.learning_set = ls "
                            "ORDER BY res, date_added)"
                            "WHERE id = " + request[i] + ""
                            ";"
                            "DELETE FROM states WHERE id = " + request[i] + " AND "
                            "1 = CASE "
                            "WHEN score > 0 AND is_useful = 1 AND count > 2 THEN 1 "
                            "WHEN score < 0 AND count < -2  THEN 1 "
                            "WHEN Score >= 0 AND (SELECT deals_damage FROM combos WHERE id = states.id_combo) = 'FALSE' AND count > 2 THEN 1 "
                            "WHEN Score = 0 AND (SELECT deals_damage FROM combos WHERE id = states.id_combo)= 'TRUE' AND count < -2 THEN 1 "
                            "WHEN is_useful = 0 AND count < -2 THEN 1 "
                            "ELSE 0 END"))
            return;
        if(i%10 == 0)
        {
            Thread::instance()->sleep(1);
            Thread::instance()->write(to_string(i), "current_nn_information");
        }
    }
    Thread::instance()->sleep(200);
}

void Database_manager::calcul_average_damage()
{
        Database::write("UPDATE combos SET average_damage = (SELECT AVG(score) FROM states WHERE score > 0 AND states.id_combo = combos.id AND is_useful = 1)");
        Database::write("UPDATE combos SET average_damage = average_damage / (SELECT MAX(average_damage) FROM combos)");
}

bool Database_manager::database_has_been_modify()
{
    vector<string> request = Database::read("SELECT COUNT(*) FROM states");
    int value = stoi(request[0]);

    QSettings settings(SAVE_FILE, QSettings::IniFormat);
    if(value != settings.value("Database/number_of_states").toInt())
    {
        settings.setValue("Database/number_of_states", value);
        Thread::instance()->write("Database has been modify.", "nn_information");
        return true;
    }
    else
        return false;
    return true;
}

void Database_manager::clean_neural_networks()
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

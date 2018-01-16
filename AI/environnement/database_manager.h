#ifndef DATABASE_MANAGER_H
#define DATABASE_MANAGER_H
#include <parameters.h>
#include <gui/thread.h>
#include <environnement/player.h>

struct state
{
    int id_combo;
    int score;
    bool deal_damage;
    bool is_useful;
    vector<float> inputs;
    vector<float> desired_outputs;

};

class Database_manager
{
    public:

        static vector<string> combos;
        static vector<int> ids;

        static void initialize();
        static void initialize_basic_information();
        static void initialize_vector_of_ids();

        static void shuffle_states();
        static void balance_states();
        static void calcul_average_damage();
        static bool database_has_been_modify();
        static void clean_neural_networks();


    private :

        static float *array_for_normalisation;
        static int index_of_states;

        static vector<state> states_for_learning;
        static vector<state> states_for_testing;

        static void initialize_states(vector<state> v, string text);
};




#endif // DATABASE_MANAGER_H

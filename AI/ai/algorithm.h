#ifndef AI_H
#define AI_H

#include <gui/thread.h>
#include <environnement/player.h>
#include <environnement/database.h>
#include "environnement/read_memory.h"
#include "environnement/controler.h"
#include "parameters.h"
#include "neural_network.h"
#include "neural_network_manager.h"
#include "ongoing_action.h"
#include "vector"

class Algorithm
{
    private:

        static string actions[NUMBER_ACTION+1];
        static string actions_can_deal_damage[NUMBER_ACTION+1];
        static float actions_average_damage[NUMBER_ACTION+1];
        static int previous_life[2];
        static int id_single_nn;
        static int most_use_action;

        static int score;
        static bool has_error;
        static bool has_sort;
        static bool end_of_the_actions();
        static bool register_action(const string &action);
        static bool execute_action(const string &action);
        static bool initialization();
        static int least_present_action();

        static vector<Ongoing_action> ongoing_actions;


    public:

        Algorithm();
        const static int number_action;
        static string choose_action(bool is_learning);
        static void mode(bool is_learning = true);
        static void fighting_mode();
        static void sort_actions();

};

#endif // AI_H

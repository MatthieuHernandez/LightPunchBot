#ifndef PLAYER_H
#define PLAYER_H
#include <parameters.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include "environnement/read_memory.h"

using namespace std;

template <typename T>
struct datum
{
    int64_t address;
    T value;
    const float value_for_normalization;
    string name;
};

class Player //: public QThread
{
    //Q_OBJECT
    private:
        static vector<float> inputs;

    public:

        Player(Player const& p);

        const static int number_int = 8;
        const static int number_float = 4;

        const static int number_inputs = 9;
        const static int number_outputs = NUMBER_ACTION;

        static Player* instance(int number_player);
        static Player* instance1();
        static Player* instance2();

        datum<int> state = {0, 0, 1024, "state"};// normalisation
        datum<int> life = {0, 0, 500, "life"};
        datum<int> stun = {0, 0, 1000, "stun"};
        datum<int> v_trigger = {0, 0, 900, "v_trigger"};
        datum<int> duration = {0, 0, 800, "duration"};// TO CHANGE
        datum<int> super = {0, 0, 900, "super"};
        datum<int> time_state = {0, 0, 128, "time_state"};
        datum<int> attack_counter = {0, 0, 1, "attack_counter"};

        int previous_value;
        int is_useful;

        datum<float> position_x = {0, 0, 7.5, "position_x"};
        datum<float> position_y = {0, 0, 5, "position_y"};
        datum<float> distance = {0, 0, 3.5, "distance"};
        datum<float> orientation = {0, 0, 90.0, "orientation"};

        // WARNING : don't forget to modify the copy constructor
        datum<float>* data_float[number_float] = {&position_x, &position_y, &distance, &orientation};
        datum<int>* data_int[number_int] = {&life, &state, &stun, &v_trigger, &duration, &super, &time_state, &attack_counter};

        Player(int player_number);
        static void refresh_values();

        static vector<float> get_inputs();
        static vector<float> get_inputs(const vector<string> &result, const int &index = 0);
};

#endif // PLAYER_H

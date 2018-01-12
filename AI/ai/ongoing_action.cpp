#include "ongoing_action.h"


Ongoing_action::Ongoing_action(const string &action, const Player &player1, const Player &player2)
{
    this->player[0] = player1;
    this->player[1] = player2;
    this->action = action;
}

Ongoing_action::~Ongoing_action()
{
    //this->save();
}

bool Ongoing_action::save()
{
    int score = score_calculation();
    string request = "INSERT INTO states (";
    for(int h = 0; h <= 1; h++)
    {
        for(int i = 0; i < Player::number_float; i++)
        {
            request += this->player[h].data_float[i]->name + ", ";
        }
        for(int i = 0; i < Player::number_int; i++)
        {
            if(this->player[h].data_int[i]->name != "attack_counter_2"
            && this->player[h].data_int[i]->name != "attack_counter")
                request += this->player[h].data_int[i]->name + ", ";
        }
    }
    request += "is_useful, score, id_combo, date_added, learning_set) VALUES (";
    for(int h = 0; h <= 1; h++)
    {
        for(int i = 0; i < Player::number_float; i++)
        {
            request += std::to_string(this->player[h].data_float[i]->value) + ", ";
        }
        for(int i = 0; i < Player::number_int; i++)
        {
            if(this->player[h].data_int[i]->name != "attack_counter_2"
            && this->player[h].data_int[i]->name != "attack_counter")
                request += std::to_string(this->player[h].data_int[i]->value) + ", ";
        }
    }
    request += std::to_string(this->player[0].is_useful) + ", " +std::to_string(score) + ", (SELECT id FROM combos WHERE combo = '" + this->action + "'), DATETIME('now', 'localtime'), "
    "(CASE WHEN (SELECT COUNT(*) FROM states WHERE states.learning_set = 'FALSE') * 2  > "
    "(SELECT COUNT(*) FROM states WHERE states.learning_set = 'TRUE') THEN 'TRUE' ELSE 'FALSE' END))"; // IMPORTANT
    return Database::write(request);
}

void Ongoing_action::refresh_is_useful()
{
    this->player[0].is_useful = Player::instance(1)->is_useful;
}

inline
int Ongoing_action::score_calculation()
{ 
    return (Player::instance(1)->life.value - this->player[0].life.value) - (Player::instance(2)->life.value - this->player[1].life.value);
}



#include "player.h"
#include "gui/mainwindow.h"

Player *r_instance1 = 0;
Player *r_instance2 = 0;

vector<float> Player::inputs;

Player::Player(Player const& p)  // copy constructor
{
    this->state = p.state;
    this->life = p.life;
    this->stun = p.stun;
    this->v_trigger = p.v_trigger;
    this->duration = p.duration;
    this->super = p.super;
    this->time_state = p.time_state;
    this->attack_counter = p.attack_counter;

    this->previous_value;
    this->is_useful;

    this->position_x = p.position_x;
    this->position_y = p.position_y;
    this->distance = p.distance;
    this->orientation = p.orientation;

    this->data_float[0] = &this->position_x;
    this->data_float[1] = &this->position_y;
    this->data_float[2] = &this->distance;
    this->data_float[3] = &this->orientation;

    this->data_int[0] = &this->life;
    this->data_int[1] = &this->state;
    this->data_int[2] = &this->stun;
    this->data_int[3] = &this->v_trigger;
    this->data_int[4] = &this->duration;
    this->data_int[5] = &this->super;
    this->data_int[6] = &this->time_state;
    this->data_int[7] = &this->attack_counter;
}

Player* Player::instance(int player_number)
{
    if(player_number == 1)
    {
        if(r_instance1 == NULL)
            r_instance1 = new Player(1);
        return r_instance1;
    }
    else if(player_number == 2)
    {
        if(r_instance2 == NULL)
            r_instance2 = new Player(2);
        return r_instance2;
    }
    return NULL;
}

Player* Player::instance1()
{
    if(r_instance1 == NULL)
        r_instance1 = new Player(1);
    return r_instance1;
}

Player* Player::instance2()
{
    if(r_instance2 == NULL)
        r_instance2 = new Player(2);
    return r_instance2;
}

Player::Player(int player_number)
{
    inputs.resize(number_inputs);
    previous_value = 0;
    if(player_number == 2)
    {
        for(int i = 0; i < number_float; i++)
        {
            data_float[i]->name += "_2";
        }
        for(int i = 0; i < number_int; i++)
        {
            data_int[i]->name += "_2";
        }
    }
}

void Player::refresh_values()
{
    Read_memory::instance()->refresh_value(Player::instance(1)->state.address, Player::instance(1)->state.value);
    Read_memory::instance()->refresh_value(Player::instance(1)->life.address, Player::instance(1)->life.value);
    Read_memory::instance()->refresh_value(Player::instance(1)->stun.address, Player::instance(1)->stun.value);
    Read_memory::instance()->refresh_value(Player::instance(1)->super.address, Player::instance(1)->super.value);
    Read_memory::instance()->refresh_value(Player::instance(1)->v_trigger.address, Player::instance(1)->v_trigger.value);
    Read_memory::instance()->refresh_value(Player::instance(1)->duration.address, Player::instance(1)->duration.value);
    Read_memory::instance()->refresh_value(Player::instance(1)->position_x.address, Player::instance(1)->position_x.value);
    Read_memory::instance()->refresh_value(Player::instance(1)->position_y.address, Player::instance(1)->position_y.value);
    Read_memory::instance()->refresh_value(Player::instance(1)->distance.address, Player::instance(1)->distance.value);
    Read_memory::instance()->refresh_value(Player::instance(1)->orientation.address, Player::instance(1)->orientation.value);
    Read_memory::instance()->refresh_value(Player::instance(1)->time_state.address, Player::instance(1)->time_state.value);

    Read_memory::instance()->refresh_value(Player::instance(1)->attack_counter.address, Player::instance(1)->attack_counter.value);
    if(Player::instance(1)->attack_counter.value != Player::instance(1)->previous_value)
    {
        Player::instance(1)->previous_value = Player::instance(1)->attack_counter.value;
        Player::instance(1)->is_useful = 1;
    }
    else
    {
        Player::instance(1)->is_useful = 0;
    }


    Read_memory::instance()->refresh_value(Player::instance(2)->state.address, Player::instance(2)->state.value);
    Read_memory::instance()->refresh_value(Player::instance(2)->life.address, Player::instance(2)->life.value);
    Read_memory::instance()->refresh_value(Player::instance(2)->stun.address, Player::instance(2)->stun.value);
    Read_memory::instance()->refresh_value(Player::instance(2)->super.address, Player::instance(2)->super.value);
    Read_memory::instance()->refresh_value(Player::instance(2)->v_trigger.address, Player::instance(2)->v_trigger.value);
    Read_memory::instance()->refresh_value(Player::instance(2)->duration.address, Player::instance(2)->duration.value);

    Read_memory::instance()->refresh_value(Player::instance(2)->position_x.address, Player::instance(2)->position_x.value);
    Read_memory::instance()->refresh_value(Player::instance(2)->position_y.address, Player::instance(2)->position_y.value);
    Read_memory::instance()->refresh_value(Player::instance(2)->time_state.address, Player::instance(2)->time_state.value);

    Player::instance(2)->distance.value = Player::instance(1)->distance.value;
    Player::instance(2)->orientation.value = -Player::instance(1)->orientation.value;
}

vector<float> Player::get_inputs() // MODIFY ALSO DATABASE_MANAGER
{
    inputs[0]  = (float)Player::instance(1)->position_x.value/Player::instance(1)->position_x.value_for_normalization;
    inputs[1]  = (float)Player::instance(1)->position_y.value/Player::instance(1)->position_y.value_for_normalization;
    inputs[2]  = (float)Player::instance(2)->position_x.value/Player::instance(2)->position_x.value_for_normalization;
    inputs[3]  = (float)Player::instance(2)->position_y.value/Player::instance(2)->position_y.value_for_normalization;
    inputs[4]  = (float)Player::instance(1)->distance.value/Player::instance(1)->distance.value_for_normalization;
    inputs[5]  = (float)Player::instance(1)->state.value/Player::instance(1)->state.value_for_normalization;
    inputs[6]  = (float)Player::instance(2)->state.value/Player::instance(2)->state.value_for_normalization;
    inputs[7]  = (float)Player::instance(1)->time_state.value/Player::instance(1)->time_state.value_for_normalization;
    inputs[8]  = (float)Player::instance(2)->time_state.value/Player::instance(2)->time_state.value_for_normalization;
    return inputs;
}

vector<float> Player::get_inputs(const vector<string> &result, const int &index)
{
    inputs[0]  = stof(result[index+0])/Player::instance(1)->position_x.value_for_normalization;
    inputs[1]  = stof(result[index+1])/Player::instance(1)->position_y.value_for_normalization;
    inputs[2]  = stof(result[index+2])/Player::instance(2)->position_x.value_for_normalization;
    inputs[3]  = stof(result[index+3])/Player::instance(2)->position_y.value_for_normalization;
    inputs[4]  = stof(result[index+4])/Player::instance(1)->distance.value_for_normalization;
    inputs[5]  = stof(result[index+5])/Player::instance(1)->state.value_for_normalization;
    inputs[6]  = stof(result[index+6])/Player::instance(2)->state.value_for_normalization;
    inputs[7]  = stof(result[index+7])/Player::instance(1)->time_state.value_for_normalization;
    inputs[8]  = stof(result[index+8])/Player::instance(2)->time_state.value_for_normalization;
    return inputs;
}

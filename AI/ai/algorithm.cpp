#include "algorithm.h"

bool Algorithm::has_error = false;
bool Algorithm::has_sort = false;
int Algorithm::score = 0;
int Algorithm::id_single_nn = -1;
int Algorithm::most_use_action = -1;
vector<Ongoing_action> Algorithm::ongoing_actions;

const int Algorithm::number_action = NUMBER_ACTION;

int Algorithm::previous_life[2] = {0,0};

string Algorithm::actions[NUMBER_ACTION+1] = {"ALL", "LP", "MP", "HP", "LLP", "LMP", "LHP",
                                                     "LK", "MK", "HK", "LLK", "LMK", "LHK",
                                                     "L hadoken",
                                                     "L tatsumaki", "M tatsumaki", "H tatsumaki", "EX tatsumaki",
                                                     "advance", "back", "dash", "backdash", "wait 50",
                                                     "shoulder throw", "somersault throw",
                                                     "axe_kick", "solar plexus strike",
                                                     "L shoryuken",
                                                     "jump", "back jump", "forward jump"};

string Algorithm::actions_can_deal_damage[NUMBER_ACTION+1] = {"", "TRUE", "TRUE", "TRUE", "TRUE", "TRUE", "TRUE",
                                                                  "TRUE", "TRUE", "TRUE", "TRUE", "TRUE", "TRUE",
                                                                  "TRUE",
                                                                  "TRUE", "TRUE", "TRUE", "TRUE",
                                                                  "FALSE", "FALSE", "FALSE", "FALSE", "FALSE",
                                                                  "TRUE", "TRUE",
                                                                  "TRUE", "TRUE",
                                                                  "TRUE",
                                                                  "FALSE", "FALSE" , "FALSE"};

float Algorithm::actions_average_damage[NUMBER_ACTION+1] = {0.0};

void Algorithm::mode(bool is_learning) // WARNING TO : Player::refresh_values();
{
    //Thread::instance()->sleep(9000);
    cout << "start fight" << endl;
    initialization();

    if(Player::instance(1)->life.value > 0 && Player::instance(2)->life.value > 0 && GetForegroundWindow() == Read_memory::hwnd)
    {
        Controler::press_button(VK_B, 50, 150);
        while(GetForegroundWindow() == Read_memory::hwnd && has_error == false)
        {
            Thread::instance()->sleep(50);
            Player::refresh_values();

            while(Player::instance(1)->life.value > 0 && Player::instance(2)->life.value > 0 && GetForegroundWindow() == Read_memory::hwnd && has_error == false) // main loop
            {
                Player::refresh_values();
                do
                {
                    ongoing_actions.push_back(Ongoing_action(choose_action(is_learning), *Player::instance(1), *Player::instance(2)));
                    if(!execute_action(ongoing_actions.back().action))
                        has_error = true;
                    Thread::instance()->sleep(50);
                    Player::refresh_values();
                    ongoing_actions.back().refresh_is_useful();
                }
                while(end_of_the_actions() == false && GetForegroundWindow() == Read_memory::hwnd && !has_error && Player::instance(1)->life.value > 0 && Player::instance(2)->life.value > 0);

                if(is_learning == true && GetForegroundWindow() == Read_memory::hwnd)
                {
                    for(int i = 0; i < ongoing_actions.size(); i++)
                        if(!ongoing_actions[i].save())
                            has_error = true;
                }
                ongoing_actions.clear();
                has_sort = false;
            }
            most_use_action = least_present_action();
        }
        Database::write("DELETE FROM states WHERE states.date_added > DATETIME('NOW', 'localtime', '-1 minutes')");
    }
    Thread::instance()->write("Stop fight", "console");
}

inline
bool Algorithm::end_of_the_actions() // TO DEFINE
{
    if((Player::instance(1)->state.value == 0 || Player::instance(2)->state.value == 0)) // CAN ADD STATE 17 AND 16 (move)
    {
        return true;
    }
    else
    {
        return false;
    }
}

string Algorithm::choose_action(bool is_learning)
{
    //cout << most_use_action << endl;
    if(is_learning == true)// TO MODIFY
    {
        int random = Parameters::random_between(0, 2);
        if(random == 1)
              return actions[most_use_action];
        else
            random = Parameters::random_between(1, (NUMBER_ACTION+1));

        return actions[random];

    }
    else
    {
        float score = -2;
        float new_score;
        string name = "no action choose";
        if(Parameters::is_single_nn == false)
        {
            /*for(int i = 0; i < Neural_network_manager::neural_networks.size(); i++)
            {
                if(Neural_network_manager::neural_networks[i].id != 0)
                {
                    new_score = Neural_network_manager::neural_networks[i].floatingOutput(Player::get_inputs()) ;/* * Neural_network_manager::neural_networks[i].clustering_rate/100;
                    if(actions_can_deal_damage[Neural_network_manager::neural_networks[i].id_combo] == "TRUE")
                    {
                        new_score -= 0.3;
                    }
                    //cout << new_score << endl;
                    if(new_score > score)
                    {
                        score = new_score;
                        name = Neural_network_manager::neural_networks[i].name;
                    }
                }
            }*/
        }
        else // if(is_sigle_nn == true)
        {
            vector<float> result = Neural_network_manager::neural_networks[id_single_nn].outputFloat(Player::get_inputs());
            cout << result.size() << endl;
            for(int i = 0; i < result.size(); i++)
            {
                new_score = result[i] * (0.93 + 0.05 * Neural_network_manager::neural_networks[id_single_nn].multi_clustering_rate[i] + 0.02 * actions_average_damage[i]);
                /** if(i == 26)
                {
                    new_score *= 0.9999;
                }
                else if(i == 29)
                {
                    new_score *= 0.97;
                }**/
                if(actions_can_deal_damage[i+1] == "FALSE")
                {
                    new_score *= 0.99;// ARBITRARY
                }
                //cout << new_score << endl;
                if(new_score >= score)
                {
                    score = new_score;
                    name = actions[i+1];
                    //cout << i << " " << result.size() << endl;
                }
            }
        }
        cout << to_string(score) << " " << name << endl;
        return name;
    }
}

inline
bool Algorithm::execute_action(const string &action)// WARNING TO : number timer
{
    if(action == "LP")
    {
        Controler::punch(LIGHT);
    }
    else if(action == "MP")
    {
        Controler::punch(MEDIUM);
    }
    else if(action == "HP")
    {
        Controler::punch(HEAVY);
    }
    else if(action == "LLP")
    {
        Controler::punch(LIGHT, LOW);
    }
    else if(action == "LMP")
    {
        Controler::punch(MEDIUM, LOW);
    }
    else if(action == "LHP")
    {
        Controler::punch(HEAVY, LOW);
    }
    else if(action == "LK")
    {
        Controler::kick(LIGHT);
    }
    else if(action == "MK")
    {
        Controler::kick(MEDIUM);
    }
    else if(action == "HK")
    {
        Controler::kick(HEAVY);
    }
    else if(action == "LLK")
    {
        Controler::kick(LIGHT, LOW);
    }
    else if(action == "LMK")
    {
        Controler::kick(MEDIUM, LOW);
    }
    else if(action == "LHK")
    {
        Controler::kick(HEAVY, LOW);
    }
    else if(action == "L hadoken")
    {
        Controler::hadoken(LIGHT);
    }
    else if(action == "L shoryuken")
    {
        Controler::shoryuken(LIGHT);
    }
    else if(action == "L tatsumaki")
    {
        Controler::tatsumaki(LIGHT);
    }
    else if(action == "M tatsumaki")
    {
        Controler::tatsumaki(MEDIUM);
    }
    else if(action == "H tatsumaki")
    {
        Controler::tatsumaki(HEAVY);
    }
    else if(action == "EX tatsumaki")
    {
        Controler::tatsumaki(EX);
    }
    else if(action == "advance")
    {
        Controler::advance(500);
    }
    else if(action == "back")
    {
        Controler::back(1000);
    }
    else if(action == "dash")
    {
        Controler::dash(FORWARD);
    }
    else if(action == "backdash")
    {
        Controler::dash(BACK);
    }
    else if(action == "wait 50")
    {
        Thread::instance()->sleep(50);
    }
    else if(action == "shoulder throw")
    {
        Controler::shoulder_throw();
    }
    else if(action == "somersault throw")
    {
        Controler::somersault_throw();
    }
    else if(action == "axe_kick")
    {
        Controler::axe_kick();
    }
    else if(action == "solar plexus strike")
    {
        Controler::solar_plexus_strike();
    }
    else if(action == "jump")
    {
        Controler::jump();
    }
    else if(action == "back jump")
    {
        Controler::jump(BACK);
    }
    else if(action == "forward jump")
    {
        Controler::jump(FORWARD);
    }
    else
    {
        Thread::instance()->write("ERROR IN EXECUTE ACTION : " + action, "console");
        return false;
    }
    return true;
}

inline
void Algorithm::sort_actions()// TO MODIFY
{
    vector<string> request = Database::read("SELECT combo, deals_damage FROM combos ORDER BY (SELECT COUNT(*) FROM states WHERE id_combo = combos.id) ASC");
    for(int i = 0, j = 0; i < request.size(); i += 2, j++)
    {
        actions[j] = request[i];
        actions_can_deal_damage[j] = request[i+1];
    }
}

inline
bool Algorithm::initialization()
{
    for(int i = 0; i < Neural_network_manager::neural_networks.size(); i++)
    {
        if(Neural_network_manager::neural_networks[i].id == Parameters::number_single_nn)
        {
            id_single_nn = i;
            break;
        }
    }
    has_error = false;
    for(int i = 1; i < number_action+1; i++)
    {
        if(Database::write("INSERT INTO combos (combo, deals_damage) VALUES ('" + actions[i] + "', '" + actions_can_deal_damage[i] + "')") == false)
        {
            Thread::instance()->write("ERROR IN register_combo :", "console");
            Thread::instance()->write("aaa", "console");
            return false;
        }
    }
    vector<string> request = Database::read("SELECT average_damage FROM combos ORDER BY id");
    for(int i = 1; i < request.size();i++)
        actions_average_damage[i] = stof(request[i]);

    most_use_action = least_present_action();
    //sort_actions(); // WARNING TO : after use actions_can_deal_damage
    SetForegroundWindow(Read_memory::hwnd);
    Thread::instance()->sleep(50);
    Thread::instance()->write("Start fight", "console");
    return true;
}

inline
int Algorithm::least_present_action()
{
    vector<string> request = Database::read("SELECT id_combo, COUNT(*) tmp FROM states GROUP BY id_combo ORDER BY tmp ASC LIMIT 1");
    if(!request.empty())
        return stoi(request[0]);
    else
        return 1;
}

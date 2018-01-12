#ifndef ONGOING_ACTION_H
#define ONGOING_ACTION_H
#include <environnement/player.h>
#include <environnement/database.h>

class Ongoing_action
{
    public:
        Ongoing_action();
        Ongoing_action(const string &action, const Player &player1, const Player &player2);
        ~Ongoing_action();
        string action;
        bool save();
        void refresh_is_useful();

    private:
        Player player[2] = {Player(1), Player(2)};
        int score_calculation();
};

#endif // ONGOING_ACTION_H

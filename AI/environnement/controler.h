#ifndef CONTROLER_H
#define CONTROLER_H

#include "player.h"
#include "parameters.h"

class Controler
{
public:
    static void press_button(char button, int time = 0, int time_after = 0);
    static void press_2_buttons(char button1, char button2, int time = 0, int time_after = 0);
    static void down_button(char button);
    static void up_button(char button);

    static void advance(int time);
    static void back(int time);
    static void dash(int direction = 0);
    static void crouch(int time);
    static void crouching_block(int time);
    static void jump(int direction = 0);

    static void punch(int power, bool is_low = false);
    static void kick(int power, bool is_low = false);
    static void hadoken(int power);
    static void shoryuken(int power);
    static void shinku_hadoken(int power = LIGHT);
    static void tatsumaki(int power);
    static void collarbone_breaker();
    static void solar_plexus_strike();
    static void axe_kick();
    static void denjin_renki();
    static void mind_s_eye();
    static void hashogeki();
    static void shoulder_throw();
    static void somersault_throw();



};

#endif // CONTROLER_H

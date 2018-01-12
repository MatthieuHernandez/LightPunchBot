#include <windows.h>
#include "environnement/controler.h"
#include "gui/thread.h"

#define PRESSING_TIME 50

void Controler::press_button(char button, int time, int time_after)
{
    keybd_event(button, 0, 0, 0);
    Thread::instance()->sleep(time);
    keybd_event(button, 0, KEYEVENTF_KEYUP, 0);
    if(time_after > 0)
        Thread::instance()->sleep(time_after);
}

void Controler::press_2_buttons(char button1, char button2, int time, int time_after)
{
    keybd_event(button1, 0, 0, 0);
    keybd_event(button2, 0, 0, 0);
    Thread::instance()->sleep(time);
    keybd_event(button1, 0, KEYEVENTF_KEYUP, 0);
    keybd_event(button2, 0, KEYEVENTF_KEYUP, 0);
    if(time_after > 0)
        Thread::instance()->sleep(time_after);
}

void Controler::down_button(char button)
{
    keybd_event(button, 0, 0, 0);
}

void Controler::up_button(char button)
{
    keybd_event(button, 0, KEYEVENTF_KEYUP, 0);
}

//------------------------------------------------------------------------------------
//                                 Control Character
//------------------------------------------------------------------------------------

void Controler::advance(int time)
{
    if(Player::instance(1)->orientation.value > 0)
    {
        keybd_event(VK_RIGHT, 0, 0, 0);
        Thread::instance()->sleep(time);
        keybd_event(VK_RIGHT, 0, KEYEVENTF_KEYUP, 0);
    }
    else
    {
        keybd_event(VK_LEFT, 0, 0, 0);
        Thread::instance()->sleep(time);
        keybd_event(VK_LEFT, 0, KEYEVENTF_KEYUP, 0);
    }

}

void Controler::back(int time)
{
    if(Player::instance(1)->orientation.value > 0)
    {
        keybd_event(VK_LEFT, 0, 0, 0);
        Thread::instance()->sleep(time);
        keybd_event(VK_LEFT, 0, KEYEVENTF_KEYUP, 0);
    }
    else
    {
        keybd_event(VK_RIGHT, 0, 0, 0);
        Thread::instance()->sleep(time);
        keybd_event(VK_RIGHT, 0, KEYEVENTF_KEYUP, 0);
    }
}

void Controler::dash(int direction)
{
    if((direction == FORWARD && Player::instance(1)->orientation.value > 0)
    || (direction == BACK && Player::instance(1)->orientation.value < 0)
    || (direction == FORWARD && Player::instance(1)->orientation.value == 0))
    {
        keybd_event(VK_RIGHT, 0, 0, 0);
        Thread::instance()->sleep(PRESSING_TIME-20);
        keybd_event(VK_RIGHT, 0, KEYEVENTF_KEYUP, 0);
        Thread::instance()->sleep(PRESSING_TIME-20);
        keybd_event(VK_RIGHT, 0, 0, 0);
        Thread::instance()->sleep(PRESSING_TIME-20);
        keybd_event(VK_RIGHT, 0, KEYEVENTF_KEYUP, 0);
    }
    else
    {
        keybd_event(VK_LEFT, 0, 0, 0);
        Thread::instance()->sleep(PRESSING_TIME-20);
        keybd_event(VK_LEFT, 0, KEYEVENTF_KEYUP, 0);
        Thread::instance()->sleep(PRESSING_TIME-20);
        keybd_event(VK_LEFT, 0, 0, 0);
        Thread::instance()->sleep(PRESSING_TIME-20);
        keybd_event(VK_LEFT, 0, KEYEVENTF_KEYUP, 0);
    }
}

void Controler::crouch(int time)
{
    keybd_event(VK_DOWN, 0, 0, 0);
    Thread::instance()->sleep(time);
    keybd_event(VK_DOWN, 0, KEYEVENTF_KEYUP, 0);
}

void Controler::crouching_block(int time)
{
    keybd_event(VK_DOWN, 0, 0, 0);
    if(Player::instance(1)->orientation.value > 0)
    {
        keybd_event(VK_LEFT, 0, 0, 0);
        Thread::instance()->sleep(time);
        keybd_event(VK_LEFT, 0, KEYEVENTF_KEYUP, 0);
    }
    else
    {
        keybd_event(VK_RIGHT, 0, 0, 0);
        Thread::instance()->sleep(time);
        keybd_event(VK_RIGHT, 0, KEYEVENTF_KEYUP, 0);
    }
    keybd_event(VK_DOWN, 0, KEYEVENTF_KEYUP, 0);
}

void Controler::jump(int direction)
{
    if((direction == FORWARD && Player::instance(1)->orientation.value > 0)
    || (direction == BACK && Player::instance(1)->orientation.value < 0)
    || (direction == FORWARD && Player::instance(1)->orientation.value == 0))
    {
        keybd_event(VK_UP, 0, 0, 0);
        keybd_event(VK_RIGHT, 0, 0, 0);
        Thread::instance()->sleep(PRESSING_TIME);
        keybd_event(VK_UP, 0, KEYEVENTF_KEYUP, 0);
        keybd_event(VK_RIGHT, 0, KEYEVENTF_KEYUP, 0);

    }
    else if((direction == BACK && Player::instance(1)->orientation.value > 0)
         || (direction == FORWARD && Player::instance(1)->orientation.value < 0)
         || (direction == BACK && Player::instance(1)->orientation.value == 0))
    {
        keybd_event(VK_UP, 0, 0, 0);
        keybd_event(VK_LEFT, 0, 0, 0);
        Thread::instance()->sleep(PRESSING_TIME);
        keybd_event(VK_UP, 0, KEYEVENTF_KEYUP, 0);
        keybd_event(VK_LEFT, 0, KEYEVENTF_KEYUP, 0);
    }
    else
    {
        keybd_event(VK_UP, 0, 0, 0);
        Thread::instance()->sleep(PRESSING_TIME);
        keybd_event(VK_UP, 0, KEYEVENTF_KEYUP, 0);
    }
}

void Controler::punch(int power, bool is_low)
{
    if(is_low)
    {
        keybd_event(VK_DOWN, 0, 0, 0);
    }
    if(power == LIGHT)
    {
        keybd_event(VK_LP, 0, 0, 0);
        Thread::instance()->sleep(PRESSING_TIME);
        keybd_event(VK_LP, 0, KEYEVENTF_KEYUP, 0);
    }
    else if(power == MEDIUM)
    {
        keybd_event(VK_MP, 0, 0, 0);
        Thread::instance()->sleep(PRESSING_TIME);
        keybd_event(VK_MP, 0, KEYEVENTF_KEYUP, 0);
    }
    else if(power == HEAVY)
    {
        keybd_event(VK_HP, 0, 0, 0);
        Thread::instance()->sleep(PRESSING_TIME);
        keybd_event(VK_HP, 0, KEYEVENTF_KEYUP, 0);
    }
    if(is_low)
    {
        keybd_event(VK_DOWN, 0, KEYEVENTF_KEYUP, 0);
    }
}

void Controler::kick(int power, bool is_low)
{
    if(is_low)
    {
        keybd_event(VK_DOWN, 0, 0, 0);
    }
    if(power == LIGHT)
    {
        keybd_event(VK_LK, 0, 0, 0);
        Thread::instance()->sleep(PRESSING_TIME);
        keybd_event(VK_LK, 0, KEYEVENTF_KEYUP, 0);
    }
    else if(power == MEDIUM)
    {
        keybd_event(VK_MK, 0, 0, 0);
        Thread::instance()->sleep(PRESSING_TIME);
        keybd_event(VK_MK, 0, KEYEVENTF_KEYUP, 0);
    }
    else if(power == HEAVY)
    {
        keybd_event(VK_HK, 0, 0, 0);
        Thread::instance()->sleep(PRESSING_TIME);
        keybd_event(VK_HK, 0, KEYEVENTF_KEYUP, 0);
    }
    if(is_low)
    {
        keybd_event(VK_DOWN, 0, KEYEVENTF_KEYUP, 0);
    }
}

void Controler::hadoken(int power)
{
    keybd_event(VK_DOWN, 0, 0, 0);
    Thread::instance()->sleep(PRESSING_TIME);

    if(Player::instance(1)->orientation.value > 0)
    {
        keybd_event(VK_RIGHT, 0, 0, 0);
        Thread::instance()->sleep(PRESSING_TIME);
        keybd_event(VK_DOWN, 0, KEYEVENTF_KEYUP, 0);
        Thread::instance()->sleep(PRESSING_TIME);
        keybd_event(VK_RIGHT, 0, KEYEVENTF_KEYUP, 0);
    }
    else
    {
        keybd_event(VK_LEFT, 0, 0, 0);
        Thread::instance()->sleep(PRESSING_TIME);
        keybd_event(VK_DOWN, 0, KEYEVENTF_KEYUP, 0);
        Thread::instance()->sleep(PRESSING_TIME);
        keybd_event(VK_LEFT, 0, KEYEVENTF_KEYUP, 0);
    }
    if(power == LIGHT)
    {
        keybd_event(VK_LP, 0, 0, 0);
        Thread::instance()->sleep(PRESSING_TIME);
        keybd_event(VK_LP, 0, KEYEVENTF_KEYUP, 0);
    }
    else if(power == MEDIUM)
    {
        keybd_event(VK_MP, 0, 0, 0);
        Thread::instance()->sleep(PRESSING_TIME);
        keybd_event(VK_MP, 0, KEYEVENTF_KEYUP, 0);
    }
    else if(power == HEAVY)
    {
        keybd_event(VK_HP, 0, 0, 0);
        Thread::instance()->sleep(PRESSING_TIME);
        keybd_event(VK_HP, 0, KEYEVENTF_KEYUP, 0);
    }
    else if(power == EX)
    {
        keybd_event(VK_3P, 0, 0, 0);
        Thread::instance()->sleep(PRESSING_TIME);
        keybd_event(VK_3P, 0, KEYEVENTF_KEYUP, 0);
    }
}

void Controler::shoryuken(int power)
{
    if(Player::instance(1)->orientation.value > 0)
    {
        keybd_event(VK_RIGHT, 0, 0, 0);
        Thread::instance()->sleep(PRESSING_TIME);
        keybd_event(VK_RIGHT, 0, KEYEVENTF_KEYUP, 0);

        keybd_event(VK_DOWN, 0, 0, 0);
        Thread::instance()->sleep(PRESSING_TIME);

        keybd_event(VK_RIGHT, 0, 0, 0);
        Thread::instance()->sleep(PRESSING_TIME);
        keybd_event(VK_RIGHT, 0, KEYEVENTF_KEYUP, 0);
        keybd_event(VK_DOWN, 0, KEYEVENTF_KEYUP, 0);

    }
    else
    {
        keybd_event(VK_LEFT, 0, 0, 0);
        Thread::instance()->sleep(PRESSING_TIME);
        keybd_event(VK_LEFT, 0, KEYEVENTF_KEYUP, 0);

        keybd_event(VK_DOWN, 0, 0, 0);
        Thread::instance()->sleep(PRESSING_TIME);

        keybd_event(VK_LEFT, 0, 0, 0);
        Thread::instance()->sleep(PRESSING_TIME);
        keybd_event(VK_LEFT, 0, KEYEVENTF_KEYUP, 0);
        keybd_event(VK_DOWN, 0, KEYEVENTF_KEYUP, 0);
    }
    if(power == LIGHT)
    {
        keybd_event(VK_LP, 0, 0, 0);
        Thread::instance()->sleep(PRESSING_TIME);
        keybd_event(VK_LP, 0, KEYEVENTF_KEYUP, 0);
    }
    else if(power == MEDIUM)
    {
        keybd_event(VK_MP, 0, 0, 0);
        Thread::instance()->sleep(PRESSING_TIME);
        keybd_event(VK_MP, 0, KEYEVENTF_KEYUP, 0);
    }
    else if(power == HEAVY)
    {
        keybd_event(VK_HP, 0, 0, 0);
        Thread::instance()->sleep(PRESSING_TIME);
        keybd_event(VK_HP, 0, KEYEVENTF_KEYUP, 0);
    }
    else if(power == EX)
    {
        keybd_event(VK_3P, 0, 0, 0);
        Thread::instance()->sleep(PRESSING_TIME);
        keybd_event(VK_3P, 0, KEYEVENTF_KEYUP, 0);
    }
}

void Controler::shinku_hadoken(int power)
{
    keybd_event(VK_DOWN, 0, 0, 0);
    Thread::instance()->sleep(PRESSING_TIME);

    if(Player::instance(1)->orientation.value > 0)
    {
        keybd_event(VK_RIGHT, 0, 0, 0);
        Thread::instance()->sleep(PRESSING_TIME);
        keybd_event(VK_DOWN, 0, KEYEVENTF_KEYUP, 0);
        Thread::instance()->sleep(PRESSING_TIME);
        keybd_event(VK_RIGHT, 0, KEYEVENTF_KEYUP, 0);
        Thread::instance()->sleep(PRESSING_TIME);
        keybd_event(VK_DOWN, 0, 0, 0);
        Thread::instance()->sleep(PRESSING_TIME);
        keybd_event(VK_RIGHT, 0, 0, 0);
        Thread::instance()->sleep(PRESSING_TIME);
        keybd_event(VK_DOWN, 0, KEYEVENTF_KEYUP, 0);
        Thread::instance()->sleep(PRESSING_TIME);
        keybd_event(VK_RIGHT, 0, KEYEVENTF_KEYUP, 0);
    }
    else
    {
        keybd_event(VK_LEFT, 0, 0, 0);
        Thread::instance()->sleep(PRESSING_TIME);
        keybd_event(VK_DOWN, 0, KEYEVENTF_KEYUP, 0);
        Thread::instance()->sleep(PRESSING_TIME);
        keybd_event(VK_LEFT, 0, KEYEVENTF_KEYUP, 0);
        Thread::instance()->sleep(PRESSING_TIME);
        keybd_event(VK_DOWN, 0, 0, 0);
        Thread::instance()->sleep(PRESSING_TIME);
        keybd_event(VK_LEFT, 0, 0, 0);
        Thread::instance()->sleep(PRESSING_TIME);
        keybd_event(VK_DOWN, 0, KEYEVENTF_KEYUP, 0);
        Thread::instance()->sleep(PRESSING_TIME);
        keybd_event(VK_LEFT, 0, KEYEVENTF_KEYUP, 0);
    }
    if(power == LIGHT)
    {
        keybd_event(VK_LP, 0, 0, 0);
        Thread::instance()->sleep(PRESSING_TIME);
        keybd_event(VK_LP, 0, KEYEVENTF_KEYUP, 0);
    }
    else if(power == MEDIUM)
    {
        keybd_event(VK_MP, 0, 0, 0);
        Thread::instance()->sleep(PRESSING_TIME);
        keybd_event(VK_MP, 0, KEYEVENTF_KEYUP, 0);
    }
    else if(power == HEAVY)
    {
        keybd_event(VK_HP, 0, 0, 0);
        Thread::instance()->sleep(PRESSING_TIME);
        keybd_event(VK_HP, 0, KEYEVENTF_KEYUP, 0);
    }
}

void Controler::tatsumaki(int power)
{
    keybd_event(VK_DOWN, 0, 0, 0);
    Thread::instance()->sleep(PRESSING_TIME);

    if(Player::instance(1)->orientation.value > 0)
    {
        keybd_event(VK_LEFT, 0, 0, 0);
        Thread::instance()->sleep(PRESSING_TIME*3);
        keybd_event(VK_DOWN, 0, KEYEVENTF_KEYUP, 0);
        Thread::instance()->sleep(PRESSING_TIME);
        keybd_event(VK_LEFT, 0, KEYEVENTF_KEYUP, 0);
    }
    else
    {
        keybd_event(VK_RIGHT, 0, 0, 0);
        Thread::instance()->sleep(PRESSING_TIME*3);
        keybd_event(VK_DOWN, 0, KEYEVENTF_KEYUP, 0);
        Thread::instance()->sleep(PRESSING_TIME);
        keybd_event(VK_RIGHT, 0, KEYEVENTF_KEYUP, 0);
    }
    if(power == LIGHT)
    {
        press_button(VK_B, PRESSING_TIME);
    }
    else if(power == MEDIUM)
    {
        press_button(VK_N, PRESSING_TIME);
    }
    else if(power == HEAVY)
    {
        press_button(VK_M, PRESSING_TIME);
    }
    else if(power == EX)
    {
        press_button(VK_3K, PRESSING_TIME);
    }
}

void Controler::collarbone_breaker()
{
    keybd_event(VK_MP, 0, 0, 0);
    if(Player::instance(1)->orientation.value > 0)
    {
        keybd_event(VK_LEFT, 0, 0, 0);
        Thread::instance()->sleep(PRESSING_TIME);
        keybd_event(VK_LEFT, 0, KEYEVENTF_KEYUP, 0);
    }
    else
    {
        keybd_event(VK_RIGHT, 0, 0, 0);
        Thread::instance()->sleep(PRESSING_TIME);
        keybd_event(VK_RIGHT, 0, KEYEVENTF_KEYUP, 0);
    }
    keybd_event(VK_DOWN, 0, KEYEVENTF_KEYUP, 0);
}

void Controler::solar_plexus_strike()
{
    keybd_event(VK_HP, 0, 0, 0);

    if(Player::instance(1)->orientation.value > 0)
    {
        keybd_event(VK_RIGHT, 0, 0, 0);
        Thread::instance()->sleep(PRESSING_TIME+10);
        keybd_event(VK_RIGHT, 0, KEYEVENTF_KEYUP, 0);
    }
    else
    {
        keybd_event(VK_LEFT, 0, 0, 0);
        Thread::instance()->sleep(PRESSING_TIME+10);
        keybd_event(VK_LEFT, 0, KEYEVENTF_KEYUP, 0);
    }
    keybd_event(VK_HP, 0, KEYEVENTF_KEYUP, 0);
}

void Controler::axe_kick()
{
    keybd_event(VK_HK, 0, 0, 0);

    if(Player::instance(1)->orientation.value > 0)
    {
        keybd_event(VK_LEFT, 0, 0, 0);
        Thread::instance()->sleep(PRESSING_TIME+10);
        keybd_event(VK_LEFT, 0, KEYEVENTF_KEYUP, 0);
    }
    else
    {
        keybd_event(VK_RIGHT, 0, 0, 0);
        Thread::instance()->sleep(PRESSING_TIME+10);
        keybd_event(VK_RIGHT, 0, KEYEVENTF_KEYUP, 0);
    }
    keybd_event(VK_HK, 0, KEYEVENTF_KEYUP, 0);
}

void Controler::denjin_renki()
{
    keybd_event(VK_HP, 0, 0, 0);
    keybd_event(VK_HK, 0, 0, 0);
    Thread::instance()->sleep(PRESSING_TIME);
    keybd_event(VK_HP, 0, KEYEVENTF_KEYUP, 0);
    keybd_event(VK_HK, 0, KEYEVENTF_KEYUP, 0);
}

void Controler::mind_s_eye()
{
    keybd_event(VK_MP, 0, 0, 0);
    keybd_event(VK_MK, 0, 0, 0);
    Thread::instance()->sleep(PRESSING_TIME);
    keybd_event(VK_MP, 0, KEYEVENTF_KEYUP, 0);
    keybd_event(VK_MK, 0, KEYEVENTF_KEYUP, 0);
}

void Controler::hashogeki()
{
    keybd_event(VK_3P, 0, 0, 0);
    if(Player::instance(1)->orientation.value > 0)
    {
        keybd_event(VK_RIGHT, 0, 0, 0);
        Thread::instance()->sleep(PRESSING_TIME);
        keybd_event(VK_RIGHT, 0, KEYEVENTF_KEYUP, 0);
    }
    else
    {
        keybd_event(VK_LEFT, 0, 0, 0);
        Thread::instance()->sleep(PRESSING_TIME);
        keybd_event(VK_LEFT, 0, KEYEVENTF_KEYUP, 0);
    }
    keybd_event(VK_DOWN, 0, KEYEVENTF_KEYUP, 0);
}

void Controler::shoulder_throw()
{
    keybd_event(VK_LP, 0, 0, 0);
    keybd_event(VK_LK, 0, 0, 0);
    Thread::instance()->sleep(PRESSING_TIME);
    keybd_event(VK_LP, 0, KEYEVENTF_KEYUP, 0);
    keybd_event(VK_LK, 0, KEYEVENTF_KEYUP, 0);
}

void Controler::somersault_throw()
{
    keybd_event(VK_LP, 0, 0, 0);
    keybd_event(VK_LK, 0, 0, 0);
    if(Player::instance(1)->orientation.value > 0)
    {
        keybd_event(VK_LEFT, 0, 0, 0);
        Thread::instance()->sleep(PRESSING_TIME);
        keybd_event(VK_LEFT, 0, KEYEVENTF_KEYUP, 0);
    }
    else
    {
        keybd_event(VK_RIGHT, 0, 0, 0);
        Thread::instance()->sleep(PRESSING_TIME);
        keybd_event(VK_RIGHT, 0, KEYEVENTF_KEYUP, 0);
    }
    keybd_event(VK_LP, 0, KEYEVENTF_KEYUP, 0);
    keybd_event(VK_LK, 0, KEYEVENTF_KEYUP, 0);
}

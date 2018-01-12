#include "data_recovery.h"

#include "environnement/read_memory.h"
#include "gui/thread.h"
#include "controler.h"

Data_recovery::Data_recovery()
{

}

Data_recovery::~Data_recovery()
{

}

void Data_recovery::find_values(int version)
{
    Thread::instance()->write("Start to find values", "console");
    time_t chrono = time(NULL);
    if(version == 5) // 5
    {
        SetForegroundWindow(Read_memory::hwnd);
        if(Read_memory::instance()->ready)
        {
            //if(Player::instance(2)->life.address > Player::instance(1)->life.address)
            //else
            Thread::instance()->sleep(50);
            Thread::instance()->write("instance finish", "console");
            Controler::press_button(VK_RETURN, 100, 500);
            Controler::press_button(VK_B, 100, 200);

            vector<int> values_is_useful {9830400, 65536, 65536000, 3932160, 65536000};
            vector<int> position_is_useful {0, -36, -16, -4, -4};
            Player::instance(1)->attack_counter.address = Read_memory::instance()->find_data(0, values_is_useful, position_is_useful, 32);
            Player::instance(1)->attack_counter.address = Read_memory::instance()->find_data(Player::instance(1)->attack_counter.address, values_is_useful, position_is_useful, 32);
            if(Player::instance(1)->attack_counter.address <= 0)
                Thread::instance()->write("ERROR : is_useful not found", "console");
            else
                Thread::instance()->write("is_useful found", "console");

            vector<int> values_life_2 {500, 1, 65};
            vector<int> position_life_2 {0, -4, 40};
            Player::instance(2)->life.address = Read_memory::instance()->find_data(0, values_life_2, position_life_2);
            Player::instance(2)->life.address = Read_memory::instance()->find_data(Player::instance(2)->life.address, values_life_2, position_life_2);////////
            if(Player::instance(2)->life.address <= 0)
                Thread::instance()->write("ERROR : life_2 not found", "console");
            else
                Thread::instance()->write("life_2 found", "console");

            vector<int> values_life {500, 1, 65};
            vector<int> position_life {0, -4, 40};
            Player::instance(1)->life.address = Read_memory::instance()->find_data(Player::instance(2)->life.address + Read_memory::page_size, values_life, position_life);
            if(Player::instance(1)->life.address <= 0)
                Thread::instance()->write("ERROR : life not found", "console");
            else
                Thread::instance()->write("life found", "console");

            vector<int> values_stun {0, 3, 2, 3, 1140457472};
            vector<int> position_stun {0, -12, -4, -4, 40};
            Player::instance(1)->stun.address = Read_memory::instance()->find_data(0, values_stun, position_stun);
            if(Player::instance(1)->stun.address <= 0)
                Thread::instance()->write("ERROR : stun_2 not found", "console");
            else
                Thread::instance()->write("stun found", "console");

            vector<int> values_stun_2 {0, 3, 2, 3, 1140457472};
            vector<int> position_stun_2 {0, -20, -4, -4, 40};
            Player::instance(2)->stun.address = Read_memory::instance()->find_data(Player::instance(2)->stun.address, values_stun_2, position_stun_2);
            if(Player::instance(2)->stun.address <= 0)
                Thread::instance()->write("ERROR : stun not found", "console");
            else
                Thread::instance()->write("stun_2 found", "console");

            //vector<int> values_time_state {7, 1, 1077473216, 1, 1077607248, 1, 1100435408, 1, 1082186656, 1, 1114581104, 0, 2147483647, 0, 4, 4/*, 1117300384*/};*/
            //vector<int> position_time_state {0, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, 60, 4, 4, 4/*, 80*/}; // +12

            vector<int> values_time_state_2 {1004, 8, 1086093136};
            vector<int> position_time_state_2 {0, -4, -36};
            Player::instance(2)->time_state.address = Read_memory::instance()->find_data(0, values_time_state_2, position_time_state_2, 4);
            if(Player::instance(2)->time_state.address <= 0)
                Thread::instance()->write("ERROR : time_state_2 not found", "console");
            else
                Thread::instance()->write("time_state_2 found", "console");

            vector<int> values_time_state {1004, 6, 1086093136};
            vector<int> position_time_state {0, -4, -36};
            Player::instance(1)->time_state.address = Read_memory::instance()->find_data(0, values_time_state, position_time_state, 4); // +12
            if(Player::instance(1)->time_state.address <= 0)
                Thread::instance()->write("ERROR : time_state not found", "console");
            else
                Thread::instance()->write("time_state found", "console");

            vector<int> values_orientation {1119092736, 22, 14, 0};
            vector<int> position_orientation {0, -20, -4, -4};
            Player::instance(1)->orientation.address = Read_memory::instance()->find_data(0, values_orientation, position_orientation);
            if(Player::instance(1)->orientation.address <= 0)
                Thread::instance()->write("ERROR : orientation not found", "console");
            else
                Thread::instance()->write("orientation found", "console");

            vector<int> values_position_x {-1077936128, 0, 0, 1065353216, -1077936128}; // -1.5
            vector<int> position_position_x {0, 4, 4, 4, 4}; // Player::instance(1)->orientation.address
            Player::instance(1)->position_x.address = Read_memory::instance()->find_data(0, values_position_x, position_position_x); // Player::instance(2)->state.address
            Player::instance(1)->position_y.address = Player::instance(1)->position_x.address + 4;
            if(Player::instance(1)->position_x.address <= 0)
                Thread::instance()->write("ERROR : position_x not found", "console");
            else
                Thread::instance()->write("position_x found", "console");

            vector<int> values_distance {1076887552, 1, 1}; // 2.75
            vector<int> position_distance {0, -4, -4};
            Player::instance(1)->distance.address = Read_memory::instance()->find_data(Player::instance(1)->position_y.address, values_distance, position_distance);
            if(Player::instance(1)->distance.address <= 0)
                Thread::instance()->write("ERROR : distance not found", "console");
            else
                Thread::instance()->write("distance found", "console");

            vector<int> values_v_trigger {600, 2, 65536};
            vector<int> position_v_trigger {0, 100, 4};
            Player::instance(1)->v_trigger.address = Read_memory::instance()->find_data(Player::instance(1)->distance.address, values_v_trigger, position_v_trigger);
            Player::instance(1)->super.address = Player::instance(1)->v_trigger.address - 8;
            Player::instance(1)->duration.address = Player::instance(1)->v_trigger.address + 4;
            if(Player::instance(1)->v_trigger.address <= 0)
                Thread::instance()->write("ERROR : v_trigger not found", "console");
            else
                Thread::instance()->write("v_trigger found", "console");

            vector<int> values_position_x_2 {1069547520, 1, 0, 1069547520, 1069547520, 1069547520}; // 1.5
            vector<int> position_position_x_2 {0, -32, 36, 12, 16, 16};
            Player::instance(2)->position_x.address = Read_memory::instance()->find_data(0, values_position_x_2, position_position_x_2);
            Player::instance(2)->position_y.address = Player::instance(2)->position_x.address + 4;
            if(Player::instance(2)->position_x.address <= 0)
                Thread::instance()->write("ERROR : position_x_2 not found", "console");
            else
                Thread::instance()->write("position_x_2 found", "console");

            vector<int> values_v_trigger_2 {900, 2, 65536};
            vector<int> position_v_trigger_2 {0, 100, 4};
            Player::instance(2)->v_trigger.address = Read_memory::instance()->find_data(Player::instance(2)->position_y.address, values_v_trigger_2, position_v_trigger_2);
            Player::instance(2)->duration.address = Player::instance(2)->v_trigger.address + 4;
            Player::instance(2)->super.address = Player::instance(2)->v_trigger.address - 8;
            if(Player::instance(2)->v_trigger.address <= 0)
                Thread::instance()->write("ERROR : v_trigger_2 not found", "console");
            else
                Thread::instance()->write("v_trigger_2 found", "console");

            vector<int> values_state {65536, 65536, 128, 128};
            vector<int> position_state {0, 4, 4, 4};
            Player::instance(1)->state.address = Read_memory::instance()->find_data(0, values_state, position_state, -28);
            if(Player::instance(1)->state.address <= 0)
                Thread::instance()->write("ERROR : state not found", "console");
            else
                Thread::instance()->write("state found", "console");

            vector<int> values_state_2 {65536, 65536, 64, 64};
            vector<int> position_state_2 {0, 4, 4, 4};
            Player::instance(2)->state.address = Read_memory::instance()->find_data(0, values_state_2, position_state_2, -28);
            if(Player::instance(2)->state.address <= 0)
                Thread::instance()->write("ERROR : state_2 not found", "console");
            else
                Thread::instance()->write("state_2 found", "console");

        }
    }
    else if(version == 5)
    {
        SetForegroundWindow(Read_memory::hwnd);
        if(Read_memory::instance()->ready)
        {
            Thread::instance()->write("instance finish", "console");
            //------------------------------------------------------------------------------------
            //                                      PLAYERS
            //------------------------------------------------------------------------------------

            /*Controler::press_button(VK_RETURN, 100, 500);
            Controler::press_button(VK_K, 50, 200);
            Controler::press_button(VK_DOWN, 50, 200);
            Controler::press_button(VK_LEFT, 50, 200);
            Controler::press_button(VK_LEFT, 50, 200);
            Controler::press_button(VK_J, 50, 200);
            Controler::press_button(VK_B, 50, 500);*/

            /*Controler::press_button(VK_RETURN, 100, 500);
            Controler::press_button(VK_B, 100, 200);
            Read_memory::instance()->reach_data("==", -1.5f);
            Controler::press_button(VK_LEFT, 8000);
            Read_memory::instance()->reach_data("==", -3.5f);
            Player::instance(1)->position_x.address = Read_memory::instance()->return_address();
            if(Player::instance(1)->position_x.address <= 0)
                Thread::instance()->write("position_x not find", "console");

            //------------------------------------------------------------------------------------

            Player::instance(1)->position_y.address = Player::instance(1)->position_x.address + 4;
            if(Player::instance(1)->position_y.address <= 4)
                Thread::instance()->write("position_y not find", "console");*/

            //------------------------------------------------------------------------------------

            /*Player::instance(1)->distance.address = Player::instance(1)->position_y.address + 972;
            if(Player::instance(1)->distance.address <= 972)
                Thread::instance()->write("distance not find", "console");*/

            //keep in comment
            /*Read_memory::instance()->reach_data("==", 4.75f, 0.0f, RESET);
            Controler::press_button(VK_RETURN, 100, 500);
            Controler::press_button(VK_B, 50, 200);
            Read_memory::instance()->reach_data("==", 2.75f);
            Player::instance(1)->distance.address = Read_memory::instance()->return_address();
            if(Player::instance(1)->distance.address <= 0)
                Thread::instance()->write("distance not find", "console");*/

            //------------------------------------------------------------------------------------

            /*Controler::press_button(VK_RETURN, 100, 500);
            Controler::press_button(VK_B, 100, 100);
            Read_memory::instance()->reach_data("==", 1.5f);
            Controler::press_button(VK_RIGHT, 7000);
            Read_memory::instance()->reach_data("==", 7.5f);
            Controler::press_button(VK_RETURN, 100, 500);
            Controler::press_button(VK_B, 100, 100);
            Read_memory::instance()->reach_data("==", 1.5f);
            Player::instance(2)->position_x.address = Read_memory::instance()->return_address();
            if(Player::instance(2)->position_x.address <= 0)
                Thread::instance()->write("position_x not find", "console");*/

            //------------------------------------------------------------------------------------

            /*Player::instance(2)->position_y.address = Player::instance(2)->position_x.address + 4;
            Player::instance(2)->position_y.virtual_address = Player::instance(2)->position_x.virtual_address + 4;
            if(Player::instance(2)->position_y.address <= 4)
                Thread::instance()->write("position_y not find", "console");*/

            //------------------------------------------------------------------------------------

            /*Controler::press_button(VK_RETURN, 100, 500);
            Controler::press_button(VK_B, 50, 200);
            Controler::down_button(VK_LEFT);
            Thread::instance()->sleep(250);
            Read_memory::instance()->reach_data("==", 17, 0, RESET);
            Controler::up_button(VK_LEFT);
            Controler::down_button(VK_RIGHT);
            Thread::instance()->sleep(250);
            Read_memory::instance()->reach_data("==", 16);
            Controler::up_button(VK_RIGHT);
            Thread::instance()->sleep(500);
            Read_memory::instance()->reach_data("==", 0);
            Controler::press_button(VK_J, 100);
            Controler::press_button(VK_RETURN, 100, 200);
            Read_memory::instance()->reach_data("==", 602);
            Controler::press_button(VK_RETURN, 100, 200);
            Player::instance(1)->state.address = Read_memory::instance()->return_address();
            if(Player::instance(1)->state.address <= 0)
                Thread::instance()->write("state not find", "console");

            //------------------------------------------------------------------------------------

            Controler::press_button(VK_RETURN, 100, 500);
            Controler::press_button(VK_B, 50, 200);
            Controler::press_button(VK_RIGHT, 2000);
            Controler::press_button(VK_H, 50, 700);
            Controler::press_button(VK_RETURN, 100, 250);
            Read_memory::instance()->reach_data("==", 470, 0, RESET);
            Controler::press_button(VK_RETURN, 100, 250);
            Thread::instance()->sleep(2200);

            Controler::press_button(VK_K, 50, 700);
            Controler::press_button(VK_RETURN, 100, 100);
            Read_memory::instance()->reach_data("==", 455);
            Controler::press_button(VK_RETURN, 100, 250);
            Player::instance(2)->life.address = Read_memory::instance()->return_address();
            if(Player::instance(2)->life.address <= 0)
                Thread::instance()->write("life_2 not find", "console");

            //------------------------------------------------------------------------------------

            Player::instance(1)->life.address = Player::instance(2)->life.address + 9888; // 6656 - 3968
            if(Player::instance(1)->life.address <= 0)
                Thread::instance()->write("life not find", "console");

            //------------------------------------------------------------------------------------

            Player::instance(1)->stun.address = Player::instance(1)->life.address + 6208; // 13856
            if(Player::instance(1)->stun.address <= 0)
                Thread::instance()->write("stun not find", "console");

            //------------------------------------------------------------------------------------

            Player::instance(2)->stun.address = Player::instance(1)->stun.address + 1440; // 1120
            if(Player::instance(2)->stun.address <= 0)
                Thread::instance()->write("stun_2 not find", "console");


            //------------------------------------------------------------------------------------

            Controler::press_button(VK_RETURN, 100, 500);
            Controler::press_button(VK_B, 50, 200);
            Controler::press_button(VK_RIGHT, 2500);
            Controler::press_button(VK_B, 50, 50); // B
            Controler::press_button(VK_RETURN, 100, 250);
            Read_memory::instance()->reach_data("==", 41, 0, RESET);
            Controler::press_button(VK_RETURN, 100, 500);
            Controler::press_button(VK_K, 50, 100); // K
            Controler::press_button(VK_RETURN, 100, 250);
            Read_memory::instance()->reach_data("==", 57);
            Controler::press_button(VK_RETURN, 100, 2100);
            Read_memory::instance()->reach_data("==", 0);
            Player::instance(2)->state.address = Read_memory::instance()->return_address();
            if(Player::instance(2)->state.address <= 0)
                Thread::instance()->write("state_2 not find", "console");

            //------------------------------------------------------------------------------------

            Controler::press_button(VK_RETURN, 100, 500);
            Controler::press_button(VK_B, 50, 200);
            Read_memory::instance()->reach_data("==", 90.0f, 0.0f, RESET);
            Controler::press_button(VK_RIGHT, 1000);
            Controler::jump(FORWARD);
            Thread::instance()->sleep(500);
            Read_memory::instance()->reach_data("==", -90.0f, 0.0f);
            Player::instance(1)->orientation.address = Read_memory::instance()->return_address();
            if(Player::instance(1)->orientation.address <= 0)
                Thread::instance()->write("orientation not find", "console");

            //------------------------------------------------------------------------------------

            Controler::press_button(VK_RETURN, 100, 500); // 90 et - 90 in float
            Controler::press_button(VK_B, 50, 200);
            Read_memory::instance()->reach_data("==", 900, 0, RESET);
            Controler::tatsumaki(EX);
            Thread::instance()->sleep(1000);
            Read_memory::instance()->reach_data("==", 600);
            Controler::tatsumaki(EX);
            Thread::instance()->sleep(500);
            Read_memory::instance()->reach_data("==", 300);
            Player::instance(1)->super.address = Read_memory::instance()->return_address();
            if(Player::instance(1)->super.address <= 0)
                Thread::instance()->write("orientation not find", "console");

            //------------------------------------------------------------------------------------

            Read_memory::instance()->reach_data("==", 900, 0, RESET);
            Controler::press_button(VK_RETURN, 50, 200);
            Controler::press_button(VK_J, 50, 150);
            Controler::press_button(VK_RIGHT, 50, 200);
            Controler::press_button(VK_RIGHT, 50, 200);
            Controler::press_button(VK_RIGHT, 50, 200);
            Controler::press_button(VK_DOWN, 50, 200);
            Controler::press_button(VK_B, 50, 300);
            Controler::press_button(VK_DOWN, 50, 150);
            Controler::press_button(VK_DOWN, 50, 150);
            Controler::press_button(VK_B, 50, 200);
            Controler::press_2_buttons(VK_K, VK_M, 50, 200);
            Read_memory::instance()->reach_data("==", 0);
            Thread::instance()->sleep(4000);
            Read_memory::instance()->reach_data("==", 0);
            Thread::instance()->sleep(4000);
            Read_memory::instance()->reach_data("==", 0);
            Thread::instance()->sleep(4000);
            Read_memory::instance()->reach_data("==", 0);
            Thread::instance()->sleep(4000);
            Read_memory::instance()->reach_data("==", 0);
            Thread::instance()->sleep(8000);
            Controler::press_button(VK_RETURN, 100, 300);
            Controler::press_button(VK_N, 50, 250);
            Controler::press_button(VK_N, 50, 300);
            Read_memory::instance()->reach_data("==", 900);

            Player::instance(2)->v_trigger.address = Read_memory::instance()->return_address();
            if(Player::instance(2)->v_trigger.address <= 0)
                Thread::instance()->write("v_trigger not find", "console");


            //------------------------------------------------------------------------------------

            Player::instance(2)->duration.address = Player::instance(2)->v_trigger.address + 4;
            if(Player::instance(2)->duration.address <= 4)
                Thread::instance()->write("duration not find", "console");

            //------------------------------------------------------------------------------------

            Player::instance(2)->super.address = Player::instance(2)->v_trigger.address - 8;
            if(Player::instance(2)->super.address <= 0)
                Thread::instance()->write("super_2 not find", "console");

            //------------------------------------------------------------------------------------

            Player::instance(1)->v_trigger.address = Player::instance(1)->super.address + 8;
            if(Player::instance(1)->v_trigger.address <= 8)
                Thread::instance()->write("v_trigger not find", "console");

            //------------------------------------------------------------------------------------

            Player::instance(1)->duration.address = Player::instance(1)->v_trigger.address + 4;
            if(Player::instance(1)->duration.address <= 4)
                Thread::instance()->write("duration not find", "console");*/

            //------------------------------------------------------------------------------------

            Controler::press_button(VK_RETURN, 100, 500);
            Controler::press_button(VK_K, 50, 200);
            Controler::press_button(VK_B, 50, 200);

            Controler::press_button(VK_LEFT, 1000);
            Controler::press_button(VK_RETURN, 100, 500);
            Read_memory::instance()->reach_data("<>", 35, 45, RESET);
            Controler::press_button(VK_RETURN, 100, 500);

            Controler::press_button(VK_LEFT, 2000);
            Controler::press_button(VK_RETURN, 100, 500);
            Read_memory::instance()->reach_data("<>", 112, 122);
            Controler::press_button(VK_RETURN, 100, 500);

            Controler::press_button(VK_LEFT, 3000);
            Controler::press_button(VK_RETURN, 100, 500);
            Read_memory::instance()->reach_data("<>", 150, 170);
            Controler::press_button(VK_RETURN, 100, 500);

            Controler::press_button(VK_LEFT, 2000);
            Controler::jump();
            Thread::instance()->sleep(100);
            Controler::press_button(VK_RETURN, 100, 500);
            Read_memory::instance()->reach_data("<>", 1, 50);
            Controler::press_button(VK_RETURN, 100, 500);


            Player::instance(1)->time_state.address = Read_memory::instance()->return_address();
            if(Player::instance(1)->time_state.address <= 0)
                Thread::instance()->write("time_state not find", "console");

            Player::instance(2)->time_state.address = Player::instance(1)->time_state.address - 800;
            if(Player::instance(2)->time_state.address <= 0)
                Thread::instance()->write("time_state not find", "console");

            //------------------------------------------------------------------------------------

            Controler::press_button(VK_RETURN, 50, 500);
            Controler::press_button(VK_B, 50, 100);
            Controler::press_button(VK_RIGHT, 2100);
            Controler::press_button(VK_K, 50, 300);
            Controler::press_button(VK_RETURN, 50, 500);
            Read_memory::instance()->reach_data("<>", 5, 20, RESET);
            Controler::press_button(VK_RETURN, 50, 500);

            Thread::instance()->sleep(1000);
            Controler::press_button(VK_RETURN, 100, 500);
            Read_memory::instance()->reach_data("<>", 10, 30);
            Controler::press_button(VK_RETURN, 100, 500);

            Thread::instance()->sleep(1000);
            Controler::press_button(VK_RETURN, 100, 500);
            Read_memory::instance()->reach_data("<>", 100, 125);
            Controler::press_button(VK_RETURN, 100, 500);

            Thread::instance()->sleep(1000);
            Controler::press_button(VK_RETURN, 100, 500);
            Read_memory::instance()->reach_data("<>", 200, 225);
            Controler::press_button(VK_RETURN, 100, 500);

            Player::instance(2)->time_state.address = Read_memory::instance()->return_address();
            if(Player::instance(2)->time_state.address <= 0)
                Thread::instance()->write("time_state_2 not find", "console");

            //------------------------------------------------------------------------------------

            /*Thread::instance()->sleep(1000);
            Controler::press_button(VK_RETURN, 50, 200);
            Controler::press_button(VK_K, 50, 200);
            Controler::press_button(VK_LEFT, 50, 200);
            Controler::press_button(VK_LEFT, 50, 90);
            Controler::press_button(VK_DOWN, 50, 90);
            Controler::press_button(VK_LEFT, 50, 90);
            Controler::press_button(VK_DOWN, 50, 90);
            Controler::press_button(VK_LEFT, 50, 90);
            Controler::press_button(VK_LEFT, 50, 90);
            Controler::press_button(VK_LEFT, 50, 90);
            Controler::press_button(VK_DOWN, 50, 90);
            Controler::press_button(VK_DOWN, 50, 90);
            Controler::press_button(VK_LEFT, 50, 90);
            Controler::press_button(VK_LEFT, 50, 90);
            Controler::press_button(VK_DOWN, 50, 90);
            Controler::press_button(VK_LEFT, 50, 90);
            Controler::press_button(VK_LEFT, 50, 90);
            Controler::press_button(VK_LEFT, 50, 90);
            Controler::press_button(VK_DOWN, 50, 90);
            Controler::press_button(VK_LEFT, 50, 90);
            Controler::press_button(VK_LEFT, 50, 90);
            Controler::press_button(VK_LEFT, 50, 90);
            Controler::press_button(VK_RETURN, 100, 500);*/

            //------------------------------------------------------------------------------------
            //------------------------------------------------------------------------------------

        }
    }
    else if(version == 4)
    {
        //------------------------------------------------------------------------------------
        //                                      PLAYER 1
        //------------------------------------------------------------------------------------
        Read_memory::instance();
        if(Read_memory::instance()->ready)
        {
            Read_memory::instance()->reach_data("==", -1.5f);
            Controler::press_button(VK_LEFT, 7000);
            Read_memory::instance()->reach_data("==", -2.5f);
            Player::instance(1)->distance.address = Read_memory::instance()->return_address();
            if(Player::instance(1)->distance.address == 0)
                Thread::instance()->write("distance not find", "console");

            //------------------------------------------------------------------------------------

            Read_memory::instance()->reach_data("<>", -3.51f, -3.49f, RESET);

            Thread::instance()->sleep(250);
            Controler::press_button(VK_RETURN, 100, 500);
            Controler::press_button(VK_DOWN, 100, 500);
            Controler::press_button(VK_NUMPAD1, 100, 500);
            Controler::press_button(VK_NUMPAD1, 100, 500);

            Read_memory::instance()->reach_data("==", -1.5f);
            Player::instance(1)->position_x.address = Read_memory::instance()->return_address();
            if(Player::instance(1)->position_x.address == 0)
                Thread::instance()->write("position x not find", "console");

            //------------------------------------------------------------------------------------

            Read_memory::instance()->reach_data("==", 0, 0, RESET);
            Controler::down_button(VK_LEFT);
            Read_memory::instance()->reach_data("==", 17);
            Controler::up_button(VK_LEFT);
            Controler::down_button(VK_RIGHT);
            Read_memory::instance()->reach_data("==", 16);
            Controler::up_button(VK_RIGHT);
            Player::instance(1)->state.address = Read_memory::instance()->return_address();
            if(Player::instance(1)->state.address == 0)
                Thread::instance()->write("state not find", "console");

            //------------------------------------------------------------------------------------

            Read_memory::instance()->reach_data("==", 1.0f, 0.0f, RESET);
            Controler::hadoken(EX);
            Read_memory::instance()->reach_data("==", 0.75f);
            Controler::hadoken(EX);
            Read_memory::instance()->reach_data("==", 0.50f);
            Controler::hadoken(EX);
            Read_memory::instance()->reach_data("==", 0.25f);
            Controler::hadoken(EX);
            Read_memory::instance()->reach_data("==", 0.0f);
            Player::instance(1)->super.address = Read_memory::instance()->return_address();
            if(Player::instance(1)->super.address == 0)
                Thread::instance()->write("super not find", "console");

            //------------------------------------------------------------------------------------

            Controler::press_button(VK_RIGHT, 500, 500);
            Read_memory::instance()->reach_data("==", 1.0f, 0.0f, RESET);
            Controler::press_button(VK_NUMPAD1, 50, 800);
            Controler::press_button(VK_RETURN, 100, 500);
            Read_memory::instance()->reach_data("<>", 0.95f, 0.98f);
            Controler::press_button(VK_RETURN, 100, 500);
            Thread::instance()->sleep(2500);
            Read_memory::instance()->reach_data("==", 1.0f);
            Controler::press_button(VK_NUMPAD5, 50, 800);
            Controler::press_button(VK_RETURN, 100, 500);
            Read_memory::instance()->reach_data("<>", 0.91f, 0.94f);
            Controler::press_button(VK_RETURN, 100, 500);
            Player::instance(2)->life.address = Read_memory::instance()->return_address();
            if(Player::instance(2)->life.address == 0)
                Thread::instance()->write("life not find", "console");

            //------------------------------------------------------------------------------------
            //------------------------------------------------------------------------------------

            Thread::instance()->write("Data find !", "console");
        }
    }

    Read_memory::instance()->clear();
    chrono = time(NULL) - chrono;
    int t = (int)chrono;
    Thread::instance()->write((string)"Data find in " + std::to_string(t) + "s !", "console");
    Thread::instance()->sleep(3500);
    //MainWindow::instance()->data_found = true;
    //Player::refresh_values(false);
    //read_values();
}

void Data_recovery::save_addresses()
{
    QSettings settings(SAVE_FILE, QSettings::IniFormat);
    settings.beginGroup("Address");
    settings.remove("");
    settings.endGroup();
    settings.beginGroup("Address");
    for(int h = 1; h <= 2; h++)
    {
        for(int i = 0; i < Player::number_float; i++)
        {
            if(Player::instance(h)->data_float[i]->address != 0)
            {
                settings.setValue(QString::fromStdString(Player::instance(h)->data_float[i]->name), Player::instance(h)->data_float[i]->address);
            }
        }
        for(int i = 0; i < Player::number_int; i++)
        {
            if(Player::instance(h)->data_int[i]->address != 0)
            {
                settings.setValue(QString::fromStdString(Player::instance(h)->data_int[i]->name), Player::instance(h)->data_int[i]->address);
            }
        }
    }
    settings.endGroup();
}

void Data_recovery::load_addresses()
{
    QSettings settings(SAVE_FILE, QSettings::IniFormat);
    for(int h = 1; h <= 2; h++)
    {
        for(int i = 0; i < Player::number_float; i++)
        {
            if(Player::instance(h)->data_float[i]->address == 0)
            {
                Player::instance(h)->data_float[i]->address = settings.value(QString::fromStdString((string)"Address/" + Player::instance(h)->data_float[i]->name), 0).toInt();
            }
        }
        for(int i = 0; i < Player::number_int; i++)
        {
            if(Player::instance(h)->data_int[i]->address == 0)
            {
                Player::instance(h)->data_int[i]->address = settings.value(QString::fromStdString((string)"Address/" + Player::instance(h)->data_int[i]->name), 0).toInt();
            }
        }
    }
}

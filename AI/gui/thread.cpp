#include "thread.h"
Thread *s_instance = 0;

Thread* Thread::instance()
{
    if(s_instance == NULL)
        s_instance = new Thread();
    return s_instance;
}

Thread::Thread()
{
    moveToThread(this);
}

void Thread::run()
{
    while(Parameters::choice_function != START_THREAD
       && Parameters::is_close == false)
    {
        sleep(100);
    }
    if(Parameters::is_close == false)
        initialise();
    Parameters::choice_function = 0;
    while(Parameters::is_close == false)
    {
        if(Parameters::choice_function == READ_DATA && data_found && Parameters::is_display_values)
        {
            Player::refresh_values();
            Parameters::choice_function = 0;
        }
        if(Parameters::choice_function == FIND_DATA)
        {
            data.find_values(5);
            data_found = true;
            Parameters::choice_function = 0;
        }
        if(Parameters::choice_function == SAVE_DATA)
        {
            data.save_addresses();
            Parameters::choice_function = 0;
        }
        if(Parameters::choice_function == LOAD_DATA)
        {
            data.load_addresses();
            data_found = true;
            Parameters::choice_function = 0;
        }
        if(Parameters::choice_function == WRITE_DATA)
        {
            Read_memory::instance()->write_data_around_address_in_txt((string)TEMP_PATH + Parameters::name_of_file + ".txt", Parameters::address_to_write, 100);
            Parameters::choice_function = 0;
        }
        if(Parameters::choice_function == START_DATABASE)
        {
            Parameters::choice_function = 0;
        }
        if(Parameters::choice_function == DELETE_DATABASE)
        {
            Database::delete_all();
            Parameters::choice_function = 0;
        }
        if(Parameters::choice_function == LEARNING)
        {
            Algorithm::mode(true);
            Parameters::choice_function = 0;
        }
        if(Parameters::choice_function == FIGHT)
        {
            Algorithm::mode(false);
            Parameters::choice_function = 0;
        }
        if(Parameters::choice_function == TEST)
        {
            Parameters::choice_function = 0;
        }
        if(Parameters::choice_function == TRAIN_NN) // checkable button
        {
            Neural_network_manager::train(Parameters::id_nn);
            refresh_display();
            Parameters::choice_function = 0;
        }
        if(Parameters::choice_function == TRAIN_ALL_NN) // checkable button
        {
            Neural_network_manager::instance()->train_all();
            refresh_display();
            Parameters::choice_function = 0;
        }
        if(Parameters::choice_function == ADD_NN)
        {
            Neural_network_manager::add();
            refresh_display();
            Parameters::choice_function = 0;
        }
        if(Parameters::choice_function == DELETE_NN)        
        {
            Neural_network_manager::remove(Parameters::id_nn);
            refresh_display();
            Parameters::choice_function = 0;
        }
        if(Parameters::choice_function == TEST_NN)
        {
            Neural_network_manager::instance()->test();
            refresh_display();
            Parameters::choice_function = 0;
        }
        if(Parameters::choice_function == CLEAN_NN)
        {
            Neural_network_manager::instance()->clean();
            refresh_display();
            Parameters::choice_function = 0;
        }
        sleep(10);

    }
    Parameters::choice_function = IS_CLOSED;
}

void Thread::initialise()
{
    srand(time(NULL));
    rand();
    Read_memory::instance();
    Neural_network_manager::instance()->load();
    refresh_display();
    write("Start first thead", "console");
}

void Thread::refresh_display()
{
   if(Parameters::is_refreshing_neural_network_display == false)
   {
        emit message_refresh_display();
   }
}

void Thread::replot()
{
     emit message_replot();
}

void Thread::write(string text, string name_label)
{
   emit message_write(QString::fromStdString(text), QString::fromStdString(name_label));
}

void Thread::write_nn(string text, int id_combo)
{
   emit message_write_nn(QString::fromStdString(text), id_combo);
}

string Thread::read(string name)
{
    return "StreetFighterV";//Parameters::read_window;
}

inline void Thread::sleep(int time)
{
    dieTime = QTime::currentTime().addMSecs(time);
    while (QTime::currentTime() < dieTime)
    {
        QCoreApplication::processEvents();
    }
}

void Thread::toto2()
{
    cout << "ok ok ok ok" << endl;
}

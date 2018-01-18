#include "ui_mainwindow.h"
#include "mainwindow.h"


string Parameters::read_window = "";
int Parameters::id_nn = 0;
int Parameters::choice_function = 0;
bool Parameters::is_close = false;
bool Parameters::display_update = true;
bool Parameters::is_refreshing_neural_network_display = false;
bool Parameters::is_single_nn;
bool Parameters::is_score;
int Parameters::number_single_nn = 0;
bool Parameters::shuffle_database;
int64_t Parameters::address_to_write = 0;
string Parameters::name_of_file;
int Parameters::number_of_thread = 1;
bool Parameters::is_display_values = false;

MainWindow *p_instance = 0;

MainWindow* MainWindow::instance()
{
    if(p_instance == NULL)
        p_instance = new MainWindow();
    return p_instance;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if(Parameters::choice_function == 0
    || Parameters::choice_function == READ_DATA
    || Parameters::choice_function == FIND_DATA)
    {
        event->accept();
        this->~MainWindow();
        qDebug() << "closeEvent execute";
        system("taskkill /F /IM AI.exe");
    }
    else
        event->ignore();
}

void MainWindow::sleep(int time)
{
    /*dieTime = QTime::currentTime().addMSecs(time);
    while (QTime::currentTime() < dieTime)
    {

        QApplication::processEvents(); // (QEventLoop::AllEvents, 100)
    }*/
    QEventLoop loop; // IT IS BEST METHOD
    QTimer::singleShot(time, &loop, SLOT(quit()));
    loop.exec();
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->tabWidget->setCurrentIndex(TAB_START);
    ui->tabWidget_nn->setCurrentIndex(0);
    srand(time(NULL));
    rand();
    connect(ui->button_start,               SIGNAL(clicked()),           this, SLOT(button_start()));
    connect(ui->button_address,             SIGNAL(clicked()),           this, SLOT(button_address()));
    connect(ui->button_save,                SIGNAL(clicked()),           this, SLOT(button_save()));
    connect(ui->button_load,                SIGNAL(clicked()),           this, SLOT(button_load()));
    connect(ui->button_command_test,        SIGNAL(clicked()),           this, SLOT(button_command_test()));
    connect(ui->button_combo_test,          SIGNAL(clicked()),           this, SLOT(button_combo_test()));
    connect(ui->button_test,                SIGNAL(clicked()),           this, SLOT(button_test()));
    connect(ui->button_test_ram,            SIGNAL(clicked()),           this, SLOT(button_test_ram()));
    connect(ui->button_fight,               SIGNAL(clicked()),           this, SLOT(button_fight()));
    connect(ui->button_delete_database,     SIGNAL(clicked()),           this, SLOT(button_delete_database()));
    connect(ui->button_start_nn,            SIGNAL(clicked()),           this, SLOT(button_start_nn()));
    connect(ui->button_test_nn,             SIGNAL(clicked()),           this, SLOT(button_test_nn()));
    connect(ui->button_clean_nn,            SIGNAL(clicked()),           this, SLOT(button_clean_nn()));
    connect(ui->button_train_all_nn,        SIGNAL(clicked()),           this, SLOT(button_train_all_nn()));
    connect(ui->checkbox_display_update_nn, SIGNAL(clicked()),           this, SLOT(checkbox_display_update_nn()));
    connect(ui->checkbox_shuffle_database,  SIGNAL(clicked()),           this, SLOT(checkbox_shuffle_database()));
    connect(ui->button_single_nn,           SIGNAL(clicked()),           this, SLOT(button_single_nn()));
    connect(ui->tabWidget_nn,               SIGNAL(currentChanged(int)), this, SLOT(graph_nn()));
    connect(ui->button_save_txt,            SIGNAL(clicked()),           this, SLOT(button_save_txt()));
    connect(ui->button_convert,             SIGNAL(clicked()),           this, SLOT(button_convert()));
    connect(ui->button_number_single_nn,    SIGNAL(activated(int)),      this, SLOT(button_number_single_nn()));
    connect(ui->button_score,               SIGNAL(clicked()),           this, SLOT(button_score()));
    connect(ui->button_thread_nn,           SIGNAL(valueChanged(int)),   this, SLOT(button_thread_nn()));

    button_single_nn();

    ui->window_name->addItem("StreetFighterV");
    ui->window_name->addItem("SSFIVAE");
    ui->window_name->addItem("H:\\test.exe");

    ui->command_name->addItem("Shifting");
    ui->command_name->addItem("Jump");
    ui->command_name->addItem("Hadoken");
    ui->command_name->addItem("Throw");
    ui->command_name->addItem("Combo : LMK + Hadoken");
    ui->command_name->addItem("Combo : MP + HP + HK");
    ui->command_name->addItem("Combo : LP + LP");
    ui->command_name->addItem("Advance");
    ui->command_name->addItem("Back");
    ui->command_name->addItem("Dash");
    ui->command_name->addItem("Backdash");
    ui->command_name->addItem("L shoryuken");
    ui->command_name->addItem("M shoryuken");
    ui->command_name->addItem("H shoryuken");
    ui->command_name->addItem("EX shoryuken");
    ui->command_name->addItem("L tatsumaki");
    ui->command_name->addItem("Axe kick");
    ui->command_name->addItem("HP");
    ui->command_name->addItem("Solar plexus");
    ui->command_name->addItem("shoulder throw");
    ui->command_name->addItem("somersault throw");

    ui->scrollAreaWidgetContents->setLayout(ui->gridLayout);
    ui->scrollArea->verticalScrollBar()->setPageStep(28);
    ui->scrollArea->verticalScrollBar()->setSingleStep(28);

    Parameters::shuffle_database = ui->checkbox_shuffle_database->isChecked();

    text_console = "";
    text_information = "";
    flag_address = true;
    data_found = false;
    Parameters::is_score = false;

    x_50.push_back(0);
    x_50.push_back(100000);
    y_50.push_back(50);
    y_50.push_back(50);

    QTimer::singleShot(1, this, SLOT(initialaze()));
    Thread::instance();
    connect(Thread::instance(), SIGNAL(message_refresh_display()),       this, SLOT(refresh_neural_networks_buttons()));
    connect(Thread::instance(), SIGNAL(message_write(QString, QString)), this, SLOT(write2(QString, QString)));
    connect(Thread::instance(), SIGNAL(message_write_nn(QString, int)),  this, SLOT(write_nn(QString, int)));
    connect(Thread::instance(), SIGNAL(message_replot()),                 this, SLOT(graph_nn()));
    ///connect(thread, SIGNAL(message_read(string)), this, SLOT(read(string));
    //QTimer::singleShot(1, this, SLOT(start_thread()));
    start_thread();

    QTimer::singleShot(900, this, SLOT(refresh()));
}

void MainWindow::start_thread()
{
        Thread::instance()->start(QThread::NormalPriority);
}

void MainWindow::initialaze()
{
    Database_manager::initialize();
    neural_networks_buttons();
    Parameters::choice_function = START_THREAD;
}

void MainWindow::neural_networks_buttons() // TO MODIFY : before instance on database
{
    for(unsigned int i = 0; i < Neural_network_manager::get_number_of_neural_network(); i++)
    {
        const Neural_network *temp_nn = Neural_network_manager::get_neural_network(i);
        neural_network_buttons nnb;
        list_nn_buttons.push_back(nnb);
        const string name = to_string(temp_nn->id) + ") " + "neural network";
        list_nn_buttons.back().combo_name_nn = new QLabel(name.c_str());
        list_nn_buttons.back().id = temp_nn->id;
        list_nn_buttons.back().clustering_rate_nn = new QLabel(to_string(temp_nn->clustering_rate) + "%");
        list_nn_buttons.back().train_nn = new QPushButton("Train");
        list_nn_buttons.back().train_nn->setCheckable(true);
        list_nn_buttons.back().add_nn = new QPushButton("Add");
        list_nn_buttons.back().delete_nn = new QPushButton("Delete");
        list_nn_buttons.back().details_nn = new QPushButton("Details");

        list_nn_buttons.back().train_nn->setStyleSheet("max-width: 50px");
        list_nn_buttons.back().add_nn->setStyleSheet("max-width: 50px");
        list_nn_buttons.back().delete_nn->setStyleSheet("max-width: 50px");
        list_nn_buttons.back().details_nn->setStyleSheet("max-width: 50px");

        QSignalMapper* signal_mapper1 = new QSignalMapper(this);
        QSignalMapper* signal_mapper2 = new QSignalMapper(this);
        QSignalMapper* signal_mapper3 = new QSignalMapper(this);
        QSignalMapper* signal_mapper4 = new QSignalMapper(this);

        signal_mapper1->setMapping(list_nn_buttons.back().add_nn, list_nn_buttons.back().id);
        signal_mapper3->setMapping(list_nn_buttons.back().train_nn, list_nn_buttons.back().id);
        signal_mapper2->setMapping(list_nn_buttons.back().delete_nn, list_nn_buttons.back().id);
        signal_mapper4->setMapping(list_nn_buttons.back().details_nn, list_nn_buttons.back().id);

        connect(list_nn_buttons.back().add_nn,    SIGNAL(clicked()), signal_mapper1, SLOT(map()));
        connect(list_nn_buttons.back().train_nn, SIGNAL(clicked()), signal_mapper3, SLOT(map()));
        connect(list_nn_buttons.back().delete_nn, SIGNAL(clicked()), signal_mapper2, SLOT(map()));
        connect(list_nn_buttons.back().details_nn, SIGNAL(clicked()), signal_mapper4, SLOT(map()));

        connect(signal_mapper1, SIGNAL(mapped(QString)),  this, SLOT(button_add_nn(int)));
        connect(signal_mapper3, SIGNAL(mapped(QString)), this, SLOT(button_train_nn(int)));
        connect(signal_mapper2, SIGNAL(mapped(QString)), this, SLOT(button_delete_nn(int)));
        connect(signal_mapper4, SIGNAL(mapped(QString)), this, SLOT(button_details_nn(int)));

        ui->gridLayout->addWidget(list_nn_buttons.back().combo_name_nn, i, 0);
        ui->gridLayout->addWidget(list_nn_buttons.back().clustering_rate_nn, i, 1);
        ui->gridLayout->addWidget(list_nn_buttons.back().train_nn, i, 2);
        ui->gridLayout->addWidget(list_nn_buttons.back().add_nn, i, 3);
        ui->gridLayout->addWidget(list_nn_buttons.back().delete_nn, i, 4);
        ui->gridLayout->addWidget(list_nn_buttons.back().details_nn, i, 5);
    }
}

void MainWindow::refresh()
{
    Parameters::number_single_nn = ui->button_number_single_nn->currentText().toInt();
    while(true)
    {
        if(ui->tabWidget->currentIndex() == 1
        || ui->tabWidget->currentIndex() == 3)
        {
            Parameters::is_display_values = true;
            refresh_memory_display();
        }
        else
            Parameters::is_display_values = false;
        sleep(10);
    }
}

inline void MainWindow::refresh_memory_display()
{
    for(int h = 1; h <= 2; h++)
    {
        for(int i = 0; i < Player::number_float; i++)
        {
            if(flag_address == true)
                write((string)"" + to_string(Player::instance(h)->data_float[i]->value), Player::instance(h)->data_float[i]->name);
            else
                if(Player::instance(h)->data_float[i]->address > 0)
                    write((string)"" + QString::number(Player::instance(h)->data_float[i]->address, 16).toStdString(), Player::instance(h)->data_float[i]->name);
                else
                    write("unknown value", Player::instance(h)->data_float[i]->name);
        }
        for(int i = 0; i < Player::number_int; i++)
        {
            if(flag_address == true)
                write((string)"" + to_string(Player::instance(h)->data_int[i]->value), Player::instance(h)->data_int[i]->name);
            else
                if(Player::instance(h)->data_int[i]->address > 0)
                    write((string)"" + QString::number(Player::instance(h)->data_int[i]->address, 16).toStdString(), Player::instance(h)->data_int[i]->name);
                else
                    write("unknown value", Player::instance(h)->data_int[i]->name);
        }
    }
}

void MainWindow::refresh_neural_networks_buttons() // TO DELETE ?
{
    /*Parameters::is_refreshing_neural_network_display = true;
    Database::instance(0)->read("SELECT COUNT(*) FROM states");
    ui->number_data->setText(Database::instance(0)->result_request[0].c_str());
    Database::instance(0)->read("SELECT COUNT(*) FROM perceptrons");
    ui->number_perceptron->setText(Database::instance(0)->result_request[0].c_str());

    Database::instance(0)->read("SELECT combo FROM combos ORDER BY id ASC");
    unsigned int size = (unsigned int)Database::instance(0)->result_request.size();
    int current_index = -1;
    int c;
    ui->button_number_single_nn->clear();
    for(unsigned int i = 0; i < size; i++)
    {
        current_index = list_nn_buttons[i].choice_nn->currentIndex();
        if(current_index == -1)
            current_index = 0;
        list_nn_buttons[i].choice_nn->clear();
        Database::instance(0)->read("SELECT id, clustering_rate FROM neural_networks WHERE id_combo = " + to_string(i) + " ORDER BY id ASC");
        c = 0;
        for(unsigned int j = 0; j < Database::instance(0)->result_request.size(); j += 2)
        {
            list_nn_buttons[i].choice_nn->addItem(Database::instance(0)->result_request[j].c_str());
            if(i == 0)
            {
                ui->button_number_single_nn->addItem(Database::instance(0)->result_request[j].c_str());
            }
            if(current_index == c)
            {
                list_nn_buttons[i].clustering_rate_nn->setText(QString::fromStdString(Database::instance(0)->result_request[j+1] + "%"));
            }
            c++;
        }
        if(current_index == -1)
            current_index = 0;
        list_nn_buttons[i].choice_nn->setCurrentIndex(current_index);
        if(list_nn_buttons[i].choice_nn->count() == 0)
        {
            list_nn_buttons[i].clustering_rate_nn->setText("");
        }
    }
    Parameters::is_refreshing_neural_network_display = false;*/
}

MainWindow::~MainWindow()
{
    qDebug() << "destructor execute";
    //system("taskkill /IM test.exe");
}

void MainWindow::write(string text, string name_label)
{
    if(name_label == "console")
    {
        if(text != "CLEAR")
        {
            text_console += text + "\n";
            ui->console->setText(QString::fromStdString(text_console));
            ui->console->verticalScrollBar()->setValue(ui->console->verticalScrollBar()->maximum());
        }
        else
        {
            ui->console->setText("");
            text_console = "";
        }
    }
    else if(name_label == "current_console")
    {
            ui->current_console->setText(QString::fromStdString(text));
    }
    else if(name_label == "information")
    {
        if(text != "CLEAR")
        {
            text_information += text + "\n";
            ui->information->setText(QString::fromStdString(text_information));
            ui->information->verticalScrollBar()->setValue(ui->information->verticalScrollBar()->maximum());
        }
        else
        {
            ui->information->setText("");
            text_information = "";
        }
    }
    else if(name_label == "current_information")
    {
            ui->current_information->setText(QString::fromStdString(text));
    }
    else if(name_label == "nn_information")
    {
        if(text != "CLEAR")
        {
            text_nn_information += text + "\n";
            ui->nn_information->setText(QString::fromStdString(text_nn_information));
            ui->nn_information->verticalScrollBar()->setValue(ui->nn_information->verticalScrollBar()->maximum());
        }
        else
        {
            ui->nn_information->setText("");
            text_nn_information = "";
        }
    }
    else if(name_label == "current_nn_information")
    {
        ui->current_console_nn->setText(QString::fromStdString(text));
    }
    else if(name_label == "distance")
    {
        ui->distance->setText(QString::fromStdString(text));
    }
    else if(name_label == "position_x")
    {
        ui->position_x->setText(QString::fromStdString(text));
    }
    else if(name_label == "position_y")
    {
        ui->position_y->setText(QString::fromStdString(text));
    }
    else if(name_label == "state")
    {
        ui->state->setText(QString::fromStdString(text));
    }
    else if(name_label == "super")
    {
        ui->super_bar->setText(QString::fromStdString(text));
    }
    else if(name_label == "life")
    {
        ui->life->setText(QString::fromStdString(text));
    }
    else if(name_label == "orientation")
    {
        ui->orientation->setText(QString::fromStdString(text));
    }
    else if(name_label == "stun")
    {
        ui->stun->setText(QString::fromStdString(text));
    }
    else if(name_label == "v_trigger")
    {
        ui->v_trigger->setText(QString::fromStdString(text));
    }
    else if(name_label == "duration")
    {
        ui->duration->setText(QString::fromStdString(text));
    }
    else if(name_label == "time_state")
    {
        ui->Time_state->setText(QString::fromStdString(text));
    }
    else if(name_label == "attack_counter")
    {
        ui->Attack_counter->setText(QString::fromStdString(text));
    }
    else if(name_label == "distance_2")
    {
        ui->distance_2->setText(QString::fromStdString(text));
    }
    else if(name_label == "position_x_2")
    {
        ui->position_x_2->setText(QString::fromStdString(text));
    }
    else if(name_label == "position_y_2")
    {
        ui->position_y_2->setText(QString::fromStdString(text));
    }
    else if(name_label == "state_2")
    {
        ui->state_2->setText(QString::fromStdString(text));
    }
    else if(name_label == "super_2")
    {
        ui->super_bar_2->setText(QString::fromStdString(text));
    }
    else if(name_label == "life_2")
    {
        ui->life_2->setText(QString::fromStdString(text));
    }
    else if(name_label == "orientation_2")
    {
        ui->orientation_2->setText(QString::fromStdString(text));
    }
    else if(name_label == "stun_2")
    {
        ui->stun_2->setText(QString::fromStdString(text));
    }
    else if(name_label == "v_trigger_2")
    {
        ui->v_trigger_2->setText(QString::fromStdString(text));
    }
    else if(name_label == "duration_2")
    {
        ui->duration_2->setText(QString::fromStdString(text));
    }
    else if(name_label == "time_state_2")
    {
        ui->Time_state_2->setText(QString::fromStdString(text));
    }
    else if(name_label == "attack_counter_2")
    {
        //ui->Attack_counter_2->setText(QString::fromStdString(text));
    }
    else if(name_label == "nn_name")
    {
        //ui->nn_name->setText(QString::fromStdString(text));
    }
    else if(name_label == "nn_value")
    {
        //ui->nn_value->setText(QString::fromStdString(text));
    }
    else
    {
        text_console += "Wrong label : " + name_label + "\n";
        ui->console->setText(QString::fromStdString(text_console));
    }
    this->sleep();
}

void MainWindow::write_nn(QString text, int id)
{
    list_nn_buttons[id].clustering_rate_nn->setText(text);
}

void MainWindow::write2(QString text, QString name_label)
{
    write(text.toStdString(), name_label.toStdString());
}

string MainWindow::read(string name)
{
    if(name == "window_name")
        return ui->window_name->currentText().toStdString();
    else
        return NULL;
}

void MainWindow::button_start()
{
    if(ui->window_name->currentText() == "StreetFighterV")
    {
        Parameters::choice_function = FIND_DATA;
        ui->button_save->setEnabled(true);
        ui->button_fight->setEnabled(true);
        ui->button_start->setEnabled(false);
    }
}

void MainWindow::button_address()
{
    if(flag_address == true)
    {
        flag_address = false;
        ui->button_address->setText("Address");
    }
    else
    {
        flag_address = true;
        ui->button_address->setText("Value");
    }

}

void MainWindow::button_delete_database()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "WARNING", "Do you want delete ALL database?", QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes)
    {
        Parameters::choice_function = DELETE_DATABASE;
    }
}

void MainWindow::button_test()
{
    float r = ( rand()/(float)RAND_MAX ) * 2 - 1;
    ui->test_result->setText( QString::fromStdString(to_string(r)));
}

void MainWindow::button_test_ram()
{
    for(int i = 0; i < 1024 * 1024;i++)
    {
        list_ram.push_back(50);
    }
}

void MainWindow::button_save()
{
    Parameters::choice_function = SAVE_DATA;
    ui->button_save->setEnabled(false);
}

void MainWindow::button_load()
{
    Parameters::choice_function = LOAD_DATA;
    ui->button_load->setEnabled(false);
    ui->button_save->setEnabled(false);
    ui->button_fight->setEnabled(true);
    ui->button_start->setEnabled(false);
    data_found = true;
}

void MainWindow::button_command_test()
{
    sleep(1500);
    if(ui->command_name->currentText() == "Shifting")
    {
        Controler::advance(800);
        sleep(250);
        Controler::back(800);
        sleep(250);
        Controler::crouch(800);
        sleep(250);
        Controler::crouching_block(800);
        sleep(300);
        Controler::dash(FORWARD);
        sleep(300);
        Controler::dash(BACK);


    }
    else if(ui->command_name->currentText() == "Jump")
    {
            Controler::jump();
            sleep(1000);
            Controler::jump(FORWARD);
            sleep(1000);
            Controler::jump(BACK);
    }
    else if(ui->command_name->currentText() == "Hadoken")
    {
        Controler::hadoken(LIGHT);
        sleep(1000);
        Controler::hadoken(MEDIUM);
        sleep(1000);
        Controler::hadoken(HEAVY);
        sleep(1000);
        Controler::hadoken(EX);
        sleep(2100);
        Controler::shinku_hadoken(LIGHT);
    }
    else if(ui->command_name->currentText() == "Throw")
    {
        Controler::shoulder_throw();
        sleep(1250);
        Controler::somersault_throw();
    }
    else if(ui->command_name->currentText() == "Punch and Kick")
    {
        Controler::punch(LIGHT);
        sleep(500);
        Controler::punch(MEDIUM);
        sleep(500);
        Controler::punch(HEAVY);
        sleep(500);
        Controler::kick(LIGHT);
        sleep(500);
        Controler::kick(MEDIUM);
        sleep(500);
        Controler::kick(HEAVY);
        sleep(500);
        Controler::axe_kick();
    }
    else if(ui->command_name->currentText() == "Combo : LMK + Hadoken")
    {
       Controler::kick(MEDIUM, LOW);
       sleep(50);
       Controler::hadoken(LIGHT);
    }
    else if(ui->command_name->currentText() == "Combo : MP + HP + HK")
    {
        Controler::punch(MEDIUM);
        sleep(100);
        Controler::punch(HEAVY);
        sleep(300);
        Controler::kick(HEAVY);
    }
    else if(ui->command_name->currentText() == "Combo : LP + LP")
    {
        Controler::punch(LOW);
        sleep(200);
        Controler::punch(LOW);

    }
    else if(ui->command_name->currentText() == "Advance")
    {
        Controler::advance(500);
    }
    else if(ui->command_name->currentText() == "Back")
    {
        Controler::back(1000);

    }
    else if(ui->command_name->currentText() == "Dash")
    {
        Controler::dash(FORWARD);
    }
    else if(ui->command_name->currentText() == "Backdash")
    {
        Controler::dash(BACK);
    }
    else if(ui->command_name->currentText() == "L shoryuken")
    {
        Controler::shoryuken(LIGHT);
    }
    else if(ui->command_name->currentText() == "M shoryuken")
    {
        Controler::shoryuken(MEDIUM);
    }
    else if(ui->command_name->currentText() == "H shoryuken")
    {
        Controler::shoryuken(HEAVY);
    }
    else if(ui->command_name->currentText() == "EX shoryuken")
    {
        Controler::shoryuken(EX);
    }
    else if(ui->command_name->currentText() == "L tatsumaki")
    {
        Controler::tatsumaki(LIGHT);
    }
    else if(ui->command_name->currentText() == "Axe kick")
    {
        Controler::axe_kick();
    }
    else if(ui->command_name->currentText() == "HP")
    {
        Controler::punch(HEAVY);
    }
    else if(ui->command_name->currentText() == "Solar plexus")
    {
        Controler::solar_plexus_strike();
    }
    else if(ui->command_name->currentText() == "shoulder throw")
    {
        Controler::shoulder_throw();
    }
    else if(ui->command_name->currentText() == "somersault throw")
    {
        Controler::somersault_throw();
    }
}

void MainWindow::button_combo_test()
{
    ui->current_combo->setText(QString::fromStdString(Algorithm::choose_action(true)));
}

void MainWindow::button_fight()
{
    if(ui->button_radio_learning->isChecked())
    {
        if(Parameters::choice_function != LEARNING)
            Parameters::choice_function = LEARNING;
        else
            Parameters::choice_function = READ_DATA;
    }
    else if(ui->button_radio_fight->isChecked())
    {
        if(Parameters::choice_function != FIGHT)
            Parameters::choice_function = FIGHT;
        else
            Parameters::choice_function = READ_DATA;
    }
}

void MainWindow::button_start_nn()
{
    Parameters::choice_function = START_NN;
    ui->button_save->setEnabled(true);
    ui->button_load->setEnabled(true);
}

void MainWindow::button_train_nn(int id)
{
    Parameters::id_nn = id;
    if(list_nn_buttons[id].train_nn->isChecked())
    {

        list_nn_buttons[id].train_nn->setEnabled(false);
        Parameters::choice_function = TRAIN_NN;
    }
    else
    {
        list_nn_buttons[id].train_nn->setEnabled(true);
        Parameters::choice_function = 0;

    }
}

void MainWindow::button_add_nn(int id)
{
    Parameters::id_nn = id;
    Parameters::choice_function = ADD_NN;
}

void MainWindow::button_delete_nn(int id)
{
    Parameters::id_nn = id;
    QMessageBox::StandardButton reply;
    string text = "Do you want delete " + to_string(Parameters::id_nn) + " neural network ?";
    reply = QMessageBox::question(this, "WARNING", text.c_str(), QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes)
    {
        Parameters::choice_function = DELETE_NN;
    }
}

void MainWindow::button_details_nn(int id)
{
    nn_card = new Neural_network_card(Parameters::id_nn);
    nn_card->show();
}

void MainWindow::button_test_nn()
{
    Parameters::choice_function = TEST_NN;
}

void MainWindow::button_clean_nn()
{
    Parameters::choice_function = CLEAN_NN;
}

void MainWindow::button_train_all_nn()
{
    if(ui->button_train_all_nn->isChecked())
    {
        Parameters::choice_function = TRAIN_ALL_NN;
    }
    else
    {
        Parameters::choice_function = 0;
    }
}

void MainWindow::checkbox_display_update_nn()
{
    Parameters::display_update = ui->checkbox_display_update_nn->isChecked();
}

void MainWindow::checkbox_shuffle_database()
{
    Parameters::shuffle_database = ui->checkbox_shuffle_database->isChecked();
    //write(to_string(Parameters::shuffle_database), "nn_information");
}

void MainWindow::button_single_nn()
{
    Parameters::is_single_nn = ui->button_single_nn->isChecked();
}

void MainWindow::button_number_single_nn()
{
    Parameters::number_single_nn = ui->button_number_single_nn->currentIndex();
}

void MainWindow::graph_nn()
{
    if(ui->tabWidget_nn->currentIndex() == 1)
    {
        if(flag_graph == true)
        {
            ui->custom_plot->addGraph();
            ui->custom_plot->addGraph();
            ui->custom_plot->graph(0)->setPen(QPen(Qt::blue)); // line color blue for first graph
            ui->custom_plot->graph(0)->setBrush(QBrush(QColor(0, 0, 255, 20))); // first graph will be filled with translucent blue
            ui->custom_plot->graph(1)->setPen(QPen(Qt::red));
            ui->custom_plot->xAxis2->setVisible(true);
            ui->custom_plot->xAxis2->setTickLabels(false);
            ui->custom_plot->yAxis2->setVisible(true);
            ui->custom_plot->yAxis2->setTickLabels(false);
            connect(ui->custom_plot->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->custom_plot->xAxis2, SLOT(setRange(QCPRange)));
            connect(ui->custom_plot->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->custom_plot->yAxis2, SLOT(setRange(QCPRange)));
            ui->custom_plot->yAxis->setRange(0, 100); // (0, 100)
            flag_graph = false;
        }
        // make left and bottom axes always transfer their ranges to right and top axes:
        x.clear();
        for(double i = 0; i < Neural_network_manager::learning_rate.size(); i = i+1)
        {
          x.push_back(i);
        }
        ui->custom_plot->graph(0)->setData(x, Neural_network_manager::learning_rate);
        ui->custom_plot->graph(1)->setData(x_50, y_50);

        //ui->custom_plot->yAxis->setLabel(QString::fromUtf8("learning rate"));
        // ui->custom_plot->graph(0)->rescaleAxes();

        ui->custom_plot->xAxis->setRange(0, Neural_network_manager::learning_rate.size()-1);
        //ui->custom_plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
        ui->custom_plot->replot();
        // Allow user to drag axis ranges with mouse, zoom with mouse wheel and select graphs by clicking:
        //ui->custom_plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
    }
    sleep(2);
    if(Neural_network_manager::learning_rate.size() != 0)
    {

        average_clustering = 0;

        if(Neural_network_manager::learning_rate.size() <= 500)
        {
            for(int i = 0; i < Neural_network_manager::learning_rate.size(); i++)
            {
              average_clustering += Neural_network_manager::learning_rate[i];
            }
            average_clustering /= Neural_network_manager::learning_rate.size();
        }
        else
        {
            for(int i = Neural_network_manager::learning_rate.size()-500; i < Neural_network_manager::learning_rate.size(); i++)
            {
              average_clustering += Neural_network_manager::learning_rate[i];
            }
            average_clustering /= 500;
        }
        ui->average_clustering->setText(QString::fromStdString(to_string(average_clustering) + "%"));
    }

}

void MainWindow::button_save_txt()
{
    Parameters::choice_function = WRITE_DATA;
    Parameters::name_of_file = ui->name_of_file->text().toStdString();
    Parameters::address_to_write = ui->address_to_write->text().toLong(nullptr, 16);
    write((string)"\nWrite values aound address " +  QString::number(Parameters::address_to_write, 16).toStdString(), "console");
}

void MainWindow::button_convert()
{
    if(ui->number_to_convert->text().contains("."))
    {
        float value = ui->number_to_convert->text().toFloat();
        ui->number_to_convert->setText(QString::fromStdString(to_string(*(int *)&value)));
    }
    else
    {
        int value = ui->number_to_convert->text().toInt();
        ui->number_to_convert->setText(QString::fromStdString(to_string(*(float *)&value)));
    }
}

void MainWindow::button_display_score_or_clustering_rate()
{
    if(Parameters::is_score == true)
    {
        Parameters::is_score = false;
        ui->button_score->setText("%");
    }
    else
    {
        Parameters::is_score = true;
        ui->button_score->setText("Score");
    }
}

void MainWindow::button_thread_nn()
{
    Parameters::number_of_thread = ui->button_thread_nn->value();
}

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QtGui>
#include <QTimer>
#include <string>
#include <QComboBox>
#include <qscrollbar.h>
#include <QScrollArea>
#include <vector>
#include <QVector>
#include <time.h>


#include "parameters.h"
#include <environnement/database_manager.h>
#include <environnement/controler.h>
#include <ai/algorithm.h>
#include <ai/neural_network_manager.h>
#include <gui/thread.h>
#include <gui/neural_network_card.h>

using namespace std;

struct neural_network_buttons
{
    QLabel *combo_name_nn;
    QLabel *clustering_rate_nn;
    QPushButton *train_nn;
    QPushButton *add_nn;
    QPushButton *delete_nn;
    QPushButton *details_nn;
    int id;
};

namespace Ui {
class MainWindow;
}
class MainWindow : public QMainWindow
{
    Q_OBJECT

    public:

        static MainWindow* instance();
        ~MainWindow();
        void sleep(int time = 0);
        void start();
        bool data_found = false;
        static int choose_function_thread;
        void write(string text, string name_label);


    private:

        void closeEvent(QCloseEvent *event);
        explicit MainWindow(QWidget *parent = 0);
        void neural_networks_buttons();
        Ui::MainWindow *ui;
        QTime dieTime;
        QScrollArea *scroll_area;
        QWidget *client;
        string text_console;
        string text_information;
        string text_nn_information;
        bool flag_address;
        bool flag_graph = true;
        float average_clustering;
        vector<neural_network_buttons> list_nn_buttons;
        Neural_network_card *nn_card;

        void refresh_memory_display();

        list<int> list_ram;

        QVector<double> x;
        QVector<double> x_50;
        QVector<double> y_50;


    public slots:
        void write2(QString text, QString name_label);
        void write_nn(QString text, int id);
        string read(string name);

    private slots:
        void start_thread();
        void initialaze();
        void refresh();
        void button_start();
        void button_address();
        void button_test();
        void button_save();
        void button_save_txt();
        void button_load();
        void button_command_test();
        void button_combo_test();
        void button_fight();
        void button_delete_database();
        void button_start_nn();
        void button_test_nn();
        void button_clean_nn();
        void button_train_nn(int id);
        void button_add_nn(int id);
        void button_delete_nn(int id);
        void button_details_nn(int id);
        void button_train_all_nn();
        void checkbox_display_update_nn();
        void checkbox_shuffle_database();
        void refresh_neural_networks_buttons();
        void button_test_ram();
        void button_single_nn();
        void graph_nn();
        void button_convert();
        void button_number_single_nn();
        void button_display_score_or_clustering_rate();
        void button_thread_nn();
};

#endif // MAINWINDOW_H

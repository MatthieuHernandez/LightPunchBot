#ifndef READ_RAM_THREAD_H
#define READ_RAM_THREAD_H

#include "environnement/data_recovery.h"
#include "parameters.h"
#include "ai/algorithm.h"
#include "ai/neural_network_manager.h"
#include "environnement/controler.h"
#include "environnement/player.h"

#include "mainwindow.h"

#include <QThread>
#include <QTime>
#include <stdio.h>
#include <stdlib.h>
#include <string>

using namespace std;

class Thread : public QThread
{
    Q_OBJECT
    private:
        void run();
        Data_recovery data;
        bool data_found = false;



    public:
        Thread();
        static Thread* instance();
        void refresh_display();
        void write(string text, string name_label);
        void write_nn(string text, int id_combo);
        void sleep(int time = 0);
        void replot();
        string read(string name);

        QTime dieTime;

    private:
      void initialise();

    signals:
        void message_refresh_display();
        void message_write(const QString &text, const QString &name_label);
        void message_write_nn(const QString &text, int id_combo);
        void message_read(const string &result);
        void message_replot();

    private slots:
        void toto2();
};

#endif // READ_RAM_THREAD_H

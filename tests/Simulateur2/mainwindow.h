#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <windows.h>
#include <iostream>
#include <QTimer>

#include "neuron.h"

using namespace std;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

    public:
        explicit MainWindow(QWidget *parent = 0);
        ~MainWindow();
        void sleep(int time);


    private:
        Ui::MainWindow *ui;

    public slots:
        void code();
};

#endif // MAINWINDOW_H

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <time.h>
#include "neuralNetwork.h"

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

private slots:
    void on_spinBoxLabel_editingFinished();

    void on_spinBoxLabel_valueChanged(int value);

private:
    Ui::MainWindow *ui;

    void readData();
    void compute();

    vector<unsigned char> imagesTest;
    vector<unsigned char> labelsTest;

    unsigned char getImagesTest(int number, int x, int y);
};

#endif // MAINWINDOW_H

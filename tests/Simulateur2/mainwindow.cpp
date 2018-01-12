#include "mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::sleep(int time)
{
    QEventLoop loop;
    QTimer::singleShot(time, &loop, SLOT(quit()));
    loop.exec();
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QTimer::singleShot(1, this, SLOT(code()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::code()
{
    float w1[2] = {0.51, 0.51};
    float w2[2] = {0.49, 0.49};

    Neuron a(2, w1, 0.01f);
    Neuron b(2, w2, 0.01f);

    unsigned int time = 0;

    int picture[2][2] = {{217, 68},
                         {55, 189}};

    int pictureNumber = 0;

    cout << "start" << endl;

    while(true)
    {
        while(true)
        {
            if(time%picture[pictureNumber][0] == 0)
            {
                if(a.receiveSpike(0) == true)
                {
                    cout << "neurone A declanche sur picture " << pictureNumber << endl;
                    a.voltage = 0;
                    b.voltage = 0;
                    time ++;
                    break;
                }
                if(b.receiveSpike(0) == true)
                {
                    cout << "neurone B declanche sur picture " << pictureNumber << endl;
                    a.voltage = 0;
                    b.voltage = 0;
                    time ++;
                    break;
                }
            }
            if(time%picture[pictureNumber][1] == 0)
            {
                if(a.receiveSpike(1))
                {
                    cout << "neurone A declanche sur picture " << pictureNumber << endl;
                    a.voltage = 0;
                    b.voltage = 0;
                    time ++;
                    break;
                }
                if(b.receiveSpike(1))
                {
                    cout << "neurone B declanche sur picture " << pictureNumber << endl;
                    a.voltage = 0;
                    b.voltage = 0;
                    time ++;
                    break;
                }
            }
            time ++;
        }
        sleep(500);
        if(pictureNumber == 0)
            pictureNumber = 1;
        else if(pictureNumber == 1)
            pictureNumber = 0;
    }


    cout << "end" << endl;
    return;
}

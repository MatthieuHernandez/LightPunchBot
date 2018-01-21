#include "mainwindow.h"
#include "ui_mainwindow.h"


// 95.333% neuralNetwork(4, 1, 15, 3)                        YEAH !!!
// 95.333% neuralNetwork(4, 2, 20, 3)  4818  epoch           YEAH !!!
// 96.666% neuralNetwork(4, 2, 20, 3)  1334  epoch 0.01 lr   YEAH !!!
// 97.333% neuralNetwork(4, 2, 20, 3)  7126  epoch 0.01 lr   YEAH !!!
// 98.000% neuralNetwork(4, 3, 100, 3) 5834 epoch 0.01 lr    YEAH !!!

// 94.000% neuralNetwork(4, 1, 3, 3) 2021757 epoch 0.01 lr   YEAH !!! not really good

// 99.333% neuralNetwork(4, 3, 1000, 3) 11197 epoch 0.01 lr  YEAH !!!

static int random_between(int a, int b)  // WARNING TO : b excluded
{
    return rand()%(b-a)+a;
}

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
    this->intialisation();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::loadData()
{

    string line;
    ifstream file ("C:/Programming/LightPunchBot/tests/iris.txt");
    int count = 0;
    vector<vector<string>> individuals;
    vector<string> temp;
    if(!file.is_open())
    {
        cout << "Cannot open file" << endl;
    }
    else
    {
        cout << "file open" << endl;
        getline(file, line); // labels
        while(getline(file, line))
        {
            //cout << line << endl;
            individuals.push_back(temp);
            for(int i = 0; line != line.substr(line.find('\t')+1); i++)
            {
                individuals[count].push_back(line.substr(0, line.find('\t')));
                line = line.substr(line.find('\t')+1);
            }
            individuals[count].push_back(line.substr(0, line.find('\t')));
            count++;
        }
        file.close();
    }
    inputs.resize(150);
    desires.resize(150);
    for(int i = 0; i < 150; i++) // c <= 500
    {
        inputs[i].resize(4);
        desires[i].resize(3);
    }


    for(int i = 0; i < 150; i++) // c <= 500
    {
        inputs[i][0] = stof(individuals[i][0]);
        inputs[i][1] = stof(individuals[i][1]);
        inputs[i][2] = stof(individuals[i][2]);
        inputs[i][3] = stof(individuals[i][3]);

        if(individuals[i][4] == "setosa")
        {
            desires[i][0] = 1;
            desires[i][1] = 0;
            desires[i][2] = 0;
        }
        else if(individuals[i][4] == "versicolor")
        {
            desires[i][0] = 0;
            desires[i][1] = 1;
            desires[i][2] = 0;
        }
        else if(individuals[i][4] == "virginica")
        {
            desires[i][0] = 0;
            desires[i][1] = 0;
            desires[i][2] = 1;
        }
        else
        {
            cout << "wrong label" << endl;
            break;
        }
    }
    cout << "data load : " <<  individuals.size() << " individuals" << endl;
}

void MainWindow::intialisation() // less than 100 times train to obtain 90%
{
    cout << "Start" << endl;
    clock_t numberOfClockCycle;
    srand(time(NULL));
    loadData();

    NeuralNetwork neuralNetwork(4, 2, 10, 3); // 95.333% neuralNetwork(4, 1, 15, 3) // 3 * 100 for speed test
    neuralNetwork.resetCalculationOfClusteringRate();
    neuralNetwork.setLearningRate(0.01f);
    neuralNetwork.setMomentum(0.9f);

    //NeuralNetwork neuralNetwork2(1, 1, 1, 3);
    //neuralNetwork2 = neuralNetwork;
    //neuralNetwork2.setUseGPU(true);

    int numberOfEpochs = 200000;

    if(neuralNetwork.isValid() != 0
    /*&& neuralNetwork2.isValid() != 0*/)
    {
        cout << "ERROR : " << neuralNetwork.getLastError() << endl;
        exit(0);
    }
    cout << "network initialaze. " << endl;

    float cluseringRateMax = -1;
    int r, c, epochMax = 0;

    //numberOfClockCycle = clock();
    //neuralNetwork2.trainOnAllDataBase(inputs, desires, inputs, desires, numberOfEpochs, 1);
    //numberOfClockCycle = clock() - numberOfClockCycle;
    //cout << "\nclustering rate max on GPU : " << cluseringRateMax << endl;
    //cout << "Time for " << numberOfEpochs << " epochs : " << numberOfClockCycle << " clock cycles" << " (" << (float)numberOfClockCycle/CLOCKS_PER_SEC << " secondes)\n" << endl;

    numberOfClockCycle = clock();
    for(c = 1; c <= numberOfEpochs; c++) // c <= 500
    {
        r = randomBetween(0, 150);
        neuralNetwork.train(inputs[r], desires[r]);
        neuralNetwork.resetCalculationOfClusteringRate();
        for(unsigned int i = 0; i < inputs.size(); i++)
        {
            neuralNetwork.calculateClusteringRate(inputs[i], desires[i]);
            //output = neuralNetwork.floatingOutput(input);
            //cout << output[0] << "\t" << output[1] << "\t" << output[2] << "\t" << individuals[i][4] << endl;
        }
        if(neuralNetwork.getClusteringRate() > cluseringRateMax)
        {
            cluseringRateMax = neuralNetwork.getClusteringRate();
            epochMax = c;
        }
        if(c%200 == 0)
        {
            //cout << neuralNetwork.display() << endl;
            cout << "clustering rate : " << neuralNetwork.getClusteringRate() << " epoch : " << c << endl;
            cout << "clustering rate max : " << cluseringRateMax << " epoch : " << epochMax << endl;
        }
    }
    cout << "clustering rate max on CPU : " << cluseringRateMax << endl;
    numberOfClockCycle = clock() - numberOfClockCycle;
    cout << "Time for " << numberOfEpochs << " epochs : " << numberOfClockCycle << " clock cycles" << " (" << (float)numberOfClockCycle/CLOCKS_PER_SEC << " secondes)\n" << endl;
}

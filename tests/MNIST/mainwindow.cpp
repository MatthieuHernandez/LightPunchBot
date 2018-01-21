#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    cout << "STEP 00 : Start" << endl;
    readData();
    //compute();
    //exit(0);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::readData()
{
    labelsTest.clear();
    imagesTest.clear();
    ifstream imagesTestFile;
    ifstream labelsTestFile;
    imagesTestFile.open("C:\\Programming\\LightPunchBot\\tests\\MNIST\\mnist\\t10k-images.idx3-ubyte", ios::in | ios::binary);
    labelsTestFile.open("C:\\Programming\\LightPunchBot\\tests\\MNIST\\mnist\\t10k-labels.idx1-ubyte", ios::in | ios::binary);

    int c;
    int i;

    if(imagesTestFile.is_open() == true
    && labelsTestFile.is_open() == true)
    {
        i = 0;
        while(!labelsTestFile.eof())
        {
            c = labelsTestFile.get();
            if(c >= 0 && c <= 9 && i > 5)
                labelsTest.push_back(c);
            i++;
        }
        i = 0;
        while(!imagesTestFile.eof())
        {
            c = imagesTestFile.get();
            if(c >= 0 && c <= 255 && i > 15)
                imagesTest.push_back(c);
            i++;
        }
        imagesTestFile.close();
        labelsTestFile.close();
        cout << "STEP 01 : Data read" << endl;
        return;
    }
    cout << "STEP 01 FAILED" << endl;
}

inline
unsigned char MainWindow::getImagesTest(int number, int x, int y)
{
    return imagesTest[number * 784 + y * 28 + x];
}

void MainWindow::on_spinBoxLabel_editingFinished()
{

}

void MainWindow::on_spinBoxLabel_valueChanged(int value)
{
    int size = 28;
    int multiple = 10;
    QImage picture(size, size, QImage::Format_RGB32);
    for(int x = 0; x < size; x++)
    {
        for(int y = 0; y < size; y++)
        {
            picture.setPixel(x, y, qRgb((int)getImagesTest(value, x, y),
                                        (int)getImagesTest(value, x, y),
                                        (int)getImagesTest(value, x, y)));
        }
    }
    QImage scalePicture = picture.scaled(size*multiple, size*multiple, Qt::KeepAspectRatio);
    ui->labelPicture->setPixmap(QPixmap::fromImage(scalePicture));
}

void MainWindow::compute()
{
    clock_t numberOfClockCycle;
    NeuralNetwork neuralNetwork(784, 1, 100, 10);

    neuralNetwork.resetCalculationOfClusteringRate();
    neuralNetwork.setLearningRate(0.005f);
    //neuralNetwork.setUseGPU(true);

    if(neuralNetwork.isValid() != 0)
    {
        cout << "ERROR : " << neuralNetwork.getLastError() << endl;
        exit(0);
    }

    vector<float> input;
    input.resize(784);

    vector<float> desired;
    desired.resize(10);
    int r;

    float cluseringRateMax = -1;
    int epochMax = 0;
    int numberTrain = 2000;

    int c;
    int index = 0;
    numberOfClockCycle = clock();
    for(c = 1; -c <= 500; c++)
    {
        input.assign(&imagesTest[index*784], &imagesTest[(index+1)*784]);
        //cout << input.size() << endl;

        for(int i = 0; i < 10; i++)
        {
            if(labelsTest[index] == i)
                desired[i] = 1;
            else
                desired[i] = 0;
        }
        index ++;
        if(index+1 == 10000)
            index = 0;

        neuralNetwork.train(input, desired);
        neuralNetwork.resetCalculationOfClusteringRate();

        if(index%numberTrain == 0)
        {
            for(unsigned int i = 0; i < labelsTest.size(); i++)
            {
                input.assign(&imagesTest[index*784], &imagesTest[(index+1)*784]);

                for(int j = 0; j < 10; j++)
                {
                    if(labelsTest[i] == j)
                        desired[j] = 1;
                    else
                        desired[j] = 0;
                }
                neuralNetwork.calculateClusteringRate(input, desired);
            }
            if(neuralNetwork.getClusteringRate() > cluseringRateMax)
            {
                cluseringRateMax = neuralNetwork.getClusteringRate();
                epochMax = c;
            }
            if(c%1 == 0)
            {
                cout << "clustering rate : " << neuralNetwork.getClusteringRate() << " epoch : " << c << endl;
                cout << "clustering rate max : " << cluseringRateMax << " epoch : " << epochMax << endl;
            }
        }
    }
    numberOfClockCycle = clock() - numberOfClockCycle;
    cout << "Time for " << c-1 << " epochs : " << numberOfClockCycle << " clock cycles" << " (" << (float)numberOfClockCycle/CLOCKS_PER_SEC << " secondes)" << endl;

}

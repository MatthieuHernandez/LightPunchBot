#pragma comment(lib, "user32.lib")
#pragma comment(lib, "advapi32.lib")
#include "gui/mainwindow.h"
#include <QApplication>
#include <time.h>

int main(int argc, char *argv[])
{
    cout << "start application" <<endl;
    QApplication a(argc, argv);
    MainWindow::instance()->show();
    return a.exec();
}

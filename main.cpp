#include "mainwindow.h"

#include <QApplication>
#include "librarydatabase.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}

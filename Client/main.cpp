#include "client.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Client ::getInstance().show();
//    w.show();
    return a.exec();
}

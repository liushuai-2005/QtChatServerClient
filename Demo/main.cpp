#include "client.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    //创建对象
    QApplication a(argc, argv);

    Client w;
    w.show();
    return a.exec();
}

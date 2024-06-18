#include "hotel.h"
#include "entrance.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    hotel w;
    w.hide();
    return a.exec();
}

#include "orgmain.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    OrgMain w;
    w.show();
    return a.exec();
}

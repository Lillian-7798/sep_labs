#include "qlink.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QLink qLink;
    qLink.show();
    return a.exec();
}

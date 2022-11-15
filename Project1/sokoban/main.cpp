#include "sokoban.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    sokoban w;
    w.show();
    return a.exec();
}

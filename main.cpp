#include "Mew.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Mew meow;
    meow.meow();
    return a.exec();
}

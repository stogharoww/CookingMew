#include "Mew.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Mew w;
    w.show();
    return a.exec();
}

#include "Mew.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    a.setWindowIcon(QIcon(":/Logo/resourses/logo/Logo.png"));

    Mew meow;
    meow.meow();
    return a.exec();
}

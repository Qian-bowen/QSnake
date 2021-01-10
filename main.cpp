
#include "dialog.h"


#include <QApplication>

int main(int argc, char *argv[])
{
    srand(time(NULL));

    Atlas_init atlas_init;

    QApplication a(argc, argv);
    Dialog d;
    d.show();

    return a.exec();
}

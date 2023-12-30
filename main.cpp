#include "vista.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    View view;
    //Mostrar despues de elementos agregados
    view.show();
    return a.exec();
}

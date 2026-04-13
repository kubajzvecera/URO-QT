#include <QApplication>
#include "hlavicka.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    AlkoKalkulacka w;
    w.show();
    return a.exec();
}

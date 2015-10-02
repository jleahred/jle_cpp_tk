/*

*/


#include <iostream>

#include <QApplication>
#include "hpgui.h"
#include "core/alarm.h"


int main(int argc, char *argv[])
{
    //QApplication::setStyle("plastique");
    QApplication a(argc, argv);
    Widget w;
    w.setWindowTitle("hp GUI");
    w.show();
    return a.exec();
}


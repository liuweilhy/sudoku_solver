#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif
#include "widget.h"
#include <QDebug>
#include <QApplication>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    widget w;
    w.show();

    return a.exec();
}

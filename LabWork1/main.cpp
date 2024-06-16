#include "widget.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Widget widget;
    widget.setWindowTitle("Отображение текста по эллипсу");
    //widget.resize(1800, 1000); // Размер 1920x1080
    widget.show();
    return a.exec();
}

#include "widget.h"
#include <QIcon>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    Widget w;
    w.setWindowTitle("Калькулятор питания в походе");
    w.resize(400,200);

    QIcon ico(":/картинки/pic3.jpg");
    w.setWindowIcon(ico);
    app.setWindowIcon(ico);

    w.show();

    return app.exec();
}

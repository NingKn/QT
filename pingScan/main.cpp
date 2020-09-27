#include "mywidget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MyWidget w;
    w.setWindowTitle("网络测试工具");
    w.show();

    return a.exec();
}

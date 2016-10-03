#include <QApplication>
#include "ui/MainWindow.hpp"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow wnd;
    wnd.show();
    wnd.loginDialog.show();

    return a.exec();
}

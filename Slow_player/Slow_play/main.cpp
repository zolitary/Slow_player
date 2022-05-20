#include "mainwindow.h"
#include <QApplication>
#undef main
int s_thread_exit_flag=0;
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}

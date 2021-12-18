#include "MainWindow.h"

#include <QApplication>



int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    a.setOrganizationName("Zaid Rao");
    a.setOrganizationDomain("https://github.com/zaidrao");
    a.setApplicationName("Code Editor");
    a.setApplicationDisplayName("Code & Text Editor");
    a.setApplicationVersion("0.9.1-Beta");

    MainWindow w;
    w.show();
    return a.exec();
}

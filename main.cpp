#include <QApplication>
#include <QTranslator>
#include "ui/main_window.h"


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QTranslator translator;
    if (translator.load(":/translations/ru.qm"))
        app.installTranslator(&translator);

    MainWindow window;
    window.show();
    return app.exec();
}
#include <QApplication>
#include "mainwindow.h"
#include "databasemanager.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Инициализация соединения с базой данных
    if (!DatabaseManager::instance().openConnection("localhost", "postgres", "postgres", "postgres")) {
        return -1;
    }

    MainWindow w;
    w.show();

    return a.exec();
}

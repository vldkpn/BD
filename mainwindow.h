#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>

class QMenu;
class QToolBar;
class QAction;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

private slots:
    void showOffices();
    void showEmployees();
    void showPositions();
    void showTariffs();
    void showSubscribers();
   // void showEmployeeSubscribers();

private:
    void createMenus();
    void createToolBars();

    QStackedWidget *stackedWidget;

    // Actions
    QAction *officeAction;
    QAction *employeeAction;
    QAction *positionAction;
    QAction *tariffAction;
    QAction *subscriberAction;
   // QAction *employeeSubscriberAction;
};

#endif // MAINWINDOW_H

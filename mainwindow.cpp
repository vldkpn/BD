#include "mainwindow.h"
#include "officewidget.h"
#include "employeewidget.h"
#include "positionwidget.h"
#include "tariffwidget.h"
#include "subscriberwidget.h"
#include <QKeyEvent>
//#include "employeesubscriberwidget.h"

#include <QMenuBar>
#include <QToolBar>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    stackedWidget = new QStackedWidget(this);
    setCentralWidget(stackedWidget);

    createMenus();
    createToolBars();
    resize(800, 400);  // Устанавливаем начальный размер
    setMinimumSize(400, 300);  // Устанавливаем минимальный размер
    // Инициализация виджетов
    OfficeWidget *officeWidget = new OfficeWidget(this);
    EmployeeWidget *employeeWidget = new EmployeeWidget(this);
    PositionWidget *positionWidget = new PositionWidget(this);
    TariffWidget *tariffWidget = new TariffWidget(this);
    SubscriberWidget *subscriberWidget = new SubscriberWidget(this);
  //  EmployeeSubscriberWidget *employeeSubscriberWidget = new EmployeeSubscriberWidget(this);

    // Добавление виджетов в стек
    stackedWidget->addWidget(officeWidget); // индекс 0
    stackedWidget->addWidget(employeeWidget); // индекс 1
    stackedWidget->addWidget(positionWidget); // индекс 2
    stackedWidget->addWidget(tariffWidget); // индекс 3
    stackedWidget->addWidget(subscriberWidget); // индекс 4
  //  stackedWidget->addWidget(employeeSubscriberWidget); // индекс 5

    // Отображение начального виджета
    stackedWidget->setCurrentIndex(0);
}

void MainWindow::createMenus()
{
    QMenu *fileMenu = menuBar()->addMenu(tr("&Файл"));
    QAction *exitAction = fileMenu->addAction(tr("Выход"));
    connect(exitAction, &QAction::triggered, this, &QWidget::close);

    QMenu *viewMenu = menuBar()->addMenu(tr("&Просмотр"));
    officeAction = viewMenu->addAction(tr("&Офисы"), this, &MainWindow::showOffices);
    employeeAction = viewMenu->addAction(tr("&Сотрудники"), this, &MainWindow::showEmployees);
    positionAction = viewMenu->addAction(tr("&Должности"), this, &MainWindow::showPositions);
    tariffAction = viewMenu->addAction(tr("&Тарифы"), this, &MainWindow::showTariffs);
    subscriberAction = viewMenu->addAction(tr("&Абоненты"), this, &MainWindow::showSubscribers);
  //  employeeSubscriberAction = viewMenu->addAction(tr("&Связи сотрудник-абонент"), this, &MainWindow::showEmployeeSubscribers);
}

void MainWindow::createToolBars()
{
   // QToolBar *mainToolBar = addToolBar(tr("Главная"));

    //mainToolBar->addAction(officeAction);
    //mainToolBar->addAction(employeeAction);
    //mainToolBar->addAction(positionAction);
    //mainToolBar->addAction(tariffAction);
    //mainToolBar->addAction(subscriberAction);
    //mainToolBar->addAction(employeeSubscriberAction);
}

void MainWindow::showOffices()
{
    stackedWidget->setCurrentIndex(0);
}

void MainWindow::showEmployees()
{
    stackedWidget->setCurrentIndex(1);
}

void MainWindow::showPositions()
{
    stackedWidget->setCurrentIndex(2);
}

void MainWindow::showTariffs()
{
    stackedWidget->setCurrentIndex(3);
}

void MainWindow::showSubscribers()
{
    stackedWidget->setCurrentIndex(4);
}
/*
void MainWindow::showEmployeeSubscribers()
{
    stackedWidget->setCurrentIndex(5);
}
*/

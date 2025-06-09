#ifndef EMPLOYEESUBSCRIBERWIDGET_H
#define EMPLOYEESUBSCRIBERWIDGET_H

#include <QWidget>
#include <QSqlRelationalTableModel>

class QTableView;
class QPushButton;

class EmployeeSubscriberWidget : public QWidget
{
    Q_OBJECT

public:
    explicit EmployeeSubscriberWidget(QWidget *parent = nullptr);

private slots:
    void addRelation();
    void deleteRelation();
    void refreshData();

private:
    QSqlRelationalTableModel *model;
    QTableView *tableView;

    QPushButton *addButton;
    QPushButton *deleteButton;
    QPushButton *refreshButton;
};

#endif // EMPLOYEESUBSCRIBERWIDGET_H

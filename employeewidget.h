#ifndef EMPLOYEEWIDGET_H
#define EMPLOYEEWIDGET_H

#include <QWidget>
#include <QSqlRelationalTableModel>

class QTableView;
class QPushButton;

class EmployeeWidget : public QWidget
{
    Q_OBJECT

public:
    explicit EmployeeWidget(QWidget *parent = nullptr);

private slots:
    void addEmployee();
    void editEmployee();
    void deleteEmployee();
    void refreshData();
    void sortTable(); // Добавлено

protected:
    bool eventFilter(QObject *obj, QEvent *event) override; // Добавлено

private:
    QSqlRelationalTableModel *model;
    QTableView *tableView;

    QPushButton *addButton;
    QPushButton *editButton;
    QPushButton *deleteButton;
    QPushButton *refreshButton;

};

#endif // EMPLOYEEWIDGET_H

#ifndef OFFICEWIDGET_H
#define OFFICEWIDGET_H

#include <QWidget>
#include <QSqlTableModel>

class QTableView;
class QPushButton;

class OfficeWidget : public QWidget
{
    Q_OBJECT

public:
    explicit OfficeWidget(QWidget *parent = nullptr);

private slots:
    void addOffice();
    void editOffice();
    void deleteOffice();
    void refreshData();
    void sortTable(); // Добавлено

protected:
    bool eventFilter(QObject *obj, QEvent *event) override; // Добавлено

private:
    QSqlTableModel *model;
    QTableView *tableView;

    QPushButton *addButton;
    QPushButton *editButton;
    QPushButton *deleteButton;
    QPushButton *refreshButton;
};

#endif // OFFICEWIDGET_H

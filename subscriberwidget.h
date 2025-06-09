#ifndef SUBSCRIBERWIDGET_H
#define SUBSCRIBERWIDGET_H

#include <QWidget>
#include <QSqlRelationalTableModel>

class QTableView;
class QPushButton;

class SubscriberWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SubscriberWidget(QWidget *parent = nullptr);

private slots:
    void addSubscriber();
    void editSubscriber();
    void deleteSubscriber();
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

#endif // SUBSCRIBERWIDGET_H

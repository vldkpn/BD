#ifndef POSITIONWIDGET_H
#define POSITIONWIDGET_H

#include <QWidget>
#include <QSqlTableModel>

class QTableView;
class QPushButton;

class PositionWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PositionWidget(QWidget *parent = nullptr);

private slots:
    void addPosition();
    void editPosition();
    void deletePosition();
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

#endif // POSITIONWIDGET_H

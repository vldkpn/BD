#ifndef TARIFFWIDGET_H
#define TARIFFWIDGET_H

#include <QWidget>
#include <QSqlTableModel>

class QTableView;
class QPushButton;

class TariffWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TariffWidget(QWidget *parent = nullptr);

private slots:
    void addTariff();
    void editTariff();
    void deleteTariff();
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

#endif // TARIFFWIDGET_H

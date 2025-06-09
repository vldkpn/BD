#include "employeesubscriberwidget.h"
#include "employeesubscriberdialog.h"

#include <QTableView>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSqlRelationalDelegate>
#include <QtSql/QSqlError>
#include <QMessageBox>

EmployeeSubscriberWidget::EmployeeSubscriberWidget(QWidget *parent)
    : QWidget(parent)
{
    model = new QSqlRelationalTableModel(this);
    model->setTable("employee_subscriber");
    model->setRelation(model->fieldIndex("employee_id"), QSqlRelation("employee", "id", "name"));
    model->setRelation(model->fieldIndex("subscriber_id"), QSqlRelation("subscriber", "id", "name"));
    model->select();

    model->setHeaderData(model->fieldIndex("id"), Qt::Horizontal, tr("ID"));
    model->setHeaderData(model->fieldIndex("employee_name_2"), Qt::Horizontal, tr("Сотрудник"));
    model->setHeaderData(model->fieldIndex("name"), Qt::Horizontal, tr("Абонент"));

    tableView = new QTableView(this);
    tableView->setModel(model);
    tableView->hideColumn(model->fieldIndex("id"));
    tableView->setItemDelegate(new QSqlRelationalDelegate(tableView));
    tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    tableView->resizeColumnsToContents();

    // Добавлено: включение сортировки
    tableView->setSortingEnabled(true);

    addButton = new QPushButton(tr("Добавить связь"), this);
    deleteButton = new QPushButton(tr("Удалить связь"), this);
    refreshButton = new QPushButton(tr("Обновить"), this);

    connect(addButton, &QPushButton::clicked, this, &EmployeeSubscriberWidget::addRelation);
    connect(deleteButton, &QPushButton::clicked, this, &EmployeeSubscriberWidget::deleteRelation);
    connect(refreshButton, &QPushButton::clicked, this, &EmployeeSubscriberWidget::refreshData);

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(addButton);
    buttonLayout->addWidget(deleteButton);
    buttonLayout->addWidget(refreshButton);
    buttonLayout->addStretch();

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(tableView);
    mainLayout->addLayout(buttonLayout);

    setLayout(mainLayout);
}


void EmployeeSubscriberWidget::addRelation()
{
    EmployeeSubscriberDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        model->select();
    }
}

void EmployeeSubscriberWidget::deleteRelation()
{
    QModelIndexList selection = tableView->selectionModel()->selectedRows();
    if (selection.count() == 0) {
        QMessageBox::warning(this, tr("Удаление связи"), tr("Пожалуйста, выберите связь для удаления."));
        return;
    }
    int row = selection.at(0).row();

    int ret = QMessageBox::warning(this, tr("Удаление связи"),
                                   tr("Вы уверены, что хотите удалить выбранную связь?"),
                                   QMessageBox::Yes | QMessageBox::No);
    if (ret == QMessageBox::Yes) {
        if (!model->removeRow(row)) {
            QMessageBox::warning(this, tr("Удаление связи"), tr("Не удалось удалить связь: %1").arg(model->lastError().text()));
        } else {
            model->submitAll();
        }
    }
}

void EmployeeSubscriberWidget::refreshData()
{
    model->select();
}

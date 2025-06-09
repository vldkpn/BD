#include "employeesubscriberdialog.h"
#include <QComboBox>
#include <QFormLayout>
#include <QPushButton>
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>

EmployeeSubscriberDialog::EmployeeSubscriberDialog(QWidget *parent)
    : QDialog(parent)
{
    employeeCombo = new QComboBox(this);
    subscriberCombo = new QComboBox(this);

    // Заполнение employeeCombo
    QSqlQueryModel *employeeModel = new QSqlQueryModel(this);
    employeeModel->setQuery("SELECT id, name FROM employee");
    employeeCombo->setModel(employeeModel);
    employeeCombo->setModelColumn(1);

    // Заполнение subscriberCombo
    QSqlQueryModel *subscriberModel = new QSqlQueryModel(this);
    subscriberModel->setQuery("SELECT id, name FROM subscriber");
    subscriberCombo->setModel(subscriberModel);
    subscriberCombo->setModelColumn(1);

    QFormLayout *formLayout = new QFormLayout;
    formLayout->addRow(tr("Сотрудник:"), employeeCombo);
    formLayout->addRow(tr("Абонент:"), subscriberCombo);

    QPushButton *saveButton = new QPushButton(tr("Сохранить"), this);
    connect(saveButton, &QPushButton::clicked, this, &EmployeeSubscriberDialog::saveRelation);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(formLayout);
    mainLayout->addWidget(saveButton);

    setLayout(mainLayout);
    setWindowTitle(tr("Связь сотрудник-абонент"));
}

void EmployeeSubscriberDialog::saveRelation()
{
    int employeeId = employeeCombo->model()->index(employeeCombo->currentIndex(), 0).data().toInt();
    int subscriberId = subscriberCombo->model()->index(subscriberCombo->currentIndex(), 0).data().toInt();

    QSqlQuery query;
    query.prepare("INSERT INTO employee_subscriber (employee_id, subscriber_id) VALUES (:employee_id, :subscriber_id)");
    query.bindValue(":employee_id", employeeId);
    query.bindValue(":subscriber_id", subscriberId);

    if (!query.exec()) {
        QMessageBox::warning(this, tr("Сохранение связи"), tr("Не удалось сохранить связь: %1").arg(query.lastError().text()));
        return;
    }

    accept();
}

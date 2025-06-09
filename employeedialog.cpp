#include "employeedialog.h"
#include <QLineEdit>
#include <QComboBox>
#include <QFormLayout>
#include <QPushButton>
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>

EmployeeDialog::EmployeeDialog(QWidget *parent)
    : QDialog(parent), m_id(-1)
{
    nameEdit = new QLineEdit(this);
    positionCombo = new QComboBox(this);
    officeCombo = new QComboBox(this);
    contactEdit = new QLineEdit(this);

    // Заполнение positionCombo
    QSqlQueryModel *positionModel = new QSqlQueryModel(this);
    positionModel->setQuery("SELECT id, position_name FROM position");
    positionCombo->setModel(positionModel);
    positionCombo->setModelColumn(1);

    // Заполнение officeCombo
    QSqlQueryModel *officeModel = new QSqlQueryModel(this);
    officeModel->setQuery("SELECT id, name FROM office");
    officeCombo->setModel(officeModel);
    officeCombo->setModelColumn(1);

    QFormLayout *formLayout = new QFormLayout;
    formLayout->addRow(tr("ФИО:"), nameEdit);
    formLayout->addRow(tr("Должность:"), positionCombo);
    formLayout->addRow(tr("Офис:"), officeCombo);
    formLayout->addRow(tr("Контакты:"), contactEdit);

    QPushButton *saveButton = new QPushButton(tr("Сохранить"), this);
    connect(saveButton, &QPushButton::clicked, this, &EmployeeDialog::saveEmployee);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(formLayout);
    mainLayout->addWidget(saveButton);

    setLayout(mainLayout);
    setWindowTitle(tr("Сотрудник"));
}

void EmployeeDialog::loadEmployee(int id)
{
    m_id = id;
    QSqlQuery query;
    query.prepare("SELECT name, position_id, office_id, contact_info FROM employee WHERE id = :id");
    query.bindValue(":id", m_id);
    if (query.exec()) {
        if (query.next()) {
            nameEdit->setText(query.value(0).toString());

            // Установка должности
            int positionId = query.value(1).toInt();
            for (int i = 0; i < positionCombo->model()->rowCount(); ++i) {
                if (positionCombo->model()->index(i, 0).data().toInt() == positionId) {
                    positionCombo->setCurrentIndex(i);
                    break;
                }
            }

            // Установка офиса
            int officeId = query.value(2).toInt();
            for (int i = 0; i < officeCombo->model()->rowCount(); ++i) {
                if (officeCombo->model()->index(i, 0).data().toInt() == officeId) {
                    officeCombo->setCurrentIndex(i);
                    break;
                }
            }

            contactEdit->setText(query.value(3).toString());
        }
    } else {
        QMessageBox::warning(this, tr("Загрузка сотрудника"), tr("Не удалось загрузить данные сотрудника: %1").arg(query.lastError().text()));
    }
}

void EmployeeDialog::saveEmployee()
{
    QString name = nameEdit->text();
    int positionId = positionCombo->model()->index(positionCombo->currentIndex(), 0).data().toInt();
    int officeId = officeCombo->model()->index(officeCombo->currentIndex(), 0).data().toInt();
    QString contactInfo = contactEdit->text();

    QSqlQuery query;
    if (m_id == -1) {
        query.prepare("INSERT INTO employee (name, position_id, office_id, contact_info) VALUES (:name, :position_id, :office_id, :contact_info)");
    } else {
        query.prepare("UPDATE employee SET name = :name, position_id = :position_id, office_id = :office_id, contact_info = :contact_info WHERE id = :id");
        query.bindValue(":id", m_id);
    }
    query.bindValue(":name", name);
    query.bindValue(":position_id", positionId);
    query.bindValue(":office_id", officeId);
    query.bindValue(":contact_info", contactInfo);

    if (!query.exec()) {
        QMessageBox::warning(this, tr("Сохранение сотрудника"), tr("Не удалось сохранить сотрудника: %1").arg(query.lastError().text()));
        return;
    }

    accept();
}

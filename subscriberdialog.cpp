#include "subscriberdialog.h"
#include <QLineEdit>
#include <QComboBox>
#include <QDateEdit>
#include <QFormLayout>
#include <QPushButton>
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QDebug>

SubscriberDialog::SubscriberDialog(QWidget *parent)
    : QDialog(parent), m_id(-1)
{
    nameEdit = new QLineEdit(this);
    tariffCombo = new QComboBox(this);
    contactEdit = new QLineEdit(this);
    contractDateEdit = new QDateEdit(this);
    contractDateEdit->setCalendarPopup(true);
    contractDateEdit->setDate(QDate::currentDate());

    employeeCombo = new QComboBox(this);

    // Заполнение tariffCombo
    QSqlQueryModel *tariffModel = new QSqlQueryModel(this);
    tariffModel->setQuery("SELECT id, name FROM tariff");
    tariffCombo->setModel(tariffModel);
    tariffCombo->setModelColumn(1);

    // **Изменение здесь**: Заполнение employeeCombo только менеджерами по продажам
    QSqlQueryModel *employeeModel = new QSqlQueryModel(this);

    // Получаем идентификатор должности "Менеджер по продажам"
    QSqlQuery positionQuery;
    positionQuery.prepare("SELECT id FROM position WHERE position_name = :position_name");
    positionQuery.bindValue(":position_name", "Менеджер по продажам");
    int managerPositionId = -1;
    if (positionQuery.exec()) {
        if (positionQuery.next()) {
            managerPositionId = positionQuery.value(0).toInt();
        } else {
            QMessageBox::warning(this, tr("Ошибка"), tr("Должность 'Менеджер по продажам' не найдена."));
        }
    } else {
        QMessageBox::warning(this, tr("Ошибка"), tr("Не удалось выполнить запрос для получения должности: %1").arg(positionQuery.lastError().text()));
    }

    if (managerPositionId != -1) {
        // Запрашиваем сотрудников с должностью "Менеджер по продажам"
        employeeModel->setQuery(QString("SELECT id, name FROM employee WHERE position_id = %1").arg(managerPositionId));
        employeeCombo->setModel(employeeModel);
        employeeCombo->setModelColumn(1);
    } else {
        // Если должность не найдена, оставляем список сотрудников пустым
        employeeModel->setQuery("SELECT id, name FROM employee WHERE 1=0"); // Пустой запрос
        employeeCombo->setModel(employeeModel);
        employeeCombo->setModelColumn(1);
    }

    QFormLayout *formLayout = new QFormLayout;
    formLayout->addRow(tr("ФИО:"), nameEdit);
    formLayout->addRow(tr("Тариф:"), tariffCombo);
    formLayout->addRow(tr("Контакты:"), contactEdit);
    formLayout->addRow(tr("Дата договора:"), contractDateEdit);
    formLayout->addRow(tr("Сотрудник:"), employeeCombo);

    QPushButton *saveButton = new QPushButton(tr("Сохранить"), this);
    connect(saveButton, &QPushButton::clicked, this, &SubscriberDialog::saveSubscriber);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(formLayout);
    mainLayout->addWidget(saveButton);

    setLayout(mainLayout);
    setWindowTitle(tr("Абонент"));
}

void SubscriberDialog::loadSubscriber(int id)
{
    m_id = id;
    QSqlQuery query;
    query.prepare("SELECT name, tariff_id, contact_info, contract_date, employee_id FROM subscriber WHERE id = :id");
    query.bindValue(":id", m_id);
    if (query.exec()) {
        if (query.next()) {
            nameEdit->setText(query.value(0).toString());

            // Установка тарифа
            int tariffId = query.value(1).toInt();
            for (int i = 0; i < tariffCombo->model()->rowCount(); ++i) {
                if (tariffCombo->model()->index(i, 0).data().toInt() == tariffId) {
                    tariffCombo->setCurrentIndex(i);
                    break;
                }
            }

            contactEdit->setText(query.value(2).toString());

            // Установка даты договора
            QDate contractDate = query.value(3).toDate();
            contractDateEdit->setDate(contractDate);

            // Установка сотрудника
            int employeeId = query.value(4).toInt();
            for (int i = 0; i < employeeCombo->model()->rowCount(); ++i) {
                if (employeeCombo->model()->index(i, 0).data().toInt() == employeeId) {
                    employeeCombo->setCurrentIndex(i);
                    break;
                }
            }
        }
    } else {
        QMessageBox::warning(this, tr("Загрузка абонента"), tr("Не удалось загрузить данные абонента: %1").arg(query.lastError().text()));
    }
}

void SubscriberDialog::saveSubscriber()
{
    QString name = nameEdit->text();
    int tariffId = tariffCombo->model()->index(tariffCombo->currentIndex(), 0).data().toInt();
    QString contactInfo = contactEdit->text();
    QDate contractDate = contractDateEdit->date();
    int employeeId = employeeCombo->model()->index(employeeCombo->currentIndex(), 0).data().toInt();

    QSqlQuery query;
    if (m_id == -1) {
        query.prepare("INSERT INTO subscriber (name, tariff_id, contact_info, contract_date, employee_id) VALUES (:name, :tariff_id, :contact_info, :contract_date, :employee_id)");
    } else {
        query.prepare("UPDATE subscriber SET name = :name, tariff_id = :tariff_id, contact_info = :contact_info, contract_date = :contract_date, employee_id = :employee_id WHERE id = :id");
        query.bindValue(":id", m_id);
    }
    query.bindValue(":name", name);
    query.bindValue(":tariff_id", tariffId);
    query.bindValue(":contact_info", contactInfo);
    query.bindValue(":contract_date", contractDate);
    query.bindValue(":employee_id", employeeId);

    if (!query.exec()) {
        QMessageBox::warning(this, tr("Сохранение абонента"), tr("Не удалось сохранить абонента: %1").arg(query.lastError().text()));
        return;
    }

    accept();
}

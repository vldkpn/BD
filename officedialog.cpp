#include "officedialog.h"
#include <QLineEdit>
#include <QFormLayout>
#include <QPushButton>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>

OfficeDialog::OfficeDialog(QWidget *parent)
    : QDialog(parent), m_id(-1)
{
    nameEdit = new QLineEdit(this);
    addressEdit = new QLineEdit(this);
    phoneEdit = new QLineEdit(this);

    QFormLayout *formLayout = new QFormLayout;
    formLayout->addRow(tr("Название:"), nameEdit);
    formLayout->addRow(tr("Адрес:"), addressEdit);
    formLayout->addRow(tr("Телефон:"), phoneEdit);

    QPushButton *saveButton = new QPushButton(tr("Сохранить"), this);
    connect(saveButton, &QPushButton::clicked, this, &OfficeDialog::saveOffice);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(formLayout);
    mainLayout->addWidget(saveButton);

    setLayout(mainLayout);
    setWindowTitle(tr("Офис"));
}

void OfficeDialog::loadOffice(int id)
{
    m_id = id;
    QSqlQuery query;
    query.prepare("SELECT name, address, phone FROM office WHERE id = :id");
    query.bindValue(":id", m_id);
    if (query.exec()) {
        if (query.next()) {
            nameEdit->setText(query.value(0).toString());
            addressEdit->setText(query.value(1).toString());
            phoneEdit->setText(query.value(2).toString());
        }
    } else {
        QMessageBox::warning(this, tr("Загрузка офиса"), tr("Не удалось загрузить данные офиса: %1").arg(query.lastError().text()));
    }
}

void OfficeDialog::saveOffice()
{
    QString name = nameEdit->text();
    QString address = addressEdit->text();
    QString phone = phoneEdit->text();

    QSqlQuery query;
    if (m_id == -1) {
        query.prepare("INSERT INTO office (name, address, phone) VALUES (:name, :address, :phone)");
    } else {
        query.prepare("UPDATE office SET name = :name, address = :address, phone = :phone WHERE id = :id");
        query.bindValue(":id", m_id);
    }
    query.bindValue(":name", name);
    query.bindValue(":address", address);
    query.bindValue(":phone", phone);

    if (!query.exec()) {
        QMessageBox::warning(this, tr("Сохранение офиса"), tr("Не удалось сохранить офис: %1").arg(query.lastError().text()));
        return;
    }

    accept();
}

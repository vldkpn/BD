#include "tariffdialog.h"
#include <QLineEdit>
#include <QTextEdit>
#include <QFormLayout>
#include <QPushButton>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>

TariffDialog::TariffDialog(QWidget *parent)
    : QDialog(parent), m_id(-1)
{
    nameEdit = new QLineEdit(this);
    priceEdit = new QLineEdit(this);
    descriptionEdit = new QTextEdit(this);

    QFormLayout *formLayout = new QFormLayout;
    formLayout->addRow(tr("Название тарифа:"), nameEdit);
    formLayout->addRow(tr("Стоимость:"), priceEdit);
    formLayout->addRow(tr("Описание:"), descriptionEdit);

    QPushButton *saveButton = new QPushButton(tr("Сохранить"), this);
    connect(saveButton, &QPushButton::clicked, this, &TariffDialog::saveTariff);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(formLayout);
    mainLayout->addWidget(saveButton);

    setLayout(mainLayout);
    setWindowTitle(tr("Тариф"));
}

void TariffDialog::loadTariff(int id)
{
    m_id = id;
    QSqlQuery query;
    query.prepare("SELECT name, price, description FROM tariff WHERE id = :id");
    query.bindValue(":id", m_id);
    if (query.exec()) {
        if (query.next()) {
            nameEdit->setText(query.value(0).toString());
            priceEdit->setText(query.value(1).toString());
            descriptionEdit->setPlainText(query.value(2).toString());
        }
    } else {
        QMessageBox::warning(this, tr("Загрузка тарифа"), tr("Не удалось загрузить данные тарифа: %1").arg(query.lastError().text()));
    }
}

void TariffDialog::saveTariff()
{
    QString name = nameEdit->text();
    double price = priceEdit->text().toDouble();
    QString description = descriptionEdit->toPlainText();

    QSqlQuery query;
    if (m_id == -1) {
        query.prepare("INSERT INTO tariff (name, price, description) VALUES (:name, :price, :description)");
    } else {
        query.prepare("UPDATE tariff SET name = :name, price = :price, description = :description WHERE id = :id");
        query.bindValue(":id", m_id);
    }
    query.bindValue(":name", name);
    query.bindValue(":price", price);
    query.bindValue(":description", description);

    if (!query.exec()) {
        QMessageBox::warning(this, tr("Сохранение тарифа"), tr("Не удалось сохранить тариф: %1").arg(query.lastError().text()));
        return;
    }

    accept();
}

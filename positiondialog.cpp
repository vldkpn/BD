#include "positiondialog.h"
#include <QLineEdit>
#include <QTextEdit>
#include <QFormLayout>
#include <QPushButton>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>

PositionDialog::PositionDialog(QWidget *parent)
    : QDialog(parent), m_id(-1)
{
    nameEdit = new QLineEdit(this);
    descriptionEdit = new QTextEdit(this);

    QFormLayout *formLayout = new QFormLayout;
    formLayout->addRow(tr("Название должности:"), nameEdit);
    formLayout->addRow(tr("Описание:"), descriptionEdit);

    QPushButton *saveButton = new QPushButton(tr("Сохранить"), this);
    connect(saveButton, &QPushButton::clicked, this, &PositionDialog::savePosition);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(formLayout);
    mainLayout->addWidget(saveButton);

    setLayout(mainLayout);
    setWindowTitle(tr("Должность"));
}

void PositionDialog::loadPosition(int id)
{
    m_id = id;
    QSqlQuery query;
    query.prepare("SELECT position_name, description FROM position WHERE id = :id");
    query.bindValue(":id", m_id);
    if (query.exec()) {
        if (query.next()) {
            nameEdit->setText(query.value(0).toString());
            descriptionEdit->setPlainText(query.value(1).toString());
        }
    } else {
        QMessageBox::warning(this, tr("Загрузка должности"), tr("Не удалось загрузить данные должности: %1").arg(query.lastError().text()));
    }
}

void PositionDialog::savePosition()
{
    QString name = nameEdit->text();
    QString description = descriptionEdit->toPlainText();

    QSqlQuery query;
    if (m_id == -1) {
        query.prepare("INSERT INTO position (position_name, description) VALUES (:name, :description)");
    } else {
        query.prepare("UPDATE position SET position_name = :name, description = :description WHERE id = :id");
        query.bindValue(":id", m_id);
    }
    query.bindValue(":name", name);
    query.bindValue(":description", description);

    if (!query.exec()) {
        QMessageBox::warning(this, tr("Сохранение должности"), tr("Не удалось сохранить должность: %1").arg(query.lastError().text()));
        return;
    }

    accept();
}

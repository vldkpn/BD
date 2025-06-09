#ifndef SUBSCRIBERDIALOG_H
#define SUBSCRIBERDIALOG_H

#include <QDialog>

class QLineEdit;
class QComboBox;
class QDateEdit; // Добавлено для выбора даты

class SubscriberDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SubscriberDialog(QWidget *parent = nullptr);
    void loadSubscriber(int id);

private slots:
    void saveSubscriber();

private:
    int m_id;
    QLineEdit *nameEdit;
    QComboBox *tariffCombo;
    QLineEdit *contactEdit;
    QDateEdit *contractDateEdit; // Новое поле
    QComboBox *employeeCombo;    // Новое поле
};

#endif // SUBSCRIBERDIALOG_H

#ifndef EMPLOYEESUBSCRIBERDIALOG_H
#define EMPLOYEESUBSCRIBERDIALOG_H

#include <QDialog>

class QComboBox;

class EmployeeSubscriberDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EmployeeSubscriberDialog(QWidget *parent = nullptr);

private slots:
    void saveRelation();

private:
    QComboBox *employeeCombo;
    QComboBox *subscriberCombo;
};

#endif // EMPLOYEESUBSCRIBERDIALOG_H

#ifndef EMPLOYEEDIALOG_H
#define EMPLOYEEDIALOG_H

#include <QDialog>

class QLineEdit;
class QComboBox;

class EmployeeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EmployeeDialog(QWidget *parent = nullptr);
    void loadEmployee(int id);

private slots:
    void saveEmployee();

private:
    int m_id;
    QLineEdit *nameEdit;
    QComboBox *positionCombo;
    QComboBox *officeCombo;
    QLineEdit *contactEdit;
};

#endif // EMPLOYEEDIALOG_H

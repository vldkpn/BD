#ifndef OFFICEDIALOG_H
#define OFFICEDIALOG_H

#include <QDialog>

class QLineEdit;

class OfficeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit OfficeDialog(QWidget *parent = nullptr);
    void loadOffice(int id);

private slots:
    void saveOffice();

private:
    int m_id;
    QLineEdit *nameEdit;
    QLineEdit *addressEdit;
    QLineEdit *phoneEdit;
};

#endif // OFFICEDIALOG_H

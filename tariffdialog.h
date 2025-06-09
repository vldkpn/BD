#ifndef TARIFFDIALOG_H
#define TARIFFDIALOG_H

#include <QDialog>

class QLineEdit;
class QTextEdit;

class TariffDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TariffDialog(QWidget *parent = nullptr);
    void loadTariff(int id);

private slots:
    void saveTariff();

private:
    int m_id;
    QLineEdit *nameEdit;
    QLineEdit *priceEdit;
    QTextEdit *descriptionEdit;
};

#endif // TARIFFDIALOG_H

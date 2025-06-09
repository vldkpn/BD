#ifndef POSITIONDIALOG_H
#define POSITIONDIALOG_H

#include <QDialog>

class QLineEdit;
class QTextEdit;

class PositionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PositionDialog(QWidget *parent = nullptr);
    void loadPosition(int id);

private slots:
    void savePosition();

private:
    int m_id;
    QLineEdit *nameEdit;
    QTextEdit *descriptionEdit;
};

#endif // POSITIONDIALOG_H

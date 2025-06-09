#include "tariffwidget.h"
#include "tariffdialog.h"

#include <QTableView>
#include <QHeaderView>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QtSql/QSqlError>
#include <QMessageBox>
#include <QKeyEvent>

TariffWidget::TariffWidget(QWidget *parent)
    : QWidget(parent)
{
    model = new QSqlTableModel(this);
    model->setTable("tariff");
    model->select();

    model->setHeaderData(model->fieldIndex("id"), Qt::Horizontal, tr("ID"));
    model->setHeaderData(model->fieldIndex("name"), Qt::Horizontal, tr("Название тарифа"));
    model->setHeaderData(model->fieldIndex("price"), Qt::Horizontal, tr("Стоимость"));
    model->setHeaderData(model->fieldIndex("description"), Qt::Horizontal, tr("Описание"));

    tableView = new QTableView(this);
    tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    tableView->setModel(model);
    tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableView->hideColumn(model->fieldIndex("id"));
    tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    tableView->resizeColumnsToContents();

    // Добавлено: включение сортировки
    tableView->setSortingEnabled(true);
    tableView->installEventFilter(this);

    addButton = new QPushButton(tr("Добавить"), this);
    editButton = new QPushButton(tr("Редактировать"), this);
    deleteButton = new QPushButton(tr("Удалить"), this);
   // refreshButton = new QPushButton(tr("Обновить"), this);

    connect(addButton, &QPushButton::clicked, this, &TariffWidget::addTariff);
    connect(editButton, &QPushButton::clicked, this, &TariffWidget::editTariff);
    connect(deleteButton, &QPushButton::clicked, this, &TariffWidget::deleteTariff);
    //connect(refreshButton, &QPushButton::clicked, this, &TariffWidget::refreshData);

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(addButton);
    buttonLayout->addWidget(editButton);
    buttonLayout->addWidget(deleteButton);
   // buttonLayout->addWidget(refreshButton);
    buttonLayout->addStretch();

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(tableView);
    mainLayout->addLayout(buttonLayout);

    setLayout(mainLayout);
}

bool TariffWidget::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == tableView && event->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
        switch (keyEvent->key()) {
        case Qt::Key_Return:
        case Qt::Key_Enter:
            editTariff();
            return true; // Событие обработано
        case Qt::Key_Delete:
            deleteTariff();
            return true;
        case Qt::Key_Insert:
            addTariff();
            return true;
        case Qt::Key_S: // Обработка нажатия клавиши 'S'
            sortTable();
            return true;
        default:
            break;
        }
    }
    return QWidget::eventFilter(obj, event);
}

void TariffWidget::sortTable()
{
    // Получаем текущий выбранный столбец
    int currentColumn = tableView->currentIndex().column();

    // Если нет выбранного столбца, сортируем по первому столбцу
    if (currentColumn == -1) {
        currentColumn = 0;
    }

    // Получаем порядок сортировки: если уже отсортировано по этому столбцу в порядке возрастания, меняем на убывание
    Qt::SortOrder sortOrder = Qt::AscendingOrder;
    if (tableView->horizontalHeader()->sortIndicatorSection() == currentColumn &&
        tableView->horizontalHeader()->sortIndicatorOrder() == Qt::AscendingOrder) {
        sortOrder = Qt::DescendingOrder;
    }

    // Устанавливаем сортировку в модели
    model->sort(currentColumn, sortOrder);
    model->select();

    // Обновляем отображение
    tableView->horizontalHeader()->setSortIndicator(currentColumn, sortOrder);
}

void TariffWidget::addTariff()
{
    TariffDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        model->select();
    }
    refreshData();
}

void TariffWidget::editTariff()
{
    QModelIndexList selection = tableView->selectionModel()->selectedRows();
    if (selection.count() == 0) {
        QMessageBox::warning(this, tr("Редактирование тарифа"), tr("Пожалуйста, выберите тариф для редактирования."));
        return;
    }
    int row = selection.at(0).row();
    int id = model->data(model->index(row, model->fieldIndex("id"))).toInt();

    TariffDialog dialog(this);
    dialog.loadTariff(id);
    if (dialog.exec() == QDialog::Accepted) {
        model->select();
    }
     refreshData();
}

void TariffWidget::deleteTariff()
{
    QModelIndexList selection = tableView->selectionModel()->selectedRows();
    if (selection.count() == 0) {
        QMessageBox::warning(this, tr("Удаление тарифа"), tr("Пожалуйста, выберите тариф для удаления."));
        return;
    }
    int row = selection.at(0).row();

    int ret = QMessageBox::warning(this, tr("Удаление тарифа"),
                                   tr("Вы уверены, что хотите удалить выбранный тариф?"),
                                   QMessageBox::Yes | QMessageBox::No);
    if (ret == QMessageBox::Yes) {
        if (!model->removeRow(row)) {
            QMessageBox::warning(this, tr("Удаление тарифа"), tr("Не удалось удалить тариф: %1").arg(model->lastError().text()));
        } else {
            model->submitAll();
        }
    }
     refreshData();
}

void TariffWidget::refreshData()
{
    model->select();
}

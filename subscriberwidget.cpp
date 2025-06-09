#include "subscriberwidget.h"
#include "subscriberdialog.h"

#include <QTableView>
#include <QHeaderView>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSqlRelationalDelegate>
#include <QtSql/QSqlError>
#include <QMessageBox>
#include <QKeyEvent>

SubscriberWidget::SubscriberWidget(QWidget *parent)
    : QWidget(parent)
{
    model = new QSqlRelationalTableModel(this);
    model->setTable("subscriber");
    model->setRelation(model->fieldIndex("tariff_id"), QSqlRelation("tariff", "id", "name"));
    model->setRelation(model->fieldIndex("employee_id"), QSqlRelation("employee", "id", "name")); // Новое отношение
    model->select();

    model->setHeaderData(model->fieldIndex("id"), Qt::Horizontal, tr("ID"));
    model->setHeaderData(model->fieldIndex("name"), Qt::Horizontal, tr("ФИО"));
    model->setHeaderData(model->fieldIndex("tariff_name_3"), Qt::Horizontal, tr("Тариф"));
    model->setHeaderData(model->fieldIndex("contact_info"), Qt::Horizontal, tr("Контакты"));
    model->setHeaderData(model->fieldIndex("contract_date"), Qt::Horizontal, tr("Дата договора"));
    model->setHeaderData(model->fieldIndex("employee_name_2"), Qt::Horizontal, tr("Сотрудник"));


    tableView = new QTableView(this);
    tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    tableView->setModel(model);
    tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableView->hideColumn(model->fieldIndex("id"));
    tableView->setItemDelegate(new QSqlRelationalDelegate(tableView));
    tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    tableView->resizeColumnsToContents();

    // Добавлено: включение сортировки
    tableView->setSortingEnabled(true);
    tableView->installEventFilter(this);

    addButton = new QPushButton(tr("Добавить"), this);
    editButton = new QPushButton(tr("Редактировать"), this);
    deleteButton = new QPushButton(tr("Удалить"), this);
    //refreshButton = new QPushButton(tr("Обновить"), this);

    connect(addButton, &QPushButton::clicked, this, &SubscriberWidget::addSubscriber);
    connect(editButton, &QPushButton::clicked, this, &SubscriberWidget::editSubscriber);
    connect(deleteButton, &QPushButton::clicked, this, &SubscriberWidget::deleteSubscriber);
   // connect(refreshButton, &QPushButton::clicked, this, &SubscriberWidget::refreshData);

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

bool SubscriberWidget::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == tableView && event->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
        switch (keyEvent->key()) {
        case Qt::Key_Return:
        case Qt::Key_Enter:
            editSubscriber();
            return true; // Событие обработано
        case Qt::Key_Delete:
            deleteSubscriber();
            return true;
        case Qt::Key_Insert:
            addSubscriber();
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

void SubscriberWidget::sortTable()
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

void SubscriberWidget::addSubscriber()
{
    SubscriberDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        model->select();
    }
     refreshData();
}

void SubscriberWidget::editSubscriber()
{
    QModelIndexList selection = tableView->selectionModel()->selectedRows();
    if (selection.count() == 0) {
        QMessageBox::warning(this, tr("Редактирование абонента"), tr("Пожалуйста, выберите абонента для редактирования."));
        return;
    }
    int row = selection.at(0).row();
    int id = model->data(model->index(row, model->fieldIndex("id"))).toInt();

    SubscriberDialog dialog(this);
    dialog.loadSubscriber(id);
    if (dialog.exec() == QDialog::Accepted) {
        model->select();
    }
     refreshData();
}

void SubscriberWidget::deleteSubscriber()
{
    QModelIndexList selection = tableView->selectionModel()->selectedRows();
    if (selection.count() == 0) {
        QMessageBox::warning(this, tr("Удаление абонента"), tr("Пожалуйста, выберите абонента для удаления."));
        return;
    }
    int row = selection.at(0).row();

    int ret = QMessageBox::warning(this, tr("Удаление абонента"),
                                   tr("Вы уверены, что хотите удалить выбранного абонента?"),
                                   QMessageBox::Yes | QMessageBox::No);
    if (ret == QMessageBox::Yes) {
        if (!model->removeRow(row)) {
            QMessageBox::warning(this, tr("Удаление абонента"), tr("Не удалось удалить абонента: %1").arg(model->lastError().text()));
        } else {
            model->submitAll();
        }
    }
     refreshData();
}

void SubscriberWidget::refreshData()
{
    model->select();
}

#include "employeewidget.h"
#include "employeedialog.h"

#include <QTableView>
#include <QHeaderView>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSqlRelationalDelegate>
#include <QtSql/QSqlError>
#include <QMessageBox>
#include <QKeyEvent>

EmployeeWidget::EmployeeWidget(QWidget *parent)
    : QWidget(parent)
{
    model = new QSqlRelationalTableModel(this);
    model->setTable("employee");
    model->setRelation(model->fieldIndex("position_id"), QSqlRelation("position", "id", "position_name"));
    model->setRelation(model->fieldIndex("office_id"), QSqlRelation("office", "id", "name"));
    model->select();

    model->setHeaderData(model->fieldIndex("id"), Qt::Horizontal, tr("ID"));
    model->setHeaderData(model->fieldIndex("name"), Qt::Horizontal, tr("ФИО"));
    model->setHeaderData(model->fieldIndex("position_name"), Qt::Horizontal, tr("Должность"));
    model->setHeaderData(model->fieldIndex("office_name_2"), Qt::Horizontal, tr("Офис"));
    model->setHeaderData(model->fieldIndex("contact_info"), Qt::Horizontal, tr("Контакты"));
    tableView = new QTableView(this);
    tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableView->setModel(model);
    tableView->hideColumn(model->fieldIndex("id"));
   // tableView->setItemDelegate(new QItemDelegate(tableView));
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
  //  refreshButton = new QPushButton(tr("Обновить"), this);

    connect(addButton, &QPushButton::clicked, this, &EmployeeWidget::addEmployee);
    connect(editButton, &QPushButton::clicked, this, &EmployeeWidget::editEmployee);
    connect(deleteButton, &QPushButton::clicked, this, &EmployeeWidget::deleteEmployee);
  //  connect(refreshButton, &QPushButton::clicked, this, &EmployeeWidget::refreshData);

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

bool EmployeeWidget::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == tableView && event->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
        switch (keyEvent->key()) {
        case Qt::Key_Return:
        case Qt::Key_Enter:
            editEmployee();
            return true; // Событие обработано
        case Qt::Key_Delete:
            deleteEmployee();
            return true;
        case Qt::Key_Insert:
            addEmployee();
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

void EmployeeWidget::sortTable()
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

void EmployeeWidget::addEmployee()
{
    EmployeeDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        model->select();
    }
     refreshData();
}

void EmployeeWidget::editEmployee()
{
    QModelIndexList selection = tableView->selectionModel()->selectedRows();
    if (selection.count() == 0) {
        QMessageBox::warning(this, tr("Редактирование сотрудника"), tr("Пожалуйста, выберите сотрудника для редактирования."));
        return;
    }
    int row = selection.at(0).row();
    int id = model->data(model->index(row, model->fieldIndex("id"))).toInt();

    EmployeeDialog dialog(this);
    dialog.loadEmployee(id);
    if (dialog.exec() == QDialog::Accepted) {
        model->select();
    }
     refreshData();
}

void EmployeeWidget::deleteEmployee()
{
    QModelIndexList selection = tableView->selectionModel()->selectedRows();
    if (selection.count() == 0) {
        QMessageBox::warning(this, tr("Удаление сотрудника"), tr("Пожалуйста, выберите сотрудника для удаления."));
        return;
    }
    int row = selection.at(0).row();

    int ret = QMessageBox::warning(this, tr("Удаление сотрудника"),
                                   tr("Вы уверены, что хотите удалить выбранного сотрудника?"),
                                   QMessageBox::Yes | QMessageBox::No);
    if (ret == QMessageBox::Yes) {
        if (!model->removeRow(row)) {
            QMessageBox::warning(this, tr("Удаление сотрудника"), tr("Не удалось удалить сотрудника: %1").arg(model->lastError().text()));
        } else {
            model->submitAll();
        }
    }
     refreshData();
}

void EmployeeWidget::refreshData()
{
    model->select();
}

#include "countersview.h"

#include <QtWidgets>
#include <QLayout>
#include <QTableView>
#include <QPushButton>
#include <QLabel>

CountersView::CountersView(QWidget* parent) :
    QMainWindow(parent) {

    setWindowTitle("Counters View");

    QVBoxLayout* pMainLayout = new QVBoxLayout();
    QWidget* centralWidget = new QWidget();

    {
        QHBoxLayout* pLayout = new QHBoxLayout();
        m_lblFrequency = new QLabel();
        m_lblFrequency->setText("0");
        pLayout->addWidget(new QLabel("Частота:"));
        pLayout->addSpacing(10);
        pLayout->addWidget(m_lblFrequency);
        pLayout->addStretch(); 
        pMainLayout->addItem(pLayout);
    }

    {
        m_tvDataGrid = new QTableView();
        m_tvDataGrid->horizontalHeader()->hide();
        m_tvDataGrid->horizontalHeader()->setStretchLastSection(true);
        m_tvDataGrid->verticalHeader()->hide();
        m_tvDataGrid->setSelectionBehavior(QAbstractItemView::SelectRows);
        m_tvDataGrid->setSelectionMode(QAbstractItemView::SingleSelection);
        pMainLayout->addWidget(m_tvDataGrid);
    }

    {
        QHBoxLayout* pLayout = new QHBoxLayout();
        m_btnAddSensor = new QPushButton("Добавить");
        connect(m_btnAddSensor, &QPushButton::clicked, this, &CountersView::addCounter);

        m_btnRemoveSensor = new QPushButton("Удалить");
        m_btnRemoveSensor->setEnabled(false);
        connect(m_btnRemoveSensor, &QPushButton::clicked, this, &CountersView::removeButtonPressed);

        m_btnSaveSensors = new QPushButton("Сохранить");
        connect(m_btnSaveSensors, &QPushButton::clicked, this, &CountersView::saveCounters);

        pLayout->addStretch();
        pLayout->addWidget(m_btnAddSensor);
        pLayout->addWidget(m_btnRemoveSensor);
        pLayout->addWidget(m_btnSaveSensors);

        pMainLayout->addItem(pLayout);
    }

    centralWidget->setLayout(pMainLayout);
    setCentralWidget(centralWidget);
}

CountersView::~CountersView() {
}

void CountersView::frequencyChanged(unsigned long freq) {
    m_lblFrequency->setText(QString::number(freq));
}

void CountersView::removeButtonPressed() {
    QModelIndexList selection = m_tvDataGrid->selectionModel()->selectedRows();
    if (!selection.count())
        return;

    QModelIndex index = selection.at(0);
    m_btnRemoveSensor->setEnabled(false);
    m_tvDataGrid->selectionModel()->clearSelection();

    emit removeCounter(index.row());
}

void CountersView::setCountersDataModel(QAbstractItemModel* pModel) {
    m_tvDataGrid->setModel(pModel);
    connect(m_tvDataGrid->selectionModel(), &QItemSelectionModel::selectionChanged, [this]() {
        m_btnRemoveSensor->setEnabled(m_tvDataGrid->selectionModel()->selectedIndexes().size());
        });
}


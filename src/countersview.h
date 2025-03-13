#ifndef COUNTERSVIEW_H
#define COUNTERSVIEW_H

#include <QMainWindow>

class QTableView;
class QLabel;
class QPushButton;
class QAbstractItemModel;


class CountersView : public QMainWindow {
    Q_OBJECT
public:
    CountersView(QWidget* parent = nullptr);
    virtual ~CountersView();

    void	setCountersDataModel(QAbstractItemModel* pModel);

signals:

    void	addCounter();
    void	removeCounter(int nIndex);
    void	saveCounters();

public slots:

    void	frequencyChanged(unsigned long freq);
    void	removeButtonPressed();

protected:

    QTableView* m_tvDataGrid        = nullptr;
    QLabel*     m_lblFrequency      = nullptr;

    QPushButton* m_btnAddSensor     = nullptr;
    QPushButton* m_btnRemoveSensor  = nullptr;
    QPushButton* m_btnSaveSensors   = nullptr;
};

#endif 


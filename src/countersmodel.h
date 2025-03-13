#ifndef COUNTERSMODEL_H
#define COUNTERSMODEL_H

#include <QAbstractTableModel>
#include <QElapsedTimer>

#include <mutex>
#include <vector> 

#include "mutexlocker.h"

class CountersModel : public QAbstractTableModel {
    Q_OBJECT

    Q_PROPERTY(unsigned long Frequency READ frequency NOTIFY frequencyChanged);

public:
    CountersModel(QObject* parent = nullptr);
    virtual ~CountersModel();
    
    int		rowCount(const QModelIndex& parent = QModelIndex()) const       { Q_UNUSED(parent); return m_nRows; };
    int		columnCount(const QModelIndex& parent = QModelIndex()) const    { Q_UNUSED(parent); return 1; };

    QVariant	data(const QModelIndex& index, int role = Qt::DisplayRole) const;

    void		startIncrementThread();
    void		stopIncrementThread();
    void		incrementCurrentCounters();

    unsigned long	frequency() const { return m_frequency; };

    std::vector<unsigned int>   counters() const;
    void                        setCounters(const std::vector<unsigned int>& data);

public slots:

    void		appendCounter();
    void		removeCounter(int nIndex);

signals:
        
    void frequencyChanged(unsigned long);
    
protected:

    void timerEvent(QTimerEvent* event);

protected:

    int                             m_viewTimer         = -1;
    QElapsedTimer                   m_frequencyTimer;

    std::atomic<bool>               m_bExitFlag         = false;
    std::thread                     m_incrementThread;

    unsigned long                   m_frequency         = 0;
    unsigned long                   m_previousTotal     = 0;
    std::atomic<unsigned long>      m_currentTotal      = 0;

    std::vector<unsigned int>       m_counters;	
    mutable std::recursive_mutex    m_countersLock;

    int                             m_nRows             = 0;	
};
#endif


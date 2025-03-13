#include "countersmodel.h"

#include <QTimerEvent>
#include <QThread>
#include <QDateTime>

CountersModel::CountersModel(QObject* parent) :
    QAbstractTableModel(parent) {

    m_viewTimer = startTimer(500);
    m_frequencyTimer.start();
}

CountersModel::~CountersModel() {
}

void CountersModel::timerEvent(QTimerEvent* event) {
    if (event->timerId() == m_viewTimer) {
        if (m_nRows == 0) {
            m_frequency = 0;
            m_previousTotal = 0;
        }
        else {
            if (m_previousTotal) {
                m_frequency = (m_currentTotal - m_previousTotal) * 1000. / m_frequencyTimer.elapsed();
            }

            m_previousTotal = m_currentTotal;
            emit dataChanged(index(0, 0), index(rowCount() - 1, 0));
        }
        m_frequencyTimer.restart();
        emit frequencyChanged(m_frequency);
    }
}

QVariant CountersModel::data(const QModelIndex& index, int role) const {
    if (role == Qt::DisplayRole) {
        if (index.column() == 1) {
            return QVariant();
        }

        MutexLocker<std::recursive_mutex>	lock(m_countersLock);
        if (m_counters.size() <= index.row()) {
            return QVariant();
        }

        return QVariant(m_counters[index.row()]);
    }
    else if (role == Qt::TextAlignmentRole) {
        return QVariant(Qt::AlignRight | Qt::AlignVCenter);
    }

    return QVariant();
}

void		CountersModel::startIncrementThread() {
    m_incrementThread = std::thread(&CountersModel::incrementCurrentCounters, this);
}

void		CountersModel::stopIncrementThread() {
    m_bExitFlag = true;
    m_incrementThread.join();
}

void CountersModel::incrementCurrentCounters() {
    while (!m_bExitFlag) {
        MutexLocker<std::recursive_mutex>	lock(m_countersLock);

        unsigned long count = 0;
        for (auto i = m_counters.begin(), e = m_counters.end(); i != e; ++i) {
            (*i)++;
            count += (*i);
        }
        m_currentTotal = count;
    }
}

void		CountersModel::appendCounter() {
    MutexLocker<std::recursive_mutex>	lock(m_countersLock);
    m_counters.push_back(0);

    m_nRows = (int)m_counters.size();
    m_previousTotal = 0;
    m_currentTotal = 0;

    emit layoutChanged();
}

void		CountersModel::removeCounter(int nIndex) {
    MutexLocker<std::recursive_mutex>	lock(m_countersLock);
    if (m_counters.size() <= nIndex)
        return;

    m_counters.erase(m_counters.begin() + nIndex);

    m_previousTotal = 0;
    m_currentTotal = 0;
    m_nRows = (int)m_counters.size();

    emit layoutChanged();
}

std::vector<unsigned int>	CountersModel::counters() const {
    MutexLocker<std::recursive_mutex>	lock(m_countersLock);
    return m_counters;
}

void	CountersModel::setCounters(const std::vector<unsigned int>& data) {
    MutexLocker<std::recursive_mutex>	lock(m_countersLock);

    m_counters = data;
    m_previousTotal = 0;
    m_currentTotal = 0;
    m_nRows = (int)m_counters.size();

    emit layoutChanged();
}
#ifndef MUTEXLOCKER_H
#define MUTEXLOCKER_H

template <typename T> class   MutexLocker
{
public:
    MutexLocker(T& m) :m_mutex(m) { m_mutex.lock(); }
    ~MutexLocker() { m_mutex.unlock(); }

protected:

    T& m_mutex;

    MutexLocker(const MutexLocker& other) = delete;
    MutexLocker(MutexLocker&& other) = delete;
    MutexLocker& operator=(const MutexLocker&) = delete;
    MutexLocker& operator=(MutexLocker&&) = delete;
};

#endif
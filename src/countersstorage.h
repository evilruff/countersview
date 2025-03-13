#ifndef COUNTERSSTORAGE_H
#define COUNTERSSTORAGE_H

#include <QSqlDatabase>

#include <vector>

class CountersStorage {

public:

    CountersStorage() = default;
    ~CountersStorage() = default;

    void							connect();
    void							disconnect();
    std::vector<unsigned int>		load();
    void							save(const std::vector<unsigned int>& data);

protected:

    QSqlDatabase		m_db;
};

#endif
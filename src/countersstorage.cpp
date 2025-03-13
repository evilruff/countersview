#include "countersstorage.h"

#include <QSqlQuery>

void							CountersStorage::connect() {
    m_db = QSqlDatabase::addDatabase("QSQLITE", "SQLITE");
    QString dbName = "counters.sqlite";
    m_db.setDatabaseName(dbName);
    m_db.open();

    QString queryString = "CREATE TABLE IF NOT EXISTS counters(value INTEGER)";
    QSqlQuery	query(m_db);
    query.exec(queryString);

}

std::vector<unsigned int>		CountersStorage::load() {
    QString queryString = "SELECT * FROM counters";
    QSqlQuery	query(m_db);
    query.exec(queryString);

    std::vector<unsigned int>   items;
    while (query.next()) {
        items.push_back(query.value(0).toInt());
    }

    return items;
}

void	CountersStorage::save(const std::vector<unsigned int>& data) {
    QString queryString = "DELETE FROM counters";
    {
        QSqlQuery	query(m_db);
        query.exec(queryString);
    }

    queryString = "INSERT INTO counters VALUES(?)";
    QSqlQuery	query(m_db);
    query.prepare(queryString);

    for (unsigned int v : data) {
        query.bindValue(0, v);
        query.exec();
    }
}

void	CountersStorage::disconnect() {
    m_db.close();
}

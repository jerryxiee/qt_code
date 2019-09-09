#ifndef SQLITEDATEBASE_H
#define SQLITEDATEBASE_H

#include <QSqlDatabase>
#include <QSqlQueryModel>
#include <QSqlError>
#include <QSqlQuery>
#include <QString>
#include <QList>

class SqliteDateBase
{
public:
    SqliteDateBase();
    virtual ~SqliteDateBase();

    bool open(const QString dbname = "/mnt/sda1/database.db");

    bool isTabExists(const QString &tabname);
    bool deleteTable(const QString &tabname);
    int countInTab(const QString &tabname);

    void close();


    const QString LOGDBTAB = "logtab";

private:

    QSqlDatabase mDataBase;
};

#endif // SQLITEDATEBASE_H

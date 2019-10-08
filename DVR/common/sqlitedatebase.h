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
#ifndef LUNUX_WIN
    #define DATEBASENAME "/mnt/sda1/database.db"
#else
    #define DATEBASENAME "/home/abhw/nfsroot/dvr_config/database.db"
#endif

    bool open(const QString dbname = DATEBASENAME);
    bool createTable(const QString &tabname,QString &tabinfo);
    bool isTabExists(const QString &tabname);
    bool deleteTable(const QString &tabname);
    int countInTab(const QString &tabname);
    bool getTabData(QSqlQuery &query,const QString &tabname,QString &where);
    bool updateTabData(const QString &tabname,QString &setvalue,QString &where);

    void close();



private:
//    const QString DATEBASENAME = "/mnt/sda1/database.db";

    QSqlDatabase mDataBase;
};

#endif // SQLITEDATEBASE_H

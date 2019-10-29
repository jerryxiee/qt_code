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
    static SqliteDateBase *getSqliteDateBase();
    virtual ~SqliteDateBase();
#ifndef LUNUX_WIN
    #define DATEBASENAME "/mnt/sda1/database.db"
#else
    #define DATEBASENAME "/home/abhw/nfsroot/dvr_config/database.db"
#endif
    bool isOpen();
    bool open(const QString dbname = DATEBASENAME);
    bool createTable(const QString &tabname,QString &tabinfo);
    bool isTabExists(const QString &tabname);
    bool deleteTable(const QString &tabname);
    int countInTab(const QString &tabname);
    bool getTabData(QSqlQuery &query,const QString &tabname,QString &where);
    bool updateTabData(const QString &tabname,QString &setvalue,QString &where);
    bool deleteTabData(const QString &tabname,const QString where);

    void close();

protected:
    SqliteDateBase();

private:
//    const QString DATEBASENAME = "/mnt/sda1/database.db";
    static SqliteDateBase * mGlobDataBase;
    QSqlDatabase mDataBase;
};

#endif // SQLITEDATEBASE_H

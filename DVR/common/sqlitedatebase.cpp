#include "sqlitedatebase.h"
#include <QDebug>

SqliteDateBase::SqliteDateBase()
{

}

SqliteDateBase::~SqliteDateBase()
{
    close();

}

bool SqliteDateBase::open(const QString dbname)
{
    if(mDataBase.isOpen()){
        qDebug()<<"database have opened";
        return true;
    }
    mDataBase = QSqlDatabase::addDatabase("QSQLITE");

#ifndef LUNUX_WIN
    mDataBase.setDatabaseName(dbname);

#else
    mDataBase.setDatabaseName("database.db");
#endif

    return mDataBase.open();
}

bool SqliteDateBase::isTabExists(const QString &tabname)
{
    return mDataBase.tables().contains(tabname);
}

bool SqliteDateBase::deleteTable(const QString &tabname)
{
    QSqlQuery query;
    QString str = "DROP TABLE "+tabname;

    return query.exec(str);
}

void SqliteDateBase::close()
{
    if(mDataBase.isOpen())
        mDataBase.close();
}

int SqliteDateBase::countInTab(const QString &tabname)
{
    QSqlQuery query(mDataBase);
    query.exec("select * from "+tabname);
    QSqlQueryModel *model = new QSqlQueryModel();
    model->setQuery(query);
    while (model->canFetchMore()) {
        model->fetchMore();
    }
    int nRecordCount = model->rowCount();
    qDebug() << nRecordCount;
    return nRecordCount;
}


#include "sqlitedatebase.h"
#include <QDebug>

SqliteDateBase * SqliteDateBase::mGlobDataBase = nullptr;

SqliteDateBase *SqliteDateBase::getSqliteDateBase()
{
    if(!mGlobDataBase){
        mGlobDataBase = new SqliteDateBase();
        if(mGlobDataBase){
            if(!mGlobDataBase->open()){
                mGlobDataBase->close();
                delete mGlobDataBase;
                mGlobDataBase = nullptr;
            }
        }
    }

    return mGlobDataBase;
}

SqliteDateBase::SqliteDateBase()
{

}

SqliteDateBase::~SqliteDateBase()
{
    close();

}

bool SqliteDateBase::isOpen()
{
    if(mDataBase.isOpen()){
        qDebug()<<"database have opened";
        return true;
    }

    return false;
}

bool SqliteDateBase::open(const QString dbname)
{
    if(mDataBase.isOpen()){
        qDebug()<<"database have opened";
        return true;
    }
    mDataBase = QSqlDatabase::addDatabase("QSQLITE");

    mDataBase.setDatabaseName(dbname);


    return mDataBase.open();
}

bool SqliteDateBase::createTable(const QString &tabname, QString &tabinfo)
{
    QSqlQuery query;

    QString str = "CREATE TABLE "+tabname+tabinfo;

    if(isTabExists(tabname)){
        qDebug()<<tabname<<" has exist";
        return true;
    }

    if(query.exec(str)){
        return true;
    }

    qDebug()<<query.lastError();

    return false;

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
//    qDebug() << nRecordCount;
    return nRecordCount;
}

bool SqliteDateBase::getTabData(QSqlQuery &query,const QString &tabname,QString &where)
{
    query.exec("SELECT * FROM "+tabname+" where "+where);

    if(!query.exec()){
        qDebug()<<query.lastError();
        qDebug()<<"get data error"<<tabname+" "<<where;
        return false;
    }

    return true;
}

bool SqliteDateBase::updateTabData(const QString &tabname,QString &setvalue,QString &where)
{
    QSqlQuery query;
    query.prepare("UPDATE "+tabname+" SET"+setvalue+" where "+where);
    if(!query.exec()){
        qDebug()<<query.lastError();
        return false;
    }

    return true;
}

bool SqliteDateBase::deleteTabData(const QString &tabname,const QString where)
{
    QSqlQuery query;
    query.prepare("DELETE FROM "+tabname+" where "+where);
    if(!query.exec()){
        qDebug()<<query.lastError();
        return false;
    }

    return true;
}


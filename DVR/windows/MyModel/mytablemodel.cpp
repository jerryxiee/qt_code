#include "mytablemodel.h"
#include <QDebug>
#include <QDateTimeEdit>
//#include <QThread>

MyTableModel::MyTableModel(QObject *parent):
    QAbstractTableModel(parent)
{

#ifndef LUNUX_WIN
    QDir rootDir("/mnt/sda1/venc/");
#else
    QDir rootDir("/home/abhw");
#endif

    mCurrentPath = rootDir.absolutePath();
    QStringList string;
    string<<"*";
    QFileInfoList list = rootDir.entryInfoList(string,QDir::Files | QDir::AllEntries | QDir::NoDotAndDotDot);
    showFileInfoList(list);
}

MyTableModel::~MyTableModel()
{
    qDebug()<<"~MyTableModel exit";
}

void MyTableModel::onShowSlot(QDir dir)
{
    QStringList string;
    string<<"*";
    QFileInfoList list = dir.entryInfoList(string, QDir::Files | QDir::AllEntries | QDir::NoDotAndDotDot , QDir::DirsFirst);
    showFileInfoList(list);
}

void MyTableModel::showFileInfoList(QFileInfoList list)
{
    m_data.clear();
    qDebug()<<"list.count ="<<list.count();
    for(unsigned int i=0; i<list.count() ;i++){
        QFileInfo tmpFileInfo = list.at(i);
        QString fileName = tmpFileInfo.fileName();
        QVariantList list1;
        list1.push_back(fileName);

        QDateTime time = tmpFileInfo.lastModified();
        list1.push_back(time.toString("yyyy-MM-dd-hh-mm-ss"));

        const qint64 size = tmpFileInfo.size();
        list1.push_back(tr("%1 KB").arg(int((size + 1023) / 1024)));

        m_data.push_back(list1);
    }
    refrushModel();
}

void MyTableModel::onDirShowSlot(QString &filename)
{
    qDebug()<<"enter slotDirShow";
    QDir dir;
    dir.setPath(mCurrentPath);
    mPath.push(mCurrentPath);
    dir.cd(filename);
    mCurrentPath = dir.absolutePath();

    emit pathChanged();
    onShowSlot(dir);
}

void MyTableModel::onBackButtonClickedSlot()
{
    QDir dir;

    if(!mPath.isEmpty()){
        mCurrentPath = mPath.pop();
        dir.setPath(mCurrentPath);
        onShowSlot(dir);
        emit pathChanged();
         qDebug()<<"mCurrentPath"<<mCurrentPath;
    }

}

void MyTableModel::oncellDoubleClickedSlot(int row,int column)
{
    QString text = m_data[row].at(column).toString();
    QString filename = mCurrentPath+"/"+text;
    QFileInfo fileinfo(filename);

    if(fileinfo.isFile()){
        qDebug()<<"display:"<<filename;
        mFileName = filename;
        emit fileNameChanged();
    }else{
        onDirShowSlot(text);
    }

    qDebug()<<"testSlot"<<filename;
}

QString MyTableModel::name() const
{
    return mFileName;
}
QString MyTableModel::pathname() const
{
    return mCurrentPath;
}

void MyTableModel::refrushModel()
{
     beginResetModel();
     endResetModel();

//     emit updateCount(this->rowCount(QModelIndex()));
 }

int MyTableModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_data.size();
}

int MyTableModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    if(m_data.isEmpty())
        return 0;
    return m_data.at(0).size();
}


QVariant MyTableModel::data(const QModelIndex &index, int role) const
{
//    qDebug(">>>>>>>%s%d",__FUNCTION__,__LINE__);
//    qDebug()<<role;
    if (role == Qt::DisplayRole) {
        return m_data[index.row()].at(index.column());
    }
    else
    {
        int columnIdx = role - Qt::UserRole - 1;
        QModelIndex modelIndex = this->index(index.row(), columnIdx);
        return m_data[modelIndex.row()].at(modelIndex.column());
    }
    return QVariant();
}

QStringList MyTableModel::roles() const
{
//    qDebug(">>>>>>>%s%d",__FUNCTION__,__LINE__);
    return m_roleList;
}


void MyTableModel::setRoles(const QStringList roles)
{
//    qDebug(">>>>>>>%s%d",__FUNCTION__,__LINE__);
    if(!roles.isEmpty())
    {
        m_roleList = roles;
    }
}

QHash<int, QByteArray> MyTableModel::roleNames() const
{
//    qDebug(">>>>>>>%s%d",__FUNCTION__,__LINE__);
    QHash<int, QByteArray> roles;
    for(int i = 0;i < m_roleList.size();i++)
    {
        roles[Qt::UserRole+i+1] = m_roleList.at(i).toLocal8Bit();
    }
    return roles;
}


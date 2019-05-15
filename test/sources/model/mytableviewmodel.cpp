#include "mytableviewmodel.h"
#include <QDebug>
#include <QDateTimeEdit>

//MyTableViewModel::MyTableViewModel(QObject *parent) : QObject(parent)
//{

//}


MyTableViewModel::MyTableViewModel(QObject *parent):
    QAbstractTableModel(parent)
{
//    QVariantList list1;
//    list1.push_back("ceshi1");
//    list1.push_back("nan");
//    list1.push_back("nan");
//    QVariantList list2;
//    list2.push_back("ceshi2");
//    list2.push_back("nv");
//    list2.push_back("nv");
//    QVariantList list3;
//    list3.push_back("na");
//    list3.push_back("na");
//    list3.push_back("na");

//    m_data.push_back(list1);
//    m_data.push_back(list2);
//    m_data.push_back(list3);

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

MyTableViewModel::~MyTableViewModel()
{
    qDebug()<<"exit ~MyTableViewModel";

}

void MyTableViewModel::onShowSlot(QDir dir)
{
    QStringList string;
    string<<"*";
    QFileInfoList list = dir.entryInfoList(string, QDir::Files | QDir::AllEntries | QDir::NoDotAndDotDot , QDir::DirsFirst);
    showFileInfoList(list);
}

void MyTableViewModel::showFileInfoList(QFileInfoList list)
{
//    mFileTabWidget->setRowCount(0);
//    mFileTabWidget->clearContents();
    m_data.clear();
    qDebug()<<"list.count ="<<list.count();
    for(unsigned int i=0; i<list.count() ;i++){
        QFileInfo tmpFileInfo = list.at(i);
//        if(tmpFileInfo.isDir()){
//            QIcon icon("dir.png");
            QString fileName = tmpFileInfo.fileName();
            QVariantList list1;
            list1.push_back(fileName);

//            QTableWidgetItem *name = new QTableWidgetItem(/*icon,*/ fileName);
//            name->setFlags(name->flags() ^ Qt::ItemIsEditable);
////            name->setData(absoluteFileNameRole,fileName);

            QDateTime time = tmpFileInfo.lastModified();
            list1.push_back(time.toString("yyyy-MM-dd-hh-mm-ss"));
//            QTableWidgetItem *modfilytime = new QTableWidgetItem(/*icon,*/ time.toString("yyyy-MM-dd-hh-mm-ss"));
//            modfilytime->setFlags(modfilytime->flags() ^ Qt::ItemIsEditable);

            const qint64 size = tmpFileInfo.size();
            list1.push_back(tr("%1 KB").arg(int((size + 1023) / 1024)));

            m_data.push_back(list1);

//            QTableWidgetItem *sizeItem = new QTableWidgetItem(tr("%1 KB")
//                                                 .arg(int((size + 1023) / 1024)));
//            sizeItem->setFlags(sizeItem->flags() ^ Qt::ItemIsEditable);

//            int row = mFileTabWidget->rowCount();
//            mFileTabWidget->insertRow(row);
//            mFileTabWidget->setItem(row, 0, name);
//            mFileTabWidget->setItem(row, 1, modfilytime);
//            mFileTabWidget->setItem(row, 2, sizeItem);
//        }
//        else if(tmpFileInfo.isFile()){
////            QIcon icon("file.png");
//            QString fileName = tmpFileInfo.fileName();
//            QListWidgetItem *tmp = new QListWidgetItem(/*icon,*/ fileName);
//            fileListWidget->addItem(tmp);
//        }
    }
    refrushModel();

}

void MyTableViewModel::onDirShowSlot(QString &filename)
{
    qDebug()<<"enter slotDirShow";
//    QString str = item->text();
    QDir dir;
    dir.setPath(mCurrentPath);
    mPath.push(mCurrentPath);
    dir.cd(filename);
    mCurrentPath = dir.absolutePath();
//    mMainLab->setText(mCurrentPath);

    onShowSlot(dir);
}


void MyTableViewModel::oncellDoubleClickedSlot(int row,int column)
{
//    QTableWidgetItem * item1 = new QTableWidgetItem;
//    item1 = mFileTabWidget->item(row,0);
    QString text = m_data[row].at(column).toString();
    QString filename = mCurrentPath+"/"+text;
    QFileInfo fileinfo(filename);

    if(fileinfo.isFile()){
        qDebug()<<"display:"<<filename;
//        emit videoDispSignal(filename);
        mFileName = filename;
        emit dispVideo();
    }else{
        onDirShowSlot(text);
    }

    qDebug()<<"testSlot"<<filename;
}

QString MyTableViewModel::name() const
{
    return mFileName;
}

void MyTableViewModel::refrushModel()
{
     beginResetModel();
     endResetModel();

//     emit updateCount(this->rowCount(QModelIndex()));
 }

int MyTableViewModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_data.size();
}

int MyTableViewModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    if(m_data.isEmpty())
        return 0;
    return m_data.at(0).size();
}


QVariant MyTableViewModel::data(const QModelIndex &index, int role) const
{
    qDebug(">>>>>>>%s%d",__FUNCTION__,__LINE__);
    qDebug()<<role;
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

QStringList MyTableViewModel::roles() const
{
    qDebug(">>>>>>>%s%d",__FUNCTION__,__LINE__);
    return m_roleList;
}

void MyTableViewModel::setRoles(const QStringList roles)
{
    qDebug(">>>>>>>%s%d",__FUNCTION__,__LINE__);
    if(!roles.isEmpty())
    {
        m_roleList = roles;
    }
}

QHash<int, QByteArray> MyTableViewModel::roleNames() const
{
    qDebug(">>>>>>>%s%d",__FUNCTION__,__LINE__);
    QHash<int, QByteArray> roles;
    for(int i = 0;i < m_roleList.size();i++)
    {
        roles[Qt::UserRole+i+1] = m_roleList.at(i).toLocal8Bit();
    }
    return roles;
}



#include "mytableviewmodel.h"
#include <QDebug>

//MyTableViewModel::MyTableViewModel(QObject *parent) : QObject(parent)
//{

//}


MyTableViewModel::MyTableViewModel(QObject *parent):
    QAbstractTableModel(parent)
{
    QVariantList list1;
    list1.push_back("ceshi1");
    list1.push_back("nan");
    list1.push_back("nan");
    QVariantList list2;
    list2.push_back("ceshi2");
    list2.push_back("nv");
    list2.push_back("nv");
    QVariantList list3;
    list3.push_back("na");
    list3.push_back("na");
    list3.push_back("na");

    m_data.push_back(list1);
    m_data.push_back(list2);
    m_data.push_back(list3);
}

MyTableViewModel::~MyTableViewModel()
{

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



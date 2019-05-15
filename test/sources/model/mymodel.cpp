#include "mymodel.h"

MyModel::MyModel(QObject* parent) :QAbstractListModel(parent)
{
}
void MyModel::Add(mydata& md)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    m_datas.push_back(md);
    endInsertRows();
}
void MyModel::clear()
{ //清除rows 界面将不显示
    beginRemoveRows(QModelIndex(), 0, m_datas.size());
    //清空动态数组
    m_datas.clear();
    endRemoveRows();
    //end
}
//外部接口 QML调用 添加数据在指定行
void MyModel::minsert(int index, const QString& data1, const QString& data2)
{
    mydata d(data1, data2);
    beginInsertRows(QModelIndex(), index, index);
    m_datas.insert(m_datas.begin()+index, d); endInsertRows();
}
void MyModel::mremove(int index)
{
    beginRemoveRows(QModelIndex(), index, index);
    m_datas.erase(m_datas.begin() + index);
    endRemoveRows();
}
void MyModel::pushdata(const QString& data1, const QString& data2)
{
    mydata d(data1, data2);
    Add(d);
}
int MyModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_datas.size();
}
QVariant MyModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= m_datas.size())
    {
        return QVariant();
    }
    const mydata& d = m_datas[index.row()];
    if (role == datatype::type1) {
        return d.data1();
    } else if (role == datatype::type2) {
        return d.data2();
    } else if (role == datatype::type3) {
        return d.obj;
    }
    return QVariant();
}
//定义数据别名  QHash<int, QByteArray> 父类规定的
QHash<int, QByteArray> MyModel::roleNames() const
{
    QHash<int, QByteArray> d;
    d[datatype::type1] = "data1";
    d[datatype::type2] = "data2";
    d[datatype::type3] = "curitem";
    return d;
}


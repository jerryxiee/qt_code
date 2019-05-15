#ifndef MYTABLEVIEWMODEL_H
#define MYTABLEVIEWMODEL_H

#include <QObject>
#include <QAbstractTableModel>
#include <QDir>
#include <QStack>

class MyTableViewModel : public QAbstractTableModel
{
    Q_OBJECT
    Q_PROPERTY(QStringList roles READ roles WRITE setRoles)
    Q_PROPERTY(QString name READ name NOTIFY dispVideo)
//    Q_PROPERTY(QList<QVariantList> m_data NOTIFY )
public:
    explicit MyTableViewModel(QObject *parent = nullptr);
    ~MyTableViewModel();

    void showFileInfoList(QFileInfoList list);
    void onShowSlot(QDir dir);
    void onDirShowSlot(QString &filename);
    Q_INVOKABLE void oncellDoubleClickedSlot(int row,int column);

    QVariant data(const QModelIndex &index, int role) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QHash<int, QByteArray> roleNames() const; //重新实现roleNames()

    QStringList roles() const;
    void setRoles(const QStringList roles);
    void refrushModel();
    QString name() const;
signals:
    void dataChanged();
    void videoDispSignal(QString &);
    void dispVideo();
private:
    QList<QVariantList> m_data;
    QStringList m_roleList;
    QString mCurrentPath;
    QString mFileName;
    QStack <QString> mPath;

};

#endif // MYTABLEVIEWMODEL_H

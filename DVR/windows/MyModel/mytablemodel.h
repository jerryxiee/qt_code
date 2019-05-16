#ifndef MYTABLEMODEL_H
#define MYTABLEMODEL_H

#include <QObject>
#include <QAbstractTableModel>
#include <QFileInfoList>
#include <QDir>
#include <QStack>

class MyTableModel : public QAbstractTableModel
{
    Q_OBJECT
    Q_PROPERTY(QStringList roles READ roles WRITE setRoles)
    Q_PROPERTY(QString name READ name NOTIFY fileNameChanged)
    Q_PROPERTY(QString mCurrentPath READ pathname NOTIFY pathChanged)
public:
    explicit MyTableModel(QObject *parent = nullptr);
    ~MyTableModel();

    void showFileInfoList(QFileInfoList list);
    void onShowSlot(QDir dir);
    void onDirShowSlot(QString &filename);
    Q_INVOKABLE void oncellDoubleClickedSlot(int row,int column);
    Q_INVOKABLE void onBackButtonClickedSlot();


    QVariant data(const QModelIndex &index, int role) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QHash<int, QByteArray> roleNames() const; //重新实现roleNames()

    QStringList roles() const;
    void setRoles(const QStringList roles);
    void refrushModel();
    QString name() const;
    QString pathname() const;

signals:
    void dataChanged();
    void fileNameChanged();
    void pathChanged();
private:
    QList<QVariantList> m_data;
    QStringList m_roleList;
    QString mFileName;
    QString mCurrentPath;
    QStack <QString> mPath;

};

#endif // MYTABLEVIEWMODEL_H


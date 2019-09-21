#ifndef REMOTESCHEULER_H
#define REMOTESCHEULER_H

#include <QObject>

class RemoteScheuler : public QObject
{
    Q_OBJECT
public:
    explicit RemoteScheuler(QObject *parent = nullptr);

signals:

public slots:
};

#endif // REMOTESCHEULER_H
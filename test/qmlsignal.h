#ifndef QMLSIGNAL_H
#define QMLSIGNAL_H

#include <QObject>

class qmlSignal : public QObject
{
    Q_OBJECT
public:
    explicit qmlSignal(QObject *parent = nullptr);

signals:

public slots:
};

#endif // QMLSIGNAL_H
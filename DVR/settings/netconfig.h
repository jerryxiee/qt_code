#ifndef NETCONFIG_H
#define NETCONFIG_H

#include <QObject>

class NetConfig : public QObject
{
    Q_OBJECT
public:
    explicit NetConfig(QObject *parent = nullptr);

    void netSet(bool dhcp);
    void netAuto();



signals:

public slots:
};

#endif // NETCONFIG_H

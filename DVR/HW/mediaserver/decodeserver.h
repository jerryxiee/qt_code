#ifndef DECODESERVER_H
#define DECODESERVER_H

#include <QThread>

class DeCodeServer : public QThread
{
    Q_OBJECT
public:
    ~DeCodeServer();
    explicit DeCodeServer(QObject *parent = nullptr);

private:
    virtual void run();

signals:

public slots:
};

#endif // DECODESERVER_H

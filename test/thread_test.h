#ifndef THREAD_TEST_H
#define THREAD_TEST_H

#include <QObject>
#include <QTimer>
#include <QDebug>
#include <QThread>

class thread_test : public QThread
{
    Q_OBJECT
public:
    explicit thread_test(QObject *parent = nullptr);

protected:
    virtual void run(){while (1) {
            sleep(2);
      }
    }
public slots:
    void onMakefile(){qDebug()<<"make file ";}
private:
    QTimer *timer;

signals:

};

#endif // THREAD_TEST_H

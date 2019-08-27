#ifndef MP4PLAY_H
#define MP4PLAY_H

#include <QThread>

class MP4Play : public QThread
{
    Q_OBJECT
public:
    explicit MP4Play(QObject *parent = nullptr);
    MP4Play(const MP4Play& video);
    ~MP4Play();
    MP4Play& operator=(const MP4Play& video);

protected:
    virtual void run();

signals:
    void frameRateChangedSignal();

public slots:


private:
    bool mRun;
};

#endif // MP4PLAY_H

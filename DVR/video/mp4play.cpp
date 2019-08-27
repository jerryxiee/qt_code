#include "mp4play.h"

MP4Play::MP4Play(QObject *parent) : QThread(parent)
{

}

MP4Play::~MP4Play()
{

}

void MP4Play::run()
{
    mRun = true;

    while (mRun) {

    }

}

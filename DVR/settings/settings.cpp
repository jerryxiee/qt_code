#include "settings.h"

VencSet *Settings::mVencIni = nullptr;

Settings::Settings(QObject *parent) : QObject(parent)
{

}

VencSet *Settings::getVencIni()
{
    if(mVencIni == nullptr)
    {
        mVencIni = new VencSet();
    }

    return mVencIni;
}

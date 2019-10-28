#ifndef BCDTRANSFORM_H
#define BCDTRANSFORM_H

#include <QString>

class BCDTransform
{
public:
    BCDTransform();
    BCDTransform(int value);
    BCDTransform(const char *value,int len);

    static QByteArray toArray(const char *bcd,int len);
    static QByteArray toBcd(long value);


private:

};

#endif // BCDTRANSFORM_H

#ifndef BCDTRANSFORM_H
#define BCDTRANSFORM_H

#include <QString>

class BCDTransform
{
public:
    BCDTransform();
    BCDTransform(int value);
    BCDTransform(const char *value,int len);

    static QByteArray ChartoArray(const char *bcd,int len);
    static QByteArray toArray(const char *bcd,int len);
    static QByteArray toArray(const uint8_t *bcd,int len);
    static QByteArray toBcd(long value);
    static QByteArray toBcd(const QByteArray &value);
    static QByteArray BcdToHex(QByteArray &value);



private:

};

#endif // BCDTRANSFORM_H

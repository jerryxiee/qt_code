#include "bcdtransform.h"
#include <QStack>
#include <QDebug>

BCDTransform::BCDTransform()
{

}

QByteArray BCDTransform::ChartoArray(const char *bcd,int len)
{
    QByteArray bytearray;

    for (int i = 0;i < len;i ++) {
        bytearray.append(bcd[i]);
    }

    return bytearray;
}

QByteArray BCDTransform::toArray(const char *bcd, int len)
{
    QByteArray bytearray;

    bytearray.resize(len*2);
    for (int i = 0,j = 0;i < len;i++,j = 2*i) {
        bytearray[j] = (bcd[i]>>4) + '0';
        bytearray[j+1] = (bcd[i]&0x0f) + '0';

    }

    return bytearray;
}

QByteArray BCDTransform::toArray(const uint8_t *bcd,int len)
{
    QByteArray bytearray;

    bytearray.resize(len*2);
    for (int i = 0,j = 0;i < len;i++,j = 2*i) {
        bytearray[j] = (bcd[i]>>4) + '0';
        bytearray[j+1] = (bcd[i]&0x0f) + '0';

    }

    return bytearray;

}

QByteArray BCDTransform::toBcd(long value)
{
    QByteArray bytearray;
    QStack<char> qstack;
    int i = 0;

    char byte;

    while (value) {

        byte = value%10;
        value /=10;
        qstack.push(byte);
//        qDebug("%x",byte);
    }
    bytearray.resize((qstack.count()+1)/2);
    while (!qstack.isEmpty()) {
        byte = qstack.pop();
        byte <<= 4;
        if(qstack.isEmpty()){
            bytearray.append(byte);
            break;
        }
        byte |= qstack.pop();
        bytearray[i++] = byte;
    }

    return bytearray;
}

QByteArray BCDTransform::toBcd(const QByteArray &value)
{
    QByteArray bytearray;
    char tmp = 0x0;
    int start = 0;

    if(value.length()%2 != 0){
        tmp |= (value[0] - '0')&0x0f;
        start = 1;
        bytearray.append(tmp);
    }else {
        start = 0;
    }

    for (;start < value.length();start += 2) {
        tmp = (value[start]-'0')&0xf0 + (value[start+1]-'0')&0x0f;
        bytearray.append(tmp);
    }
    return bytearray;
}

QByteArray BCDTransform::BcdToHex(QByteArray &value)
{
    QByteArray bytearray;
    char tmp;

    for (int i = 0; i < value.length();i++) {
        tmp = (value[i]>>4) *10 + (value[i]&0x0f);
        bytearray.append(tmp);
    }

    return bytearray;
}

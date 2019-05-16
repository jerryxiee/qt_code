/****************************************************************************
** Meta object code from reading C++ file 'animationbutton.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.8)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../DVR/windows/CustomControls/animationbutton.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'animationbutton.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_AnimationButton_t {
    QByteArrayData data[22];
    char stringdata0[201];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_AnimationButton_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_AnimationButton_t qt_meta_stringdata_AnimationButton = {
    {
QT_MOC_LITERAL(0, 0, 15), // "AnimationButton"
QT_MOC_LITERAL(1, 16, 7), // "clicked"
QT_MOC_LITERAL(2, 24, 0), // ""
QT_MOC_LITERAL(3, 25, 7), // "setText"
QT_MOC_LITERAL(4, 33, 4), // "text"
QT_MOC_LITERAL(5, 38, 8), // "setImage"
QT_MOC_LITERAL(6, 47, 5), // "image"
QT_MOC_LITERAL(7, 53, 12), // "setImageSize"
QT_MOC_LITERAL(8, 66, 6), // "QSize&"
QT_MOC_LITERAL(9, 73, 4), // "size"
QT_MOC_LITERAL(10, 78, 5), // "width"
QT_MOC_LITERAL(11, 84, 6), // "height"
QT_MOC_LITERAL(12, 91, 10), // "setOpacity"
QT_MOC_LITERAL(13, 102, 7), // "opacity"
QT_MOC_LITERAL(14, 110, 11), // "setDuration"
QT_MOC_LITERAL(15, 122, 5), // "msecs"
QT_MOC_LITERAL(16, 128, 8), // "setValue"
QT_MOC_LITERAL(17, 137, 5), // "start"
QT_MOC_LITERAL(18, 143, 3), // "end"
QT_MOC_LITERAL(19, 147, 23), // "onEnterImageChangedSlot"
QT_MOC_LITERAL(20, 171, 5), // "index"
QT_MOC_LITERAL(21, 177, 23) // "onLeaveImageChangedSlot"

    },
    "AnimationButton\0clicked\0\0setText\0text\0"
    "setImage\0image\0setImageSize\0QSize&\0"
    "size\0width\0height\0setOpacity\0opacity\0"
    "setDuration\0msecs\0setValue\0start\0end\0"
    "onEnterImageChangedSlot\0index\0"
    "onLeaveImageChangedSlot"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_AnimationButton[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   64,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       3,    1,   65,    2, 0x0a /* Public */,
       5,    1,   68,    2, 0x0a /* Public */,
       7,    1,   71,    2, 0x0a /* Public */,
       7,    2,   74,    2, 0x0a /* Public */,
      12,    1,   79,    2, 0x0a /* Public */,
      14,    1,   82,    2, 0x0a /* Public */,
      16,    2,   85,    2, 0x0a /* Public */,
      19,    1,   90,    2, 0x08 /* Private */,
      21,    1,   93,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, QMetaType::QString,    4,
    QMetaType::Void, QMetaType::QString,    6,
    QMetaType::Void, 0x80000000 | 8,    9,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,   10,   11,
    QMetaType::Void, QMetaType::QReal,   13,
    QMetaType::Void, QMetaType::Int,   15,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,   17,   18,
    QMetaType::Void, QMetaType::QVariant,   20,
    QMetaType::Void, QMetaType::QVariant,   20,

       0        // eod
};

void AnimationButton::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        AnimationButton *_t = static_cast<AnimationButton *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->clicked(); break;
        case 1: _t->setText((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: _t->setImage((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 3: _t->setImageSize((*reinterpret_cast< QSize(*)>(_a[1]))); break;
        case 4: _t->setImageSize((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 5: _t->setOpacity((*reinterpret_cast< qreal(*)>(_a[1]))); break;
        case 6: _t->setDuration((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: _t->setValue((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 8: _t->onEnterImageChangedSlot((*reinterpret_cast< QVariant(*)>(_a[1]))); break;
        case 9: _t->onLeaveImageChangedSlot((*reinterpret_cast< QVariant(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            typedef void (AnimationButton::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&AnimationButton::clicked)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject AnimationButton::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_AnimationButton.data,
      qt_meta_data_AnimationButton,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *AnimationButton::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *AnimationButton::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_AnimationButton.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int AnimationButton::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 10)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 10;
    }
    return _id;
}

// SIGNAL 0
void AnimationButton::clicked()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE

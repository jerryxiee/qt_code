/****************************************************************************
** Meta object code from reading C++ file 'widget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.8)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../DVR/widget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'widget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_Widget_t {
    QByteArrayData data[26];
    char stringdata0[399];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Widget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Widget_t qt_meta_stringdata_Widget = {
    {
QT_MOC_LITERAL(0, 0, 6), // "Widget"
QT_MOC_LITERAL(1, 7, 10), // "Set_VoMode"
QT_MOC_LITERAL(2, 18, 0), // ""
QT_MOC_LITERAL(3, 19, 17), // "SAMPLE_VO_MODE_E&"
QT_MOC_LITERAL(4, 37, 8), // "enVoMode"
QT_MOC_LITERAL(5, 46, 18), // "ChnDispToWinSignal"
QT_MOC_LITERAL(6, 65, 20), // "QMap<VO_CHN,RECT_S>&"
QT_MOC_LITERAL(7, 86, 12), // "StopVoSignal"
QT_MOC_LITERAL(8, 99, 15), // "onVideoDispSlot"
QT_MOC_LITERAL(9, 115, 8), // "filepath"
QT_MOC_LITERAL(10, 124, 12), // "onHidQmlSlot"
QT_MOC_LITERAL(11, 137, 14), // "onMainMenuSlot"
QT_MOC_LITERAL(12, 152, 16), // "onOneWinShowSlot"
QT_MOC_LITERAL(13, 169, 16), // "onTwoWinShowSlot"
QT_MOC_LITERAL(14, 186, 16), // "onThrWinShowSlot"
QT_MOC_LITERAL(15, 203, 16), // "onForWinShowSlot"
QT_MOC_LITERAL(16, 220, 16), // "onFivWinShowSlot"
QT_MOC_LITERAL(17, 237, 16), // "onSixWinShowSlot"
QT_MOC_LITERAL(18, 254, 16), // "onSevWinShowSlot"
QT_MOC_LITERAL(19, 271, 16), // "onEigWinShowSlot"
QT_MOC_LITERAL(20, 288, 14), // "on1MuxModeSlot"
QT_MOC_LITERAL(21, 303, 14), // "on4MuxModeSlot"
QT_MOC_LITERAL(22, 318, 14), // "on9MuxModeSlot"
QT_MOC_LITERAL(23, 333, 21), // "onLeftButtonClickSlot"
QT_MOC_LITERAL(24, 355, 22), // "onRightButtonClickSlot"
QT_MOC_LITERAL(25, 378, 20) // "onVideoExitClickSlot"

    },
    "Widget\0Set_VoMode\0\0SAMPLE_VO_MODE_E&\0"
    "enVoMode\0ChnDispToWinSignal\0"
    "QMap<VO_CHN,RECT_S>&\0StopVoSignal\0"
    "onVideoDispSlot\0filepath\0onHidQmlSlot\0"
    "onMainMenuSlot\0onOneWinShowSlot\0"
    "onTwoWinShowSlot\0onThrWinShowSlot\0"
    "onForWinShowSlot\0onFivWinShowSlot\0"
    "onSixWinShowSlot\0onSevWinShowSlot\0"
    "onEigWinShowSlot\0on1MuxModeSlot\0"
    "on4MuxModeSlot\0on9MuxModeSlot\0"
    "onLeftButtonClickSlot\0onRightButtonClickSlot\0"
    "onVideoExitClickSlot"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Widget[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      20,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,  114,    2, 0x06 /* Public */,
       5,    1,  117,    2, 0x06 /* Public */,
       7,    0,  120,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       8,    1,  121,    2, 0x0a /* Public */,
      10,    0,  124,    2, 0x08 /* Private */,
      11,    0,  125,    2, 0x08 /* Private */,
      12,    0,  126,    2, 0x08 /* Private */,
      13,    0,  127,    2, 0x08 /* Private */,
      14,    0,  128,    2, 0x08 /* Private */,
      15,    0,  129,    2, 0x08 /* Private */,
      16,    0,  130,    2, 0x08 /* Private */,
      17,    0,  131,    2, 0x08 /* Private */,
      18,    0,  132,    2, 0x08 /* Private */,
      19,    0,  133,    2, 0x08 /* Private */,
      20,    0,  134,    2, 0x08 /* Private */,
      21,    0,  135,    2, 0x08 /* Private */,
      22,    0,  136,    2, 0x08 /* Private */,
      23,    0,  137,    2, 0x08 /* Private */,
      24,    0,  138,    2, 0x08 /* Private */,
      25,    0,  139,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, 0x80000000 | 6,    2,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, QMetaType::QString,    9,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void Widget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Widget *_t = static_cast<Widget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->Set_VoMode((*reinterpret_cast< SAMPLE_VO_MODE_E(*)>(_a[1]))); break;
        case 1: _t->ChnDispToWinSignal((*reinterpret_cast< QMap<VO_CHN,RECT_S>(*)>(_a[1]))); break;
        case 2: _t->StopVoSignal(); break;
        case 3: _t->onVideoDispSlot((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 4: _t->onHidQmlSlot(); break;
        case 5: _t->onMainMenuSlot(); break;
        case 6: _t->onOneWinShowSlot(); break;
        case 7: _t->onTwoWinShowSlot(); break;
        case 8: _t->onThrWinShowSlot(); break;
        case 9: _t->onForWinShowSlot(); break;
        case 10: _t->onFivWinShowSlot(); break;
        case 11: _t->onSixWinShowSlot(); break;
        case 12: _t->onSevWinShowSlot(); break;
        case 13: _t->onEigWinShowSlot(); break;
        case 14: _t->on1MuxModeSlot(); break;
        case 15: _t->on4MuxModeSlot(); break;
        case 16: _t->on9MuxModeSlot(); break;
        case 17: _t->onLeftButtonClickSlot(); break;
        case 18: _t->onRightButtonClickSlot(); break;
        case 19: _t->onVideoExitClickSlot(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            typedef void (Widget::*_t)(SAMPLE_VO_MODE_E & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Widget::Set_VoMode)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (Widget::*_t)(QMap<VO_CHN,RECT_S> & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Widget::ChnDispToWinSignal)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (Widget::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Widget::StopVoSignal)) {
                *result = 2;
                return;
            }
        }
    }
}

const QMetaObject Widget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_Widget.data,
      qt_meta_data_Widget,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *Widget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Widget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_Widget.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int Widget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 20)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 20;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 20)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 20;
    }
    return _id;
}

// SIGNAL 0
void Widget::Set_VoMode(SAMPLE_VO_MODE_E & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void Widget::ChnDispToWinSignal(QMap<VO_CHN,RECT_S> & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void Widget::StopVoSignal()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE

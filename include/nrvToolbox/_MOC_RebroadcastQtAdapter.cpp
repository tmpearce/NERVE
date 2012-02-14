/****************************************************************************
** Meta object code from reading C++ file 'RebroadcastQtAdapter.h'
**
** Created: Mon Dec 19 11:21:55 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "RebroadcastQtAdapter.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'RebroadcastQtAdapter.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_QtRebroadcastVoid[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      19,   18,   18,   18, 0x05,

 // slots: signature, parameters, type, tag, flags
      36,   18,   18,   18, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_QtRebroadcastVoid[] = {
    "QtRebroadcastVoid\0\0signal_toQSlot()\0"
    "slot_connectToQSignal()\0"
};

const QMetaObject QtRebroadcastVoid::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_QtRebroadcastVoid,
      qt_meta_data_QtRebroadcastVoid, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &QtRebroadcastVoid::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *QtRebroadcastVoid::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *QtRebroadcastVoid::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QtRebroadcastVoid))
        return static_cast<void*>(const_cast< QtRebroadcastVoid*>(this));
    if (!strcmp(_clname, "UsesSlots"))
        return static_cast< UsesSlots*>(const_cast< QtRebroadcastVoid*>(this));
    return QObject::qt_metacast(_clname);
}

int QtRebroadcastVoid::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: signal_toQSlot(); break;
        case 1: slot_connectToQSignal(); break;
        default: ;
        }
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void QtRebroadcastVoid::signal_toQSlot()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}
static const uint qt_meta_data_QtRebroadcastDouble[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: signature, parameters, type, tag, flags
      30,   21,   20,   20, 0x05,
      53,   21,   20,   20, 0x05,
      73,   21,   20,   20, 0x05,

 // slots: signature, parameters, type, tag, flags
     100,   94,   20,   20, 0x0a,
     130,   94,   20,   20, 0x0a,
     157,   94,   20,   20, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_QtRebroadcastDouble[] = {
    "QtRebroadcastDouble\0\0newValue\0"
    "signal_toQSlot(double)\0signal_toQSlot(int)\0"
    "signal_toQSlot(bool)\0value\0"
    "slot_connectToQSignal(double)\0"
    "slot_connectToQSignal(int)\0"
    "slot_connectToQSignal(bool)\0"
};

const QMetaObject QtRebroadcastDouble::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_QtRebroadcastDouble,
      qt_meta_data_QtRebroadcastDouble, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &QtRebroadcastDouble::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *QtRebroadcastDouble::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *QtRebroadcastDouble::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QtRebroadcastDouble))
        return static_cast<void*>(const_cast< QtRebroadcastDouble*>(this));
    if (!strcmp(_clname, "UsesSlots"))
        return static_cast< UsesSlots*>(const_cast< QtRebroadcastDouble*>(this));
    return QObject::qt_metacast(_clname);
}

int QtRebroadcastDouble::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: signal_toQSlot((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 1: signal_toQSlot((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: signal_toQSlot((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 3: slot_connectToQSignal((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 4: slot_connectToQSignal((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: slot_connectToQSignal((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void QtRebroadcastDouble::signal_toQSlot(double _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void QtRebroadcastDouble::signal_toQSlot(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void QtRebroadcastDouble::signal_toQSlot(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
static const uint qt_meta_data_QtRebroadcastString[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      31,   21,   20,   20, 0x05,

 // slots: signature, parameters, type, tag, flags
      55,   20,   20,   20, 0x0a,
      87,   20,   20,   20, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_QtRebroadcastString[] = {
    "QtRebroadcastString\0\0newString\0"
    "signal_toQSlot(QString)\0"
    "slot_connectToEditingFinished()\0"
    "slot_connectToReturnPressed()\0"
};

const QMetaObject QtRebroadcastString::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_QtRebroadcastString,
      qt_meta_data_QtRebroadcastString, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &QtRebroadcastString::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *QtRebroadcastString::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *QtRebroadcastString::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QtRebroadcastString))
        return static_cast<void*>(const_cast< QtRebroadcastString*>(this));
    if (!strcmp(_clname, "UsesSlots"))
        return static_cast< UsesSlots*>(const_cast< QtRebroadcastString*>(this));
    return QObject::qt_metacast(_clname);
}

int QtRebroadcastString::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: signal_toQSlot((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 1: slot_connectToEditingFinished(); break;
        case 2: slot_connectToReturnPressed(); break;
        default: ;
        }
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void QtRebroadcastString::signal_toQSlot(const QString & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE

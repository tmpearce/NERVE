/****************************************************************************
** Meta object code from reading C++ file 'SettingsEditor.h'
**
** Created: Mon Feb 20 16:18:47 2012
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../include/nrvApp/SettingsEditor.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'SettingsEditor.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_SettingsEditor[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

       0        // eod
};

static const char qt_meta_stringdata_SettingsEditor[] = {
    "SettingsEditor\0"
};

const QMetaObject SettingsEditor::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_SettingsEditor,
      qt_meta_data_SettingsEditor, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &SettingsEditor::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *SettingsEditor::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *SettingsEditor::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_SettingsEditor))
        return static_cast<void*>(const_cast< SettingsEditor*>(this));
    return QDialog::qt_metacast(_clname);
}

int SettingsEditor::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
static const uint qt_meta_data_Checkbox[] = {

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
      10,    9,    9,    9, 0x05,

 // slots: signature, parameters, type, tag, flags
      29,   27,    9,    9, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_Checkbox[] = {
    "Checkbox\0\0checked(QString)\0b\0"
    "wasChecked(bool)\0"
};

const QMetaObject Checkbox::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_Checkbox,
      qt_meta_data_Checkbox, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Checkbox::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Checkbox::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Checkbox::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Checkbox))
        return static_cast<void*>(const_cast< Checkbox*>(this));
    return QWidget::qt_metacast(_clname);
}

int Checkbox::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: checked((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 1: wasChecked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void Checkbox::checked(const QString & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
static const uint qt_meta_data_StartupPage[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      13,   12,   12,   12, 0x0a,
      25,   21,   12,   12, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_StartupPage[] = {
    "StartupPage\0\0apply()\0str\0"
    "selectionChanged(QString)\0"
};

const QMetaObject StartupPage::staticMetaObject = {
    { &SettingsPage::staticMetaObject, qt_meta_stringdata_StartupPage,
      qt_meta_data_StartupPage, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &StartupPage::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *StartupPage::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *StartupPage::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_StartupPage))
        return static_cast<void*>(const_cast< StartupPage*>(this));
    return SettingsPage::qt_metacast(_clname);
}

int StartupPage::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = SettingsPage::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: apply(); break;
        case 1: selectionChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 2;
    }
    return _id;
}
QT_END_MOC_NAMESPACE

/****************************************************************************
** Meta object code from reading C++ file 'TaskAdapterGui.h'
**
** Created: Mon Dec 19 11:21:55 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../include/nrvApp/TaskAdapterGui.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'TaskAdapterGui.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_PreviewWindow[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      15,   14,   14,   14, 0x05,

 // slots: signature, parameters, type, tag, flags
      34,   14,   14,   14, 0x0a,
      42,   14,   14,   14, 0x0a,
      49,   14,   14,   14, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_PreviewWindow[] = {
    "PreviewWindow\0\0stringSet(QString)\0"
    "apply()\0save()\0load()\0"
};

const QMetaObject PreviewWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_PreviewWindow,
      qt_meta_data_PreviewWindow, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &PreviewWindow::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *PreviewWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *PreviewWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_PreviewWindow))
        return static_cast<void*>(const_cast< PreviewWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int PreviewWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: stringSet((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: apply(); break;
        case 2: save(); break;
        case 3: load(); break;
        default: ;
        }
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void PreviewWindow::stringSet(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
static const uint qt_meta_data_RendererButtons[] = {

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
      48,   17,   16,   16, 0x05,

 // slots: signature, parameters, type, tag, flags
      93,   91,   16,   16, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_RendererButtons[] = {
    "RendererButtons\0\0provider,service,index,checked\0"
    "rendererSelected(QString,QString,int,bool)\0"
    "i\0checkboxChanged(int)\0"
};

const QMetaObject RendererButtons::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_RendererButtons,
      qt_meta_data_RendererButtons, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &RendererButtons::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *RendererButtons::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *RendererButtons::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_RendererButtons))
        return static_cast<void*>(const_cast< RendererButtons*>(this));
    return QWidget::qt_metacast(_clname);
}

int RendererButtons::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: rendererSelected((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< bool(*)>(_a[4]))); break;
        case 1: checkboxChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void RendererButtons::rendererSelected(QString _t1, QString _t2, int _t3, bool _t4)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
static const uint qt_meta_data_TaskAdapterGui[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      16,   15,   15,   15, 0x0a,
      25,   15,   15,   15, 0x0a,
      51,   47,   15,   15, 0x0a,
     108,   77,   15,   15, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_TaskAdapterGui[] = {
    "TaskAdapterGui\0\0update()\0browseButtonClicked()\0"
    "str\0setPreviewString(QString)\0"
    "provider,service,index,checked\0"
    "rendererSelected(QString,QString,int,bool)\0"
};

const QMetaObject TaskAdapterGui::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_TaskAdapterGui,
      qt_meta_data_TaskAdapterGui, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &TaskAdapterGui::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *TaskAdapterGui::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *TaskAdapterGui::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_TaskAdapterGui))
        return static_cast<void*>(const_cast< TaskAdapterGui*>(this));
    return QDialog::qt_metacast(_clname);
}

int TaskAdapterGui::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: update(); break;
        case 1: browseButtonClicked(); break;
        case 2: setPreviewString((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 3: rendererSelected((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< bool(*)>(_a[4]))); break;
        default: ;
        }
        _id -= 4;
    }
    return _id;
}
QT_END_MOC_NAMESPACE

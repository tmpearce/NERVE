/****************************************************************************
** Meta object code from reading C++ file 'GeneralGui.h'
**
** Created: Fri Feb 17 11:27:08 2012
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../include/nrvApp/GeneralGui.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'GeneralGui.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_GeneralGui[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
      21,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       6,       // signalCount

 // signals: signature, parameters, type, tag, flags
      12,   11,   11,   11, 0x05,
      43,   11,   11,   11, 0x05,
      75,   72,   11,   11, 0x05,
     119,   72,   11,   11, 0x05,
     161,   72,   11,   11, 0x05,
     213,   72,   11,   11, 0x05,

 // slots: signature, parameters, type, tag, flags
     266,  263,   11,   11, 0x08,
     285,  263,   11,   11, 0x08,
     319,  307,   11,   11, 0x08,
     357,  307,   11,   11, 0x08,
     398,   11,   11,   11, 0x08,
     415,  263,   11,   11, 0x0a,
     437,  263,   11,   11, 0x0a,
     457,   11,   11,   11, 0x0a,
     477,   11,   11,   11, 0x0a,
     498,  486,   11,   11, 0x0a,
     535,  530,   11,   11, 0x0a,
     555,  530,   11,   11, 0x0a,
     582,   11,   11,   11, 0x0a,
     604,   11,  597,   11, 0x0a,
     622,   11,   11,   11, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_GeneralGui[] = {
    "GeneralGui\0\0pluginAvailableSignal(QString)\0"
    "pluginRemovedSignal(QString)\0,,\0"
    "uiAvailableSignal(QString,QString,QWidget*)\0"
    "uiRemovedSignal(QString,QString,QWidget*)\0"
    "uiAvailableSignalBlocking(QString,QString,QWidget*)\0"
    "uiRemovedSignalBlocking(QString,QString,QWidget*)\0"
    "id\0addPlugin(QString)\0removePlugin(QString)\0"
    "id,title,ui\0addPluginUI(QString,QString,QWidget*)\0"
    "removePluginUI(QString,QString,QWidget*)\0"
    "toggleSettings()\0createPlugin(QString)\0"
    "quitPlugin(QString)\0refreshPluginList()\0"
    "update()\0id,floating\0"
    "pluginDockChanged(QString,bool)\0file\0"
    "loadConfig(QString)\0updateRecentFiles(QString)\0"
    "saveConfigAs()\0QMenu*\0createPopupMenu()\0"
    "quit()\0"
};

const QMetaObject GeneralGui::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_GeneralGui,
      qt_meta_data_GeneralGui, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &GeneralGui::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *GeneralGui::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *GeneralGui::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_GeneralGui))
        return static_cast<void*>(const_cast< GeneralGui*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int GeneralGui::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: pluginAvailableSignal((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 1: pluginRemovedSignal((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 2: uiAvailableSignal((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< QWidget*(*)>(_a[3]))); break;
        case 3: uiRemovedSignal((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< QWidget*(*)>(_a[3]))); break;
        case 4: uiAvailableSignalBlocking((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< QWidget*(*)>(_a[3]))); break;
        case 5: uiRemovedSignalBlocking((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< QWidget*(*)>(_a[3]))); break;
        case 6: addPlugin((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 7: removePlugin((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 8: addPluginUI((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QWidget*(*)>(_a[3]))); break;
        case 9: removePluginUI((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QWidget*(*)>(_a[3]))); break;
        case 10: toggleSettings(); break;
        case 11: createPlugin((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 12: quitPlugin((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 13: refreshPluginList(); break;
        case 14: update(); break;
        case 15: pluginDockChanged((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 16: loadConfig((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 17: updateRecentFiles((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 18: saveConfigAs(); break;
        case 19: { QMenu* _r = createPopupMenu();
            if (_a[0]) *reinterpret_cast< QMenu**>(_a[0]) = _r; }  break;
        case 20: quit(); break;
        default: ;
        }
        _id -= 21;
    }
    return _id;
}

// SIGNAL 0
void GeneralGui::pluginAvailableSignal(const QString & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void GeneralGui::pluginRemovedSignal(const QString & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void GeneralGui::uiAvailableSignal(const QString & _t1, const QString & _t2, QWidget * _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void GeneralGui::uiRemovedSignal(const QString & _t1, const QString & _t2, QWidget * _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void GeneralGui::uiAvailableSignalBlocking(const QString & _t1, const QString & _t2, QWidget * _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void GeneralGui::uiRemovedSignalBlocking(const QString & _t1, const QString & _t2, QWidget * _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}
static const uint qt_meta_data_UIDock[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: signature, parameters, type, tag, flags
      20,    8,    7,    7, 0x05,
      54,   52,    7,    7, 0x05,
      85,   82,    7,    7, 0x05,

 // slots: signature, parameters, type, tag, flags
     108,   99,    7,    7, 0x0a,
     134,   52,    7,    7, 0x0a,
     149,   52,    7,    7, 0x0a,
     165,    7,    7,    7, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_UIDock[] = {
    "UIDock\0\0id,floating\0dockStatusChanged(QString,bool)\0"
    "b\0dockShowStatusChanged(bool)\0id\0"
    "quit(QString)\0floating\0dockFloatingChanged(bool)\0"
    "showDock(bool)\0floatDock(bool)\0"
    "requestQuit()\0"
};

const QMetaObject UIDock::staticMetaObject = {
    { &QDockWidget::staticMetaObject, qt_meta_stringdata_UIDock,
      qt_meta_data_UIDock, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &UIDock::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *UIDock::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *UIDock::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_UIDock))
        return static_cast<void*>(const_cast< UIDock*>(this));
    return QDockWidget::qt_metacast(_clname);
}

int UIDock::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDockWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: dockStatusChanged((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 1: dockShowStatusChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 2: quit((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 3: dockFloatingChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 4: showDock((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 5: floatDock((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 6: requestQuit(); break;
        default: ;
        }
        _id -= 7;
    }
    return _id;
}

// SIGNAL 0
void UIDock::dockStatusChanged(QString _t1, bool _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void UIDock::dockShowStatusChanged(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void UIDock::quit(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_END_MOC_NAMESPACE

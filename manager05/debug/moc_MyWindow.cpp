/****************************************************************************
** Meta object code from reading C++ file 'MyWindow.h'
**
** Created: Sun Apr 6 22:14:52 2014
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../MyWindow.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'MyWindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_MyWindow[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      10,    9,    9,    9, 0x08,
      33,    9,    9,    9, 0x08,
      52,    9,    9,    9, 0x08,
      74,   70,    9,    9, 0x08,
     121,    9,    9,    9, 0x08,
     130,    9,    9,    9, 0x08,
     148,    9,  143,    9, 0x08,
     158,    9,    9,    9, 0x08,
     175,  172,    9,    9, 0x08,
     205,    9,  197,    9, 0x08,
     218,    9,    9,    9, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_MyWindow[] = {
    "MyWindow\0\0itemSelectionChanged()\0"
    "slotStartProgram()\0slotStopProgram()\0"
    "pos\0on_treeView_customContextMenuRequested(QPoint)\0"
    "on_del()\0on_refresh()\0bool\0refresh()\0"
    "on_startAll()\0id\0startProgramById(int)\0"
    "QString\0getVlcPath()\0stopAllProcess()\0"
};

void MyWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        MyWindow *_t = static_cast<MyWindow *>(_o);
        switch (_id) {
        case 0: _t->itemSelectionChanged(); break;
        case 1: _t->slotStartProgram(); break;
        case 2: _t->slotStopProgram(); break;
        case 3: _t->on_treeView_customContextMenuRequested((*reinterpret_cast< const QPoint(*)>(_a[1]))); break;
        case 4: _t->on_del(); break;
        case 5: _t->on_refresh(); break;
        case 6: { bool _r = _t->refresh();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 7: _t->on_startAll(); break;
        case 8: _t->startProgramById((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 9: { QString _r = _t->getVlcPath();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 10: _t->stopAllProcess(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData MyWindow::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject MyWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_MyWindow,
      qt_meta_data_MyWindow, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &MyWindow::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *MyWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *MyWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MyWindow))
        return static_cast<void*>(const_cast< MyWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int MyWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    }
    return _id;
}
QT_END_MOC_NAMESPACE

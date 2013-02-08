/****************************************************************************
** Meta object code from reading C++ file 'Qt_compressor_widget_callbacks.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "Qt_compressor_widget_callbacks.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'Qt_compressor_widget_callbacks.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Compressor_widget[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      23,   19,   18,   18, 0x0a,
      56,   19,   18,   18, 0x0a,
      91,   19,   18,   18, 0x0a,
     127,   19,   18,   18, 0x0a,
     151,   18,   18,   18, 0x0a,
     176,   18,   18,   18, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_Compressor_widget[] = {
    "Compressor_widget\0\0val\0"
    "on_enable_checkbox_toggled(bool)\0"
    "on_attack_slider_valueChanged(int)\0"
    "on_release_slider_valueChanged(int)\0"
    "on_bypass_toggled(bool)\0"
    "on_load_button_pressed()\0"
    "on_save_button_pressed()\0"
};

void Compressor_widget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        Compressor_widget *_t = static_cast<Compressor_widget *>(_o);
        switch (_id) {
        case 0: _t->on_enable_checkbox_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: _t->on_attack_slider_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->on_release_slider_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->on_bypass_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 4: _t->on_load_button_pressed(); break;
        case 5: _t->on_save_button_pressed(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData Compressor_widget::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject Compressor_widget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_Compressor_widget,
      qt_meta_data_Compressor_widget, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Compressor_widget::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Compressor_widget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Compressor_widget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Compressor_widget))
        return static_cast<void*>(const_cast< Compressor_widget*>(this));
    if (!strcmp(_clname, "Ui::Compressor_widget"))
        return static_cast< Ui::Compressor_widget*>(const_cast< Compressor_widget*>(this));
    return QWidget::qt_metacast(_clname);
}

int Compressor_widget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    }
    return _id;
}
QT_END_MOC_NAMESPACE

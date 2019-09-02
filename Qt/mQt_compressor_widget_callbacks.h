/****************************************************************************
** Meta object code from reading C++ file 'Qt_compressor_widget_callbacks.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.10.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "Qt_compressor_widget_callbacks.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'Qt_compressor_widget_callbacks.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.10.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_Compressor_widget_t {
    QByteArrayData data[9];
    char stringdata0[175];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Compressor_widget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Compressor_widget_t qt_meta_stringdata_Compressor_widget = {
    {
QT_MOC_LITERAL(0, 0, 17), // "Compressor_widget"
QT_MOC_LITERAL(1, 18, 26), // "on_enable_checkbox_toggled"
QT_MOC_LITERAL(2, 45, 0), // ""
QT_MOC_LITERAL(3, 46, 3), // "val"
QT_MOC_LITERAL(4, 50, 29), // "on_attack_slider_valueChanged"
QT_MOC_LITERAL(5, 80, 30), // "on_release_slider_valueChanged"
QT_MOC_LITERAL(6, 111, 17), // "on_bypass_toggled"
QT_MOC_LITERAL(7, 129, 22), // "on_load_button_pressed"
QT_MOC_LITERAL(8, 152, 22) // "on_save_button_pressed"

    },
    "Compressor_widget\0on_enable_checkbox_toggled\0"
    "\0val\0on_attack_slider_valueChanged\0"
    "on_release_slider_valueChanged\0"
    "on_bypass_toggled\0on_load_button_pressed\0"
    "on_save_button_pressed"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Compressor_widget[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   44,    2, 0x0a /* Public */,
       4,    1,   47,    2, 0x0a /* Public */,
       5,    1,   50,    2, 0x0a /* Public */,
       6,    1,   53,    2, 0x0a /* Public */,
       7,    0,   56,    2, 0x0a /* Public */,
       8,    0,   57,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void, QMetaType::Bool,    3,
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Bool,    3,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void Compressor_widget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Compressor_widget *_t = static_cast<Compressor_widget *>(_o);
        Q_UNUSED(_t)
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

const QMetaObject Compressor_widget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_Compressor_widget.data,
      qt_meta_data_Compressor_widget,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *Compressor_widget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Compressor_widget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_Compressor_widget.stringdata0))
        return static_cast<void*>(this);
    if (!strcmp(_clname, "Ui::Compressor_widget"))
        return static_cast< Ui::Compressor_widget*>(this);
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
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 6;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE

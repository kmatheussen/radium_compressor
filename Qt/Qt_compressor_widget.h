/********************************************************************************
** Form generated from reading UI file 'qt4_compressor_widget.ui'
**
** Created by: Qt User Interface Compiler version 4.8.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QT4_COMPRESSOR_WIDGET_H
#define UI_QT4_COMPRESSOR_WIDGET_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QSlider>
#include <QtGui/QToolButton>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Compressor_widget
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *radium_url;
    MyQCheckBox *bypass;
    MyQCheckBox *enable_checkbox;
    MyQButton *load_button;
    MyQButton *save_button;
    MyQSlider *attack_slider;
    MyQSlider *release_slider;

    void setupUi(QWidget *Compressor_widget)
    {
        if (Compressor_widget->objectName().isEmpty())
            Compressor_widget->setObjectName(QString::fromUtf8("Compressor_widget"));
        Compressor_widget->resize(385, 412);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(1);
        sizePolicy.setHeightForWidth(Compressor_widget->sizePolicy().hasHeightForWidth());
        Compressor_widget->setSizePolicy(sizePolicy);
        verticalLayout = new QVBoxLayout(Compressor_widget);
        verticalLayout->setSpacing(0);
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(2);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setSizeConstraint(QLayout::SetFixedSize);
        horizontalLayout->setContentsMargins(-1, -1, -1, 2);
        radium_url = new QLabel(Compressor_widget);
        radium_url->setObjectName(QString::fromUtf8("radium_url"));
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(radium_url->sizePolicy().hasHeightForWidth());
        radium_url->setSizePolicy(sizePolicy1);
        radium_url->setAlignment(Qt::AlignCenter);
        radium_url->setOpenExternalLinks(true);

        horizontalLayout->addWidget(radium_url);

        bypass = new MyQCheckBox(Compressor_widget);
        bypass->setObjectName(QString::fromUtf8("bypass"));
        QSizePolicy sizePolicy2(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(bypass->sizePolicy().hasHeightForWidth());
        bypass->setSizePolicy(sizePolicy2);

        horizontalLayout->addWidget(bypass);

        enable_checkbox = new MyQCheckBox(Compressor_widget);
        enable_checkbox->setObjectName(QString::fromUtf8("enable_checkbox"));
        sizePolicy2.setHeightForWidth(enable_checkbox->sizePolicy().hasHeightForWidth());
        enable_checkbox->setSizePolicy(sizePolicy2);

        horizontalLayout->addWidget(enable_checkbox);

        load_button = new MyQButton(Compressor_widget);
        load_button->setObjectName(QString::fromUtf8("load_button"));
        sizePolicy1.setHeightForWidth(load_button->sizePolicy().hasHeightForWidth());
        load_button->setSizePolicy(sizePolicy1);

        horizontalLayout->addWidget(load_button);

        save_button = new MyQButton(Compressor_widget);
        save_button->setObjectName(QString::fromUtf8("save_button"));
        sizePolicy1.setHeightForWidth(save_button->sizePolicy().hasHeightForWidth());
        save_button->setSizePolicy(sizePolicy1);

        horizontalLayout->addWidget(save_button);


        verticalLayout->addLayout(horizontalLayout);

        attack_slider = new MyQSlider(Compressor_widget);
        attack_slider->setObjectName(QString::fromUtf8("attack_slider"));
        attack_slider->setEnabled(false);
        attack_slider->setMaximum(1000);
        attack_slider->setOrientation(Qt::Horizontal);

        verticalLayout->addWidget(attack_slider);

        release_slider = new MyQSlider(Compressor_widget);
        release_slider->setObjectName(QString::fromUtf8("release_slider"));
        release_slider->setEnabled(false);
        release_slider->setMaximum(1000);
        release_slider->setOrientation(Qt::Horizontal);

        verticalLayout->addWidget(release_slider);


        retranslateUi(Compressor_widget);

        QMetaObject::connectSlotsByName(Compressor_widget);
    } // setupUi

    void retranslateUi(QWidget *Compressor_widget)
    {
        Compressor_widget->setWindowTitle(QApplication::translate("Compressor_widget", "Radium Compressor", 0, QApplication::UnicodeUTF8));
        radium_url->setText(QApplication::translate("Compressor_widget", "<A href=\"http://users.notam02.no/~kjetism/radium/\">Radium</A>", 0, QApplication::UnicodeUTF8));
        bypass->setText(QApplication::translate("Compressor_widget", "Bypass", 0, QApplication::UnicodeUTF8));
        enable_checkbox->setText(QApplication::translate("Compressor_widget", "Enable", 0, QApplication::UnicodeUTF8));
        load_button->setText(QApplication::translate("Compressor_widget", "Load", 0, QApplication::UnicodeUTF8));
        save_button->setText(QApplication::translate("Compressor_widget", "Save", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class Compressor_widget: public Ui_Compressor_widget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QT4_COMPRESSOR_WIDGET_H

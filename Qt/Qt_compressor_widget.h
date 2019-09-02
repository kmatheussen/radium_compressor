/********************************************************************************
** Form generated from reading UI file 'qt4_compressor_widget.ui'
**
** Created by: Qt User Interface Compiler version 5.10.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QT4_COMPRESSOR_WIDGET_H
#define UI_QT4_COMPRESSOR_WIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSlider>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

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
            Compressor_widget->setObjectName(QStringLiteral("Compressor_widget"));
        Compressor_widget->resize(385, 412);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(1);
        sizePolicy.setHeightForWidth(Compressor_widget->sizePolicy().hasHeightForWidth());
        Compressor_widget->setSizePolicy(sizePolicy);
        verticalLayout = new QVBoxLayout(Compressor_widget);
        verticalLayout->setSpacing(0);
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(2);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setSizeConstraint(QLayout::SetFixedSize);
        horizontalLayout->setContentsMargins(-1, -1, -1, 2);
        radium_url = new QLabel(Compressor_widget);
        radium_url->setObjectName(QStringLiteral("radium_url"));
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(radium_url->sizePolicy().hasHeightForWidth());
        radium_url->setSizePolicy(sizePolicy1);
        radium_url->setAlignment(Qt::AlignCenter);
        radium_url->setOpenExternalLinks(true);

        horizontalLayout->addWidget(radium_url);

        bypass = new MyQCheckBox(Compressor_widget);
        bypass->setObjectName(QStringLiteral("bypass"));
        QSizePolicy sizePolicy2(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(bypass->sizePolicy().hasHeightForWidth());
        bypass->setSizePolicy(sizePolicy2);

        horizontalLayout->addWidget(bypass);

        enable_checkbox = new MyQCheckBox(Compressor_widget);
        enable_checkbox->setObjectName(QStringLiteral("enable_checkbox"));
        sizePolicy2.setHeightForWidth(enable_checkbox->sizePolicy().hasHeightForWidth());
        enable_checkbox->setSizePolicy(sizePolicy2);

        horizontalLayout->addWidget(enable_checkbox);

        load_button = new MyQButton(Compressor_widget);
        load_button->setObjectName(QStringLiteral("load_button"));
        sizePolicy1.setHeightForWidth(load_button->sizePolicy().hasHeightForWidth());
        load_button->setSizePolicy(sizePolicy1);

        horizontalLayout->addWidget(load_button);

        save_button = new MyQButton(Compressor_widget);
        save_button->setObjectName(QStringLiteral("save_button"));
        sizePolicy1.setHeightForWidth(save_button->sizePolicy().hasHeightForWidth());
        save_button->setSizePolicy(sizePolicy1);

        horizontalLayout->addWidget(save_button);


        verticalLayout->addLayout(horizontalLayout);

        attack_slider = new MyQSlider(Compressor_widget);
        attack_slider->setObjectName(QStringLiteral("attack_slider"));
        attack_slider->setEnabled(false);
        attack_slider->setMaximum(1000);
        attack_slider->setOrientation(Qt::Horizontal);

        verticalLayout->addWidget(attack_slider);

        release_slider = new MyQSlider(Compressor_widget);
        release_slider->setObjectName(QStringLiteral("release_slider"));
        release_slider->setEnabled(false);
        release_slider->setMaximum(1000);
        release_slider->setOrientation(Qt::Horizontal);

        verticalLayout->addWidget(release_slider);


        retranslateUi(Compressor_widget);

        QMetaObject::connectSlotsByName(Compressor_widget);
    } // setupUi

    void retranslateUi(QWidget *Compressor_widget)
    {
        Compressor_widget->setWindowTitle(QApplication::translate("Compressor_widget", "Radium Compressor", nullptr));
        radium_url->setText(QApplication::translate("Compressor_widget", "<A href=\"http://users.notam02.no/~kjetism/radium/\">Radium</A>", nullptr));
        bypass->setText(QApplication::translate("Compressor_widget", "Bypass", nullptr));
        enable_checkbox->setText(QApplication::translate("Compressor_widget", "Enable", nullptr));
        load_button->setText(QApplication::translate("Compressor_widget", "Load", nullptr));
        save_button->setText(QApplication::translate("Compressor_widget", "Save", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Compressor_widget: public Ui_Compressor_widget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QT4_COMPRESSOR_WIDGET_H

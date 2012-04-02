/********************************************************************************
** Form generated from reading UI file 'SettingsEditor.ui'
**
** Created: Fri Feb 17 15:25:33 2012
**      by: Qt User Interface Compiler version 4.7.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef _UIC_SETTINGSEDITOR_H
#define _UIC_SETTINGSEDITOR_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QFrame>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QScrollArea>
#include <QtGui/QStackedWidget>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SettingsEditor
{
public:
    QStackedWidget *stackedWidget;
    QWidget *page;
    QFrame *frame;
    QPushButton *buttonStartup;
    QPushButton *buttonFavorites;
    QPushButton *buttonMain_GUI;
    QPushButton *buttonDone;
    QWidget *page_2;
    QLabel *label_3;
    QScrollArea *startupScrollArea;
    QWidget *scrollAreaWidgetContents;
    QPushButton *buttonApply;
    QPushButton *buttonReturn;
    QWidget *page_3;
    QPushButton *buttonReturn_2;
    QPushButton *buttonApply_2;
    QLabel *label_4;
    QScrollArea *favoritesScrollArea;
    QWidget *scrollAreaWidgetContents_2;
    QWidget *page_4;
    QPushButton *buttonReturn_3;
    QPushButton *buttonApply_3;
    QLabel *label;

    void setupUi(QDialog *SettingsEditor)
    {
        if (SettingsEditor->objectName().isEmpty())
            SettingsEditor->setObjectName(QString::fromUtf8("SettingsEditor"));
        SettingsEditor->resize(222, 391);
        stackedWidget = new QStackedWidget(SettingsEditor);
        stackedWidget->setObjectName(QString::fromUtf8("stackedWidget"));
        stackedWidget->setGeometry(QRect(10, 0, 201, 381));
        page = new QWidget();
        page->setObjectName(QString::fromUtf8("page"));
        frame = new QFrame(page);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setGeometry(QRect(30, 50, 120, 281));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        frame->setLineWidth(4);
        buttonStartup = new QPushButton(frame);
        buttonStartup->setObjectName(QString::fromUtf8("buttonStartup"));
        buttonStartup->setGeometry(QRect(20, 10, 75, 23));
        buttonFavorites = new QPushButton(frame);
        buttonFavorites->setObjectName(QString::fromUtf8("buttonFavorites"));
        buttonFavorites->setGeometry(QRect(20, 40, 75, 23));
        buttonMain_GUI = new QPushButton(frame);
        buttonMain_GUI->setObjectName(QString::fromUtf8("buttonMain_GUI"));
        buttonMain_GUI->setGeometry(QRect(20, 70, 75, 23));
        buttonDone = new QPushButton(page);
        buttonDone->setObjectName(QString::fromUtf8("buttonDone"));
        buttonDone->setGeometry(QRect(120, 356, 75, 23));
        stackedWidget->addWidget(page);
        page_2 = new QWidget();
        page_2->setObjectName(QString::fromUtf8("page_2"));
        label_3 = new QLabel(page_2);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(13, 10, 61, 16));
        startupScrollArea = new QScrollArea(page_2);
        startupScrollArea->setObjectName(QString::fromUtf8("startupScrollArea"));
        startupScrollArea->setGeometry(QRect(13, 30, 181, 321));
        startupScrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QString::fromUtf8("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 179, 319));
        startupScrollArea->setWidget(scrollAreaWidgetContents);
        buttonApply = new QPushButton(page_2);
        buttonApply->setObjectName(QString::fromUtf8("buttonApply"));
        buttonApply->setGeometry(QRect(40, 356, 75, 23));
        buttonReturn = new QPushButton(page_2);
        buttonReturn->setObjectName(QString::fromUtf8("buttonReturn"));
        buttonReturn->setGeometry(QRect(120, 356, 75, 23));
        stackedWidget->addWidget(page_2);
        page_3 = new QWidget();
        page_3->setObjectName(QString::fromUtf8("page_3"));
        buttonReturn_2 = new QPushButton(page_3);
        buttonReturn_2->setObjectName(QString::fromUtf8("buttonReturn_2"));
        buttonReturn_2->setGeometry(QRect(120, 356, 75, 23));
        buttonApply_2 = new QPushButton(page_3);
        buttonApply_2->setObjectName(QString::fromUtf8("buttonApply_2"));
        buttonApply_2->setGeometry(QRect(40, 356, 75, 23));
        label_4 = new QLabel(page_3);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(13, 10, 61, 16));
        favoritesScrollArea = new QScrollArea(page_3);
        favoritesScrollArea->setObjectName(QString::fromUtf8("favoritesScrollArea"));
        favoritesScrollArea->setGeometry(QRect(13, 30, 181, 321));
        favoritesScrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents_2 = new QWidget();
        scrollAreaWidgetContents_2->setObjectName(QString::fromUtf8("scrollAreaWidgetContents_2"));
        scrollAreaWidgetContents_2->setGeometry(QRect(0, 0, 179, 319));
        favoritesScrollArea->setWidget(scrollAreaWidgetContents_2);
        stackedWidget->addWidget(page_3);
        page_4 = new QWidget();
        page_4->setObjectName(QString::fromUtf8("page_4"));
        buttonReturn_3 = new QPushButton(page_4);
        buttonReturn_3->setObjectName(QString::fromUtf8("buttonReturn_3"));
        buttonReturn_3->setGeometry(QRect(120, 356, 75, 23));
        buttonApply_3 = new QPushButton(page_4);
        buttonApply_3->setObjectName(QString::fromUtf8("buttonApply_3"));
        buttonApply_3->setGeometry(QRect(40, 356, 75, 23));
        label = new QLabel(page_4);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(13, 10, 101, 16));
        stackedWidget->addWidget(page_4);

        retranslateUi(SettingsEditor);

        stackedWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(SettingsEditor);
    } // setupUi

    void retranslateUi(QDialog *SettingsEditor)
    {
        SettingsEditor->setWindowTitle(QApplication::translate("SettingsEditor", "Dialog", 0, QApplication::UnicodeUTF8));
        buttonStartup->setText(QApplication::translate("SettingsEditor", "Startup", 0, QApplication::UnicodeUTF8));
        buttonFavorites->setText(QApplication::translate("SettingsEditor", "Favorites", 0, QApplication::UnicodeUTF8));
        buttonMain_GUI->setText(QApplication::translate("SettingsEditor", "Main GUI", 0, QApplication::UnicodeUTF8));
        buttonDone->setText(QApplication::translate("SettingsEditor", "Done", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("SettingsEditor", "On startup:", 0, QApplication::UnicodeUTF8));
        buttonApply->setText(QApplication::translate("SettingsEditor", "Apply", 0, QApplication::UnicodeUTF8));
        buttonReturn->setText(QApplication::translate("SettingsEditor", "Return", 0, QApplication::UnicodeUTF8));
        buttonReturn_2->setText(QApplication::translate("SettingsEditor", "Return", 0, QApplication::UnicodeUTF8));
        buttonApply_2->setText(QApplication::translate("SettingsEditor", "Apply", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("SettingsEditor", "Favorites:", 0, QApplication::UnicodeUTF8));
        buttonReturn_3->setText(QApplication::translate("SettingsEditor", "Return", 0, QApplication::UnicodeUTF8));
        buttonApply_3->setText(QApplication::translate("SettingsEditor", "Apply", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("SettingsEditor", "Main GUI behavior", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class SettingsEditor: public Ui_SettingsEditor {};
} // namespace Ui

QT_END_NAMESPACE

#endif // _UIC_SETTINGSEDITOR_H

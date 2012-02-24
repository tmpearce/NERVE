/********************************************************************************
** Form generated from reading UI file 'GeneralGui.ui'
**
** Created: Mon Feb 20 16:18:47 2012
**      by: Qt User Interface Compiler version 4.7.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef _UIC_GENERALGUI_H
#define _UIC_GENERALGUI_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QStatusBar>
#include <QtGui/QToolBar>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_GeneralGui
{
public:
    QAction *actionTask_switcher;
    QAction *actionBasic_Task_selection;
    QAction *actionExit;
    QAction *actionLoad_Profile;
    QAction *actionSave_Profile;
    QAction *actionGraphics_Windows;
    QAction *actionCreate_Plugin;
    QAction *actionCreate_Window;
    QAction *actionEdit;
    QAction *actionRefresh_Plugins;
    QAction *actionOther_files;
    QAction *actionSave_as;
    QAction *actionSave_default_config;
    QWidget *centralwidget;
    QMenuBar *menubar;
    QMenu *menuFile;
    QMenu *menuPlugins;
    QMenu *menuServices;
    QStatusBar *statusbar;
    QToolBar *favoritesToolbar;

    void setupUi(QMainWindow *GeneralGui)
    {
        if (GeneralGui->objectName().isEmpty())
            GeneralGui->setObjectName(QString::fromUtf8("GeneralGui"));
        GeneralGui->resize(223, 392);
        actionTask_switcher = new QAction(GeneralGui);
        actionTask_switcher->setObjectName(QString::fromUtf8("actionTask_switcher"));
        actionTask_switcher->setCheckable(true);
        actionTask_switcher->setEnabled(true);
        actionBasic_Task_selection = new QAction(GeneralGui);
        actionBasic_Task_selection->setObjectName(QString::fromUtf8("actionBasic_Task_selection"));
        actionBasic_Task_selection->setCheckable(true);
        actionExit = new QAction(GeneralGui);
        actionExit->setObjectName(QString::fromUtf8("actionExit"));
        actionLoad_Profile = new QAction(GeneralGui);
        actionLoad_Profile->setObjectName(QString::fromUtf8("actionLoad_Profile"));
        actionSave_Profile = new QAction(GeneralGui);
        actionSave_Profile->setObjectName(QString::fromUtf8("actionSave_Profile"));
        actionGraphics_Windows = new QAction(GeneralGui);
        actionGraphics_Windows->setObjectName(QString::fromUtf8("actionGraphics_Windows"));
        actionCreate_Plugin = new QAction(GeneralGui);
        actionCreate_Plugin->setObjectName(QString::fromUtf8("actionCreate_Plugin"));
        actionCreate_Window = new QAction(GeneralGui);
        actionCreate_Window->setObjectName(QString::fromUtf8("actionCreate_Window"));
        actionEdit = new QAction(GeneralGui);
        actionEdit->setObjectName(QString::fromUtf8("actionEdit"));
        actionEdit->setEnabled(true);
        actionRefresh_Plugins = new QAction(GeneralGui);
        actionRefresh_Plugins->setObjectName(QString::fromUtf8("actionRefresh_Plugins"));
        actionOther_files = new QAction(GeneralGui);
        actionOther_files->setObjectName(QString::fromUtf8("actionOther_files"));
        actionSave_as = new QAction(GeneralGui);
        actionSave_as->setObjectName(QString::fromUtf8("actionSave_as"));
        actionSave_default_config = new QAction(GeneralGui);
        actionSave_default_config->setObjectName(QString::fromUtf8("actionSave_default_config"));
        centralwidget = new QWidget(GeneralGui);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        GeneralGui->setCentralWidget(centralwidget);
        menubar = new QMenuBar(GeneralGui);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 223, 21));
        menuFile = new QMenu(menubar);
        menuFile->setObjectName(QString::fromUtf8("menuFile"));
        menuPlugins = new QMenu(menubar);
        menuPlugins->setObjectName(QString::fromUtf8("menuPlugins"));
        menuServices = new QMenu(menubar);
        menuServices->setObjectName(QString::fromUtf8("menuServices"));
        GeneralGui->setMenuBar(menubar);
        statusbar = new QStatusBar(GeneralGui);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        GeneralGui->setStatusBar(statusbar);
        favoritesToolbar = new QToolBar(GeneralGui);
        favoritesToolbar->setObjectName(QString::fromUtf8("favoritesToolbar"));
        favoritesToolbar->setMovable(false);
        favoritesToolbar->setFloatable(false);
        GeneralGui->addToolBar(Qt::TopToolBarArea, favoritesToolbar);

        menubar->addAction(menuFile->menuAction());
        menubar->addAction(menuPlugins->menuAction());
        menubar->addAction(menuServices->menuAction());
        menuFile->addAction(actionSave_as);
        menuFile->addSeparator();
        menuFile->addAction(actionOther_files);
        menuFile->addSeparator();
        menuFile->addAction(actionExit);
        menuPlugins->addAction(actionRefresh_Plugins);
        menuPlugins->addAction(actionCreate_Plugin);
        menuPlugins->addSeparator();
        menuServices->addAction(actionEdit);

        retranslateUi(GeneralGui);

        QMetaObject::connectSlotsByName(GeneralGui);
    } // setupUi

    void retranslateUi(QMainWindow *GeneralGui)
    {
        GeneralGui->setWindowTitle(QApplication::translate("GeneralGui", "N.E.R.V.E.", 0, QApplication::UnicodeUTF8));
        actionTask_switcher->setText(QApplication::translate("GeneralGui", "Task switcher", 0, QApplication::UnicodeUTF8));
        actionBasic_Task_selection->setText(QApplication::translate("GeneralGui", "Basic Task selection", 0, QApplication::UnicodeUTF8));
        actionExit->setText(QApplication::translate("GeneralGui", "Exit", 0, QApplication::UnicodeUTF8));
        actionLoad_Profile->setText(QApplication::translate("GeneralGui", "Load Profile", 0, QApplication::UnicodeUTF8));
        actionSave_Profile->setText(QApplication::translate("GeneralGui", "Save Profile", 0, QApplication::UnicodeUTF8));
        actionGraphics_Windows->setText(QApplication::translate("GeneralGui", "Graphics Windows", 0, QApplication::UnicodeUTF8));
        actionCreate_Plugin->setText(QApplication::translate("GeneralGui", "Launch Plugin", 0, QApplication::UnicodeUTF8));
        actionCreate_Window->setText(QApplication::translate("GeneralGui", "Create Window", 0, QApplication::UnicodeUTF8));
        actionEdit->setText(QApplication::translate("GeneralGui", "Edit", 0, QApplication::UnicodeUTF8));
        actionRefresh_Plugins->setText(QApplication::translate("GeneralGui", "Refresh Plugins", 0, QApplication::UnicodeUTF8));
        actionOther_files->setText(QApplication::translate("GeneralGui", "Other files...", 0, QApplication::UnicodeUTF8));
        actionSave_as->setText(QApplication::translate("GeneralGui", "Save config as...", 0, QApplication::UnicodeUTF8));
        actionSave_default_config->setText(QApplication::translate("GeneralGui", "Save default config", 0, QApplication::UnicodeUTF8));
        menuFile->setTitle(QApplication::translate("GeneralGui", "File", 0, QApplication::UnicodeUTF8));
        menuPlugins->setTitle(QApplication::translate("GeneralGui", "Plugins", 0, QApplication::UnicodeUTF8));
        menuServices->setTitle(QApplication::translate("GeneralGui", "Settings", 0, QApplication::UnicodeUTF8));
        favoritesToolbar->setWindowTitle(QApplication::translate("GeneralGui", "toolBar", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class GeneralGui: public Ui_GeneralGui {};
} // namespace Ui

QT_END_NAMESPACE

#endif // _UIC_GENERALGUI_H

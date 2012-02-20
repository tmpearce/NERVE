/********************************************************************************
** Form generated from reading UI file 'TaskAdapterGui.ui'
**
** Created: Mon Feb 20 16:18:47 2012
**      by: Qt User Interface Compiler version 4.7.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef _UIC_TASKADAPTERGUI_H
#define _UIC_TASKADAPTERGUI_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QFrame>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QListWidget>
#include <QtGui/QProgressBar>
#include <QtGui/QPushButton>
#include <QtGui/QScrollArea>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_TaskAdapterGui
{
public:
    QPushButton *runButton;
    QFrame *line;
    QLineEdit *configFileLineEdit;
    QPushButton *browseButton;
    QPushButton *saveCurrentButton;
    QPushButton *applyButton;
    QLabel *label_2;
    QPushButton *launchButton;
    QProgressBar *taskActive;
    QProgressBar *taskRunning;
    QProgressBar *taskComplete;
    QLabel *label_3;
    QLabel *label_4;
    QLabel *label_5;
    QPushButton *previewFileButton;
    QPushButton *previewBlankButton;
    QLabel *label_6;
    QPushButton *previewCurrentButton;
    QFrame *line_2;
    QFrame *frame;
    QLabel *label;
    QListWidget *sceneRootList;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;

    void setupUi(QDialog *TaskAdapterGui)
    {
        if (TaskAdapterGui->objectName().isEmpty())
            TaskAdapterGui->setObjectName(QString::fromUtf8("TaskAdapterGui"));
        TaskAdapterGui->resize(396, 298);
        runButton = new QPushButton(TaskAdapterGui);
        runButton->setObjectName(QString::fromUtf8("runButton"));
        runButton->setGeometry(QRect(120, 40, 75, 23));
        line = new QFrame(TaskAdapterGui);
        line->setObjectName(QString::fromUtf8("line"));
        line->setGeometry(QRect(10, 60, 381, 16));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);
        configFileLineEdit = new QLineEdit(TaskAdapterGui);
        configFileLineEdit->setObjectName(QString::fromUtf8("configFileLineEdit"));
        configFileLineEdit->setGeometry(QRect(65, 70, 251, 20));
        browseButton = new QPushButton(TaskAdapterGui);
        browseButton->setObjectName(QString::fromUtf8("browseButton"));
        browseButton->setGeometry(QRect(320, 70, 21, 21));
        saveCurrentButton = new QPushButton(TaskAdapterGui);
        saveCurrentButton->setObjectName(QString::fromUtf8("saveCurrentButton"));
        saveCurrentButton->setGeometry(QRect(280, 90, 101, 21));
        applyButton = new QPushButton(TaskAdapterGui);
        applyButton->setObjectName(QString::fromUtf8("applyButton"));
        applyButton->setGeometry(QRect(340, 70, 41, 21));
        label_2 = new QLabel(TaskAdapterGui);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(10, 70, 61, 16));
        launchButton = new QPushButton(TaskAdapterGui);
        launchButton->setObjectName(QString::fromUtf8("launchButton"));
        launchButton->setGeometry(QRect(21, 40, 75, 23));
        taskActive = new QProgressBar(TaskAdapterGui);
        taskActive->setObjectName(QString::fromUtf8("taskActive"));
        taskActive->setGeometry(QRect(71, 15, 21, 23));
        taskActive->setValue(0);
        taskActive->setTextVisible(false);
        taskRunning = new QProgressBar(TaskAdapterGui);
        taskRunning->setObjectName(QString::fromUtf8("taskRunning"));
        taskRunning->setGeometry(QRect(171, 15, 21, 23));
        taskRunning->setValue(0);
        taskRunning->setTextVisible(false);
        taskComplete = new QProgressBar(TaskAdapterGui);
        taskComplete->setObjectName(QString::fromUtf8("taskComplete"));
        taskComplete->setGeometry(QRect(263, 15, 21, 23));
        taskComplete->setValue(0);
        taskComplete->setTextVisible(false);
        label_3 = new QLabel(TaskAdapterGui);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(27, 22, 41, 16));
        label_4 = new QLabel(TaskAdapterGui);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(124, 19, 51, 20));
        label_5 = new QLabel(TaskAdapterGui);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(210, 20, 51, 20));
        previewFileButton = new QPushButton(TaskAdapterGui);
        previewFileButton->setObjectName(QString::fromUtf8("previewFileButton"));
        previewFileButton->setGeometry(QRect(80, 90, 51, 23));
        previewBlankButton = new QPushButton(TaskAdapterGui);
        previewBlankButton->setObjectName(QString::fromUtf8("previewBlankButton"));
        previewBlankButton->setGeometry(QRect(130, 90, 51, 23));
        label_6 = new QLabel(TaskAdapterGui);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setGeometry(QRect(10, 95, 71, 16));
        previewCurrentButton = new QPushButton(TaskAdapterGui);
        previewCurrentButton->setObjectName(QString::fromUtf8("previewCurrentButton"));
        previewCurrentButton->setGeometry(QRect(180, 90, 51, 23));
        line_2 = new QFrame(TaskAdapterGui);
        line_2->setObjectName(QString::fromUtf8("line_2"));
        line_2->setGeometry(QRect(10, 110, 381, 16));
        line_2->setFrameShape(QFrame::HLine);
        line_2->setFrameShadow(QFrame::Sunken);
        frame = new QFrame(TaskAdapterGui);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setGeometry(QRect(290, 130, 91, 161));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        label = new QLabel(frame);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(10, 0, 71, 16));
        sceneRootList = new QListWidget(frame);
        sceneRootList->setObjectName(QString::fromUtf8("sceneRootList"));
        sceneRootList->setGeometry(QRect(0, 20, 91, 141));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(sceneRootList->sizePolicy().hasHeightForWidth());
        sceneRootList->setSizePolicy(sizePolicy);
        sceneRootList->setLineWidth(0);
        sceneRootList->setSelectionMode(QAbstractItemView::NoSelection);
        sceneRootList->setWordWrap(true);
        scrollArea = new QScrollArea(TaskAdapterGui);
        scrollArea->setObjectName(QString::fromUtf8("scrollArea"));
        scrollArea->setGeometry(QRect(9, 150, 271, 141));
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QString::fromUtf8("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 269, 139));
        scrollArea->setWidget(scrollAreaWidgetContents);

        retranslateUi(TaskAdapterGui);

        QMetaObject::connectSlotsByName(TaskAdapterGui);
    } // setupUi

    void retranslateUi(QDialog *TaskAdapterGui)
    {
        TaskAdapterGui->setWindowTitle(QApplication::translate("TaskAdapterGui", "Dialog", 0, QApplication::UnicodeUTF8));
        runButton->setText(QApplication::translate("TaskAdapterGui", "Run", 0, QApplication::UnicodeUTF8));
        browseButton->setText(QApplication::translate("TaskAdapterGui", "...", 0, QApplication::UnicodeUTF8));
        saveCurrentButton->setText(QApplication::translate("TaskAdapterGui", "Save current to file", 0, QApplication::UnicodeUTF8));
        applyButton->setText(QApplication::translate("TaskAdapterGui", "Apply", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("TaskAdapterGui", "Config file:", 0, QApplication::UnicodeUTF8));
        launchButton->setText(QApplication::translate("TaskAdapterGui", "Launch Task", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("TaskAdapterGui", "Active:", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("TaskAdapterGui", "Running:", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("TaskAdapterGui", "Complete:", 0, QApplication::UnicodeUTF8));
        previewFileButton->setText(QApplication::translate("TaskAdapterGui", "File", 0, QApplication::UnicodeUTF8));
        previewBlankButton->setText(QApplication::translate("TaskAdapterGui", "Blank", 0, QApplication::UnicodeUTF8));
        label_6->setText(QApplication::translate("TaskAdapterGui", "Preview from:", 0, QApplication::UnicodeUTF8));
        previewCurrentButton->setText(QApplication::translate("TaskAdapterGui", "Current", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("TaskAdapterGui", "Scene roots:", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class TaskAdapterGui: public Ui_TaskAdapterGui {};
} // namespace Ui

QT_END_NAMESPACE

#endif // _UIC_TASKADAPTERGUI_H

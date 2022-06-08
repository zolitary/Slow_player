/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.2.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "videoslider.h"
#include "videowidget.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QGridLayout *gridLayout_2;
    QStackedWidget *playWidget;
    QWidget *openFilePage;
    QHBoxLayout *horizontalLayout_5;
    QPushButton *openFileBtn;
    QPushButton *openDirBtn;
    QWidget *videoPage;
    QHBoxLayout *horizontalLayout_6;
    VideoWidget *videoWidget;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_7;
    QPushButton *addFileBtn;
    QPushButton *addFolderBtn;
    QPushButton *clearListBtn;
    QListWidget *fileList;
    QVBoxLayout *verticalLayout_3;
    QHBoxLayout *horizontalLayout_3;
    VideoSlider *timeSlider;
    QLabel *timeLabel;
    QLabel *label_2;
    QLabel *durationLabel;
    QHBoxLayout *horizontalLayout;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *back3sBtn;
    QPushButton *lastMediaBtn;
    QPushButton *playBtn;
    QPushButton *nextMediaBtn;
    QPushButton *speed3sBtn;
    QPushButton *stopBtn;
    QSpacerItem *horizontalSpacer;
    QHBoxLayout *horizontalLayout_4;
    QComboBox *mutipleSpeed;
    QPushButton *muteBtn;
    VideoSlider *volumnSlider;
    QLabel *volumnLabel;
    QSpacerItem *horizontalSpacer_2;
    QHBoxLayout *horizontalLayout_8;
    QPushButton *listBtn;
    QPushButton *fillBtn;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(873, 649);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        gridLayout_2 = new QGridLayout(centralwidget);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        playWidget = new QStackedWidget(centralwidget);
        playWidget->setObjectName(QString::fromUtf8("playWidget"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(playWidget->sizePolicy().hasHeightForWidth());
        playWidget->setSizePolicy(sizePolicy);
        playWidget->setMinimumSize(QSize(0, 300));
        openFilePage = new QWidget();
        openFilePage->setObjectName(QString::fromUtf8("openFilePage"));
        horizontalLayout_5 = new QHBoxLayout(openFilePage);
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        openFileBtn = new QPushButton(openFilePage);
        openFileBtn->setObjectName(QString::fromUtf8("openFileBtn"));
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(openFileBtn->sizePolicy().hasHeightForWidth());
        openFileBtn->setSizePolicy(sizePolicy1);
        openFileBtn->setFocusPolicy(Qt::NoFocus);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/new/prefix/addFile.png"), QSize(), QIcon::Normal, QIcon::Off);
        openFileBtn->setIcon(icon);
        openFileBtn->setFlat(true);

        horizontalLayout_5->addWidget(openFileBtn);

        openDirBtn = new QPushButton(openFilePage);
        openDirBtn->setObjectName(QString::fromUtf8("openDirBtn"));
        sizePolicy1.setHeightForWidth(openDirBtn->sizePolicy().hasHeightForWidth());
        openDirBtn->setSizePolicy(sizePolicy1);
        openDirBtn->setFocusPolicy(Qt::NoFocus);
        openDirBtn->setStyleSheet(QString::fromUtf8(""));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/new/prefix/addFolder.png"), QSize(), QIcon::Normal, QIcon::Off);
        openDirBtn->setIcon(icon1);
        openDirBtn->setFlat(true);

        horizontalLayout_5->addWidget(openDirBtn);

        playWidget->addWidget(openFilePage);
        videoPage = new QWidget();
        videoPage->setObjectName(QString::fromUtf8("videoPage"));
        QSizePolicy sizePolicy2(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(videoPage->sizePolicy().hasHeightForWidth());
        videoPage->setSizePolicy(sizePolicy2);
        videoPage->setStyleSheet(QString::fromUtf8(""));
        horizontalLayout_6 = new QHBoxLayout(videoPage);
        horizontalLayout_6->setSpacing(0);
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        horizontalLayout_6->setContentsMargins(0, 0, 0, 0);
        videoWidget = new VideoWidget(videoPage);
        videoWidget->setObjectName(QString::fromUtf8("videoWidget"));
        videoWidget->setEnabled(true);
        sizePolicy.setHeightForWidth(videoWidget->sizePolicy().hasHeightForWidth());
        videoWidget->setSizePolicy(sizePolicy);

        horizontalLayout_6->addWidget(videoWidget);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setObjectName(QString::fromUtf8("horizontalLayout_7"));
        addFileBtn = new QPushButton(videoPage);
        addFileBtn->setObjectName(QString::fromUtf8("addFileBtn"));
        sizePolicy1.setHeightForWidth(addFileBtn->sizePolicy().hasHeightForWidth());
        addFileBtn->setSizePolicy(sizePolicy1);
        addFileBtn->setFocusPolicy(Qt::NoFocus);
        addFileBtn->setIcon(icon);
        addFileBtn->setFlat(true);

        horizontalLayout_7->addWidget(addFileBtn);

        addFolderBtn = new QPushButton(videoPage);
        addFolderBtn->setObjectName(QString::fromUtf8("addFolderBtn"));
        sizePolicy1.setHeightForWidth(addFolderBtn->sizePolicy().hasHeightForWidth());
        addFolderBtn->setSizePolicy(sizePolicy1);
        addFolderBtn->setFocusPolicy(Qt::NoFocus);
        addFolderBtn->setIcon(icon1);
        addFolderBtn->setFlat(true);

        horizontalLayout_7->addWidget(addFolderBtn);

        clearListBtn = new QPushButton(videoPage);
        clearListBtn->setObjectName(QString::fromUtf8("clearListBtn"));
        sizePolicy1.setHeightForWidth(clearListBtn->sizePolicy().hasHeightForWidth());
        clearListBtn->setSizePolicy(sizePolicy1);
        clearListBtn->setFocusPolicy(Qt::NoFocus);
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/new/prefix/clear.png"), QSize(), QIcon::Normal, QIcon::Off);
        clearListBtn->setIcon(icon2);
        clearListBtn->setFlat(true);

        horizontalLayout_7->addWidget(clearListBtn);


        verticalLayout->addLayout(horizontalLayout_7);

        fileList = new QListWidget(videoPage);
        fileList->setObjectName(QString::fromUtf8("fileList"));
        QSizePolicy sizePolicy3(QSizePolicy::Preferred, QSizePolicy::Expanding);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(fileList->sizePolicy().hasHeightForWidth());
        fileList->setSizePolicy(sizePolicy3);

        verticalLayout->addWidget(fileList);


        horizontalLayout_6->addLayout(verticalLayout);

        playWidget->addWidget(videoPage);

        gridLayout_2->addWidget(playWidget, 0, 0, 1, 1);

        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setSpacing(0);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(3);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        timeSlider = new VideoSlider(centralwidget);
        timeSlider->setObjectName(QString::fromUtf8("timeSlider"));
        timeSlider->setEnabled(false);
        timeSlider->setMinimumSize(QSize(250, 0));
        timeSlider->setOrientation(Qt::Horizontal);

        horizontalLayout_3->addWidget(timeSlider);

        timeLabel = new QLabel(centralwidget);
        timeLabel->setObjectName(QString::fromUtf8("timeLabel"));
        timeLabel->setMinimumSize(QSize(0, 0));
        timeLabel->setMaximumSize(QSize(70, 16777215));
        QFont font;
        font.setFamilies({QString::fromUtf8("Consolas")});
        timeLabel->setFont(font);
        timeLabel->setAlignment(Qt::AlignCenter);

        horizontalLayout_3->addWidget(timeLabel);

        label_2 = new QLabel(centralwidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setFont(font);
        label_2->setAlignment(Qt::AlignCenter);

        horizontalLayout_3->addWidget(label_2);

        durationLabel = new QLabel(centralwidget);
        durationLabel->setObjectName(QString::fromUtf8("durationLabel"));
        durationLabel->setMaximumSize(QSize(70, 16777215));
        durationLabel->setFont(font);
        durationLabel->setAlignment(Qt::AlignCenter);

        horizontalLayout_3->addWidget(durationLabel);


        verticalLayout_3->addLayout(horizontalLayout_3);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        back3sBtn = new QPushButton(centralwidget);
        back3sBtn->setObjectName(QString::fromUtf8("back3sBtn"));
        back3sBtn->setEnabled(false);
        QSizePolicy sizePolicy4(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy4.setHorizontalStretch(0);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(back3sBtn->sizePolicy().hasHeightForWidth());
        back3sBtn->setSizePolicy(sizePolicy4);
        back3sBtn->setFocusPolicy(Qt::NoFocus);
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/new/prefix/reverse3s.png"), QSize(), QIcon::Normal, QIcon::Off);
        back3sBtn->setIcon(icon3);
        back3sBtn->setFlat(true);

        horizontalLayout_2->addWidget(back3sBtn);

        lastMediaBtn = new QPushButton(centralwidget);
        lastMediaBtn->setObjectName(QString::fromUtf8("lastMediaBtn"));
        lastMediaBtn->setEnabled(false);
        lastMediaBtn->setFocusPolicy(Qt::NoFocus);
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/new/prefix/lastMedia.png"), QSize(), QIcon::Normal, QIcon::Off);
        lastMediaBtn->setIcon(icon4);
        lastMediaBtn->setFlat(true);

        horizontalLayout_2->addWidget(lastMediaBtn);

        playBtn = new QPushButton(centralwidget);
        playBtn->setObjectName(QString::fromUtf8("playBtn"));
        playBtn->setEnabled(false);
        playBtn->setMaximumSize(QSize(40, 16777215));
        playBtn->setFocusPolicy(Qt::NoFocus);
        QIcon icon5;
        icon5.addFile(QString::fromUtf8(":/new/prefix/play.png"), QSize(), QIcon::Normal, QIcon::Off);
        playBtn->setIcon(icon5);
        playBtn->setFlat(true);

        horizontalLayout_2->addWidget(playBtn);

        nextMediaBtn = new QPushButton(centralwidget);
        nextMediaBtn->setObjectName(QString::fromUtf8("nextMediaBtn"));
        nextMediaBtn->setEnabled(false);
        nextMediaBtn->setFocusPolicy(Qt::NoFocus);
        QIcon icon6;
        icon6.addFile(QString::fromUtf8(":/new/prefix/nextMedia.png"), QSize(), QIcon::Normal, QIcon::Off);
        nextMediaBtn->setIcon(icon6);
        nextMediaBtn->setFlat(true);

        horizontalLayout_2->addWidget(nextMediaBtn);

        speed3sBtn = new QPushButton(centralwidget);
        speed3sBtn->setObjectName(QString::fromUtf8("speed3sBtn"));
        speed3sBtn->setEnabled(false);
        speed3sBtn->setFocusPolicy(Qt::NoFocus);
        QIcon icon7;
        icon7.addFile(QString::fromUtf8(":/new/prefix/speed3s.png"), QSize(), QIcon::Normal, QIcon::Off);
        speed3sBtn->setIcon(icon7);
        speed3sBtn->setFlat(true);

        horizontalLayout_2->addWidget(speed3sBtn);

        stopBtn = new QPushButton(centralwidget);
        stopBtn->setObjectName(QString::fromUtf8("stopBtn"));
        stopBtn->setEnabled(false);
        stopBtn->setMaximumSize(QSize(40, 16777215));
        stopBtn->setFocusPolicy(Qt::NoFocus);
        QIcon icon8;
        icon8.addFile(QString::fromUtf8(":/new/prefix/24gl-stop.png"), QSize(), QIcon::Normal, QIcon::Off);
        stopBtn->setIcon(icon8);
        stopBtn->setFlat(true);

        horizontalLayout_2->addWidget(stopBtn);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        mutipleSpeed = new QComboBox(centralwidget);
        mutipleSpeed->addItem(QString());
        mutipleSpeed->addItem(QString());
        mutipleSpeed->addItem(QString());
        mutipleSpeed->addItem(QString());
        mutipleSpeed->setObjectName(QString::fromUtf8("mutipleSpeed"));
        QSizePolicy sizePolicy5(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy5.setHorizontalStretch(0);
        sizePolicy5.setVerticalStretch(0);
        sizePolicy5.setHeightForWidth(mutipleSpeed->sizePolicy().hasHeightForWidth());
        mutipleSpeed->setSizePolicy(sizePolicy5);
        mutipleSpeed->setLayoutDirection(Qt::LeftToRight);

        horizontalLayout_4->addWidget(mutipleSpeed);

        muteBtn = new QPushButton(centralwidget);
        muteBtn->setObjectName(QString::fromUtf8("muteBtn"));
        muteBtn->setEnabled(false);
        muteBtn->setMaximumSize(QSize(40, 16777215));
        muteBtn->setFocusPolicy(Qt::NoFocus);
        QIcon icon9;
        icon9.addFile(QString::fromUtf8(":/new/prefix/aloud.png"), QSize(), QIcon::Normal, QIcon::Off);
        muteBtn->setIcon(icon9);
        muteBtn->setFlat(true);

        horizontalLayout_4->addWidget(muteBtn);

        volumnSlider = new VideoSlider(centralwidget);
        volumnSlider->setObjectName(QString::fromUtf8("volumnSlider"));
        volumnSlider->setEnabled(false);
        volumnSlider->setMinimumSize(QSize(100, 0));
        volumnSlider->setMaximumSize(QSize(100, 16777215));
        volumnSlider->setMaximum(100);
        volumnSlider->setValue(100);
        volumnSlider->setOrientation(Qt::Horizontal);

        horizontalLayout_4->addWidget(volumnSlider);

        volumnLabel = new QLabel(centralwidget);
        volumnLabel->setObjectName(QString::fromUtf8("volumnLabel"));
        volumnLabel->setMinimumSize(QSize(30, 0));
        volumnLabel->setMaximumSize(QSize(30, 16777215));
        volumnLabel->setFont(font);
        volumnLabel->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout_4->addWidget(volumnLabel);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_2);

        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setObjectName(QString::fromUtf8("horizontalLayout_8"));
        listBtn = new QPushButton(centralwidget);
        listBtn->setObjectName(QString::fromUtf8("listBtn"));
        listBtn->setEnabled(false);
        QSizePolicy sizePolicy6(QSizePolicy::Minimum, QSizePolicy::Minimum);
        sizePolicy6.setHorizontalStretch(0);
        sizePolicy6.setVerticalStretch(0);
        sizePolicy6.setHeightForWidth(listBtn->sizePolicy().hasHeightForWidth());
        listBtn->setSizePolicy(sizePolicy6);
        listBtn->setFocusPolicy(Qt::NoFocus);
        QIcon icon10;
        icon10.addFile(QString::fromUtf8(":/new/prefix/hideList.png"), QSize(), QIcon::Normal, QIcon::Off);
        listBtn->setIcon(icon10);
        listBtn->setFlat(true);

        horizontalLayout_8->addWidget(listBtn);

        fillBtn = new QPushButton(centralwidget);
        fillBtn->setObjectName(QString::fromUtf8("fillBtn"));
        fillBtn->setFocusPolicy(Qt::NoFocus);
        QIcon icon11;
        icon11.addFile(QString::fromUtf8(":/new/prefix/fill.png"), QSize(), QIcon::Normal, QIcon::Off);
        fillBtn->setIcon(icon11);
        fillBtn->setFlat(true);

        horizontalLayout_8->addWidget(fillBtn);


        horizontalLayout_4->addLayout(horizontalLayout_8);


        horizontalLayout_2->addLayout(horizontalLayout_4);


        horizontalLayout->addLayout(horizontalLayout_2);


        verticalLayout_3->addLayout(horizontalLayout);


        gridLayout_2->addLayout(verticalLayout_3, 1, 0, 1, 1);

        MainWindow->setCentralWidget(centralwidget);

        retranslateUi(MainWindow);

        playWidget->setCurrentIndex(1);
        mutipleSpeed->setCurrentIndex(2);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        openFileBtn->setText(QString());
        openDirBtn->setText(QString());
        addFileBtn->setText(QString());
        addFolderBtn->setText(QString());
        clearListBtn->setText(QString());
        timeLabel->setText(QCoreApplication::translate("MainWindow", "00:00:00", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "/", nullptr));
        durationLabel->setText(QCoreApplication::translate("MainWindow", "00:00:00", nullptr));
        back3sBtn->setText(QString());
        lastMediaBtn->setText(QString());
        playBtn->setText(QString());
        nextMediaBtn->setText(QString());
        speed3sBtn->setText(QString());
        stopBtn->setText(QString());
        mutipleSpeed->setItemText(0, QCoreApplication::translate("MainWindow", "x2", nullptr));
        mutipleSpeed->setItemText(1, QCoreApplication::translate("MainWindow", "x1.5", nullptr));
        mutipleSpeed->setItemText(2, QCoreApplication::translate("MainWindow", "x1", nullptr));
        mutipleSpeed->setItemText(3, QCoreApplication::translate("MainWindow", "x0.5", nullptr));

        muteBtn->setText(QString());
        volumnLabel->setText(QCoreApplication::translate("MainWindow", "100", nullptr));
        listBtn->setText(QString());
        fillBtn->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H

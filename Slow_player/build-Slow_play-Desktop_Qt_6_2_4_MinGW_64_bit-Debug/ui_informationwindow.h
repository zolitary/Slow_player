/********************************************************************************
** Form generated from reading UI file 'informationwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.2.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_INFORMATIONWINDOW_H
#define UI_INFORMATIONWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_informationWindow
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *fileNamelabel;
    QLabel *durationlabel;
    QLabel *bitRatelabel;
    QLabel *namelabel;
    QLabel *pixFmtlabel;
    QLabel *achannelslabel;
    QLabel *asampleRatelabel;

    void setupUi(QWidget *informationWindow)
    {
        if (informationWindow->objectName().isEmpty())
            informationWindow->setObjectName(QString::fromUtf8("informationWindow"));
        informationWindow->resize(400, 444);
        verticalLayout = new QVBoxLayout(informationWindow);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        fileNamelabel = new QLabel(informationWindow);
        fileNamelabel->setObjectName(QString::fromUtf8("fileNamelabel"));

        verticalLayout->addWidget(fileNamelabel);

        durationlabel = new QLabel(informationWindow);
        durationlabel->setObjectName(QString::fromUtf8("durationlabel"));

        verticalLayout->addWidget(durationlabel);

        bitRatelabel = new QLabel(informationWindow);
        bitRatelabel->setObjectName(QString::fromUtf8("bitRatelabel"));

        verticalLayout->addWidget(bitRatelabel);

        namelabel = new QLabel(informationWindow);
        namelabel->setObjectName(QString::fromUtf8("namelabel"));

        verticalLayout->addWidget(namelabel);

        pixFmtlabel = new QLabel(informationWindow);
        pixFmtlabel->setObjectName(QString::fromUtf8("pixFmtlabel"));

        verticalLayout->addWidget(pixFmtlabel);

        achannelslabel = new QLabel(informationWindow);
        achannelslabel->setObjectName(QString::fromUtf8("achannelslabel"));

        verticalLayout->addWidget(achannelslabel);

        asampleRatelabel = new QLabel(informationWindow);
        asampleRatelabel->setObjectName(QString::fromUtf8("asampleRatelabel"));

        verticalLayout->addWidget(asampleRatelabel);


        retranslateUi(informationWindow);

        QMetaObject::connectSlotsByName(informationWindow);
    } // setupUi

    void retranslateUi(QWidget *informationWindow)
    {
        informationWindow->setWindowTitle(QCoreApplication::translate("informationWindow", "Form", nullptr));
        fileNamelabel->setText(QCoreApplication::translate("informationWindow", "TextLabel", nullptr));
        durationlabel->setText(QCoreApplication::translate("informationWindow", "TextLabel", nullptr));
        bitRatelabel->setText(QCoreApplication::translate("informationWindow", "TextLabel", nullptr));
        namelabel->setText(QCoreApplication::translate("informationWindow", "TextLabel", nullptr));
        pixFmtlabel->setText(QCoreApplication::translate("informationWindow", "TextLabel", nullptr));
        achannelslabel->setText(QCoreApplication::translate("informationWindow", "TextLabel", nullptr));
        asampleRatelabel->setText(QCoreApplication::translate("informationWindow", "TextLabel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class informationWindow: public Ui_informationWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_INFORMATIONWINDOW_H

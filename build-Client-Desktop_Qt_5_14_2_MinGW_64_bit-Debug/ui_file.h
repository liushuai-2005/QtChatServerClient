/********************************************************************************
** Form generated from reading UI file 'file.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FILE_H
#define UI_FILE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_File
{
public:
    QHBoxLayout *horizontalLayout;
    QListWidget *listWidget;
    QVBoxLayout *verticalLayout;
    QPushButton *return_PB;
    QPushButton *flush_PB;
    QPushButton *mkdir_PB;
    QPushButton *delete_PB;
    QPushButton *rename_PB;
    QSpacerItem *verticalSpacer;
    QPushButton *upload_PB;
    QPushButton *dowload_PB;
    QPushButton *share_PB;

    void setupUi(QWidget *File)
    {
        if (File->objectName().isEmpty())
            File->setObjectName(QString::fromUtf8("File"));
        File->resize(368, 512);
        horizontalLayout = new QHBoxLayout(File);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 10, 0);
        listWidget = new QListWidget(File);
        listWidget->setObjectName(QString::fromUtf8("listWidget"));

        horizontalLayout->addWidget(listWidget);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        return_PB = new QPushButton(File);
        return_PB->setObjectName(QString::fromUtf8("return_PB"));
        return_PB->setMinimumSize(QSize(150, 0));
        QFont font;
        font.setFamily(QString::fromUtf8("AcadEref"));
        font.setPointSize(14);
        return_PB->setFont(font);

        verticalLayout->addWidget(return_PB);

        flush_PB = new QPushButton(File);
        flush_PB->setObjectName(QString::fromUtf8("flush_PB"));
        flush_PB->setMinimumSize(QSize(150, 0));
        flush_PB->setFont(font);

        verticalLayout->addWidget(flush_PB);

        mkdir_PB = new QPushButton(File);
        mkdir_PB->setObjectName(QString::fromUtf8("mkdir_PB"));
        mkdir_PB->setMinimumSize(QSize(150, 0));
        mkdir_PB->setFont(font);

        verticalLayout->addWidget(mkdir_PB);

        delete_PB = new QPushButton(File);
        delete_PB->setObjectName(QString::fromUtf8("delete_PB"));
        delete_PB->setMinimumSize(QSize(150, 0));
        delete_PB->setFont(font);

        verticalLayout->addWidget(delete_PB);

        rename_PB = new QPushButton(File);
        rename_PB->setObjectName(QString::fromUtf8("rename_PB"));
        rename_PB->setMinimumSize(QSize(150, 0));
        rename_PB->setFont(font);

        verticalLayout->addWidget(rename_PB);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        upload_PB = new QPushButton(File);
        upload_PB->setObjectName(QString::fromUtf8("upload_PB"));
        upload_PB->setMinimumSize(QSize(150, 0));
        upload_PB->setFont(font);

        verticalLayout->addWidget(upload_PB);

        dowload_PB = new QPushButton(File);
        dowload_PB->setObjectName(QString::fromUtf8("dowload_PB"));
        dowload_PB->setMinimumSize(QSize(150, 0));
        dowload_PB->setFont(font);

        verticalLayout->addWidget(dowload_PB);

        share_PB = new QPushButton(File);
        share_PB->setObjectName(QString::fromUtf8("share_PB"));
        share_PB->setMinimumSize(QSize(150, 0));
        share_PB->setFont(font);

        verticalLayout->addWidget(share_PB);


        horizontalLayout->addLayout(verticalLayout);


        retranslateUi(File);

        QMetaObject::connectSlotsByName(File);
    } // setupUi

    void retranslateUi(QWidget *File)
    {
        File->setWindowTitle(QCoreApplication::translate("File", "Form", nullptr));
        return_PB->setText(QCoreApplication::translate("File", "\350\277\224\345\233\236", nullptr));
        flush_PB->setText(QCoreApplication::translate("File", "\345\210\267\346\226\260\346\226\207\344\273\266", nullptr));
        mkdir_PB->setText(QCoreApplication::translate("File", "\345\210\233\345\273\272\346\226\207\344\273\266\345\244\271", nullptr));
        delete_PB->setText(QCoreApplication::translate("File", "\345\210\240\351\231\244\346\226\207\344\273\266", nullptr));
        rename_PB->setText(QCoreApplication::translate("File", "\351\207\215\345\221\275\345\220\215", nullptr));
        upload_PB->setText(QCoreApplication::translate("File", "\344\270\212\344\274\240\346\226\207\344\273\266", nullptr));
        dowload_PB->setText(QCoreApplication::translate("File", "\344\270\213\350\275\275\346\226\207\344\273\266", nullptr));
        share_PB->setText(QCoreApplication::translate("File", "\345\210\206\344\272\253\346\226\207\344\273\266", nullptr));
    } // retranslateUi

};

namespace Ui {
    class File: public Ui_File {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FILE_H

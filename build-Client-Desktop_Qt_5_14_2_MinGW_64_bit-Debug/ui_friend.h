/********************************************************************************
** Form generated from reading UI file 'friend.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FRIEND_H
#define UI_FRIEND_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Friend
{
public:
    QHBoxLayout *horizontalLayout;
    QListWidget *listWidget;
    QVBoxLayout *verticalLayout;
    QPushButton *chat_PB;
    QPushButton *update_PB;
    QPushButton *delete_PB;
    QSpacerItem *verticalSpacer;
    QPushButton *findUser_PB;
    QPushButton *olineUser_PB;

    void setupUi(QWidget *Friend)
    {
        if (Friend->objectName().isEmpty())
            Friend->setObjectName(QString::fromUtf8("Friend"));
        Friend->resize(321, 312);
        horizontalLayout = new QHBoxLayout(Friend);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 10, 0);
        listWidget = new QListWidget(Friend);
        listWidget->setObjectName(QString::fromUtf8("listWidget"));

        horizontalLayout->addWidget(listWidget);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        chat_PB = new QPushButton(Friend);
        chat_PB->setObjectName(QString::fromUtf8("chat_PB"));
        chat_PB->setMinimumSize(QSize(150, 0));
        QFont font;
        font.setFamily(QString::fromUtf8("AcadEref"));
        font.setPointSize(14);
        chat_PB->setFont(font);

        verticalLayout->addWidget(chat_PB);

        update_PB = new QPushButton(Friend);
        update_PB->setObjectName(QString::fromUtf8("update_PB"));
        update_PB->setMinimumSize(QSize(150, 0));
        update_PB->setFont(font);

        verticalLayout->addWidget(update_PB);

        delete_PB = new QPushButton(Friend);
        delete_PB->setObjectName(QString::fromUtf8("delete_PB"));
        delete_PB->setMinimumSize(QSize(150, 0));
        delete_PB->setFont(font);

        verticalLayout->addWidget(delete_PB);

        verticalSpacer = new QSpacerItem(20, 28, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        findUser_PB = new QPushButton(Friend);
        findUser_PB->setObjectName(QString::fromUtf8("findUser_PB"));
        findUser_PB->setMinimumSize(QSize(150, 0));
        findUser_PB->setFont(font);

        verticalLayout->addWidget(findUser_PB);

        olineUser_PB = new QPushButton(Friend);
        olineUser_PB->setObjectName(QString::fromUtf8("olineUser_PB"));
        olineUser_PB->setMinimumSize(QSize(150, 0));
        olineUser_PB->setFont(font);

        verticalLayout->addWidget(olineUser_PB);


        horizontalLayout->addLayout(verticalLayout);


        retranslateUi(Friend);

        QMetaObject::connectSlotsByName(Friend);
    } // setupUi

    void retranslateUi(QWidget *Friend)
    {
        Friend->setWindowTitle(QCoreApplication::translate("Friend", "Form", nullptr));
        chat_PB->setText(QCoreApplication::translate("Friend", "\350\201\212\345\244\251", nullptr));
        update_PB->setText(QCoreApplication::translate("Friend", "\345\210\267\346\226\260\345\245\275\345\217\213", nullptr));
        delete_PB->setText(QCoreApplication::translate("Friend", "\345\210\240\351\231\244\345\245\275\345\217\213", nullptr));
        findUser_PB->setText(QCoreApplication::translate("Friend", "\346\237\245\346\211\276\347\224\250\346\210\267", nullptr));
        olineUser_PB->setText(QCoreApplication::translate("Friend", "\345\234\250\347\272\277\347\224\250\346\210\267", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Friend: public Ui_Friend {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FRIEND_H

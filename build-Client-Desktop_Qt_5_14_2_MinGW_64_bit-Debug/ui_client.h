/********************************************************************************
** Form generated from reading UI file 'client.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CLIENT_H
#define UI_CLIENT_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Client
{
public:
    QWidget *layoutWidget;
    QGridLayout *gridLayout;
    QPushButton *login_PB;
    QLabel *label_2;
    QLineEdit *pwd_LE;
    QSpacerItem *horizontalSpacer;
    QLabel *label;
    QPushButton *regist_PB;
    QLineEdit *name_LE_2;

    void setupUi(QWidget *Client)
    {
        if (Client->objectName().isEmpty())
            Client->setObjectName(QString::fromUtf8("Client"));
        Client->resize(391, 299);
        layoutWidget = new QWidget(Client);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        layoutWidget->setGeometry(QRect(40, 40, 298, 102));
        gridLayout = new QGridLayout(layoutWidget);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setHorizontalSpacing(20);
        gridLayout->setVerticalSpacing(10);
        gridLayout->setContentsMargins(0, 0, 0, 0);
        login_PB = new QPushButton(layoutWidget);
        login_PB->setObjectName(QString::fromUtf8("login_PB"));

        gridLayout->addWidget(login_PB, 2, 3, 1, 1);

        label_2 = new QLabel(layoutWidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        QFont font;
        font.setFamily(QString::fromUtf8("AcadEref"));
        font.setPointSize(14);
        label_2->setFont(font);

        gridLayout->addWidget(label_2, 1, 0, 1, 1);

        pwd_LE = new QLineEdit(layoutWidget);
        pwd_LE->setObjectName(QString::fromUtf8("pwd_LE"));
        pwd_LE->setEchoMode(QLineEdit::Password);

        gridLayout->addWidget(pwd_LE, 1, 1, 1, 3);

        horizontalSpacer = new QSpacerItem(98, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer, 2, 2, 1, 1);

        label = new QLabel(layoutWidget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setFont(font);

        gridLayout->addWidget(label, 0, 0, 1, 1);

        regist_PB = new QPushButton(layoutWidget);
        regist_PB->setObjectName(QString::fromUtf8("regist_PB"));

        gridLayout->addWidget(regist_PB, 2, 0, 1, 1);

        name_LE_2 = new QLineEdit(layoutWidget);
        name_LE_2->setObjectName(QString::fromUtf8("name_LE_2"));

        gridLayout->addWidget(name_LE_2, 0, 1, 1, 3);


        retranslateUi(Client);

        QMetaObject::connectSlotsByName(Client);
    } // setupUi

    void retranslateUi(QWidget *Client)
    {
        Client->setWindowTitle(QCoreApplication::translate("Client", "Client", nullptr));
        login_PB->setText(QCoreApplication::translate("Client", "\347\231\273\345\275\225", nullptr));
        label_2->setText(QCoreApplication::translate("Client", "\345\257\206  \347\240\201\357\274\232", nullptr));
        label->setText(QCoreApplication::translate("Client", "\347\224\250\346\210\267\345\220\215\357\274\232", nullptr));
        regist_PB->setText(QCoreApplication::translate("Client", "\346\263\250\345\206\214", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Client: public Ui_Client {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CLIENT_H

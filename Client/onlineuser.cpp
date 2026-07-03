#include "client.h"
#include "onlineuser.h"
#include "ui_onlineuser.h"

OnlineUser::OnlineUser(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OnlineUser)
{
    ui->setupUi(this);
}

OnlineUser::~OnlineUser()
{
    delete ui;
}

void OnlineUser::updateListWidget(QStringList slName)
{
    ui->listWidget->clear();
    ui->listWidget->addItems(slName);
    //    ui->listWidget->update();
}

void OnlineUser::updateFriend(QStringList slName)
{
    ui->listWidget->clear();
    ui->listWidget->addItems(slName);
}

//添加好友
//所遇到的问题：双击用户名之后弹窗显示是否添加好友，点击Yes之后没有提示并且窗口没有退出，而点击No可以直接退出
//首先我没有在Client.cpp添加case同意添加好友响应
//原因是响应类型写错了
//在Server中的PDU *MsgHandler::handleAgreeAddFriend()有两处错误;
//在reshandler.cpp里的void ResHandler::handleAdjudgeAddFriendResend()

//

void OnlineUser::on_listWidget_itemDoubleClicked(QListWidgetItem *item)
{
    QString strCurName = Client::getInstance().m_strLoginName;
    QString strTarName = item->text();
    PDU* pdu = initPDU(0);
    pdu->uiType = ENUM_TYPE_ADJUDGE_ADD_FRIEND_REQ;
    memcpy(pdu->caData,strCurName.toStdString().c_str(),32);
    memcpy(pdu->caData+32,strTarName.toStdString().c_str(),32);
    Client::getInstance().sendMsg(pdu);
}

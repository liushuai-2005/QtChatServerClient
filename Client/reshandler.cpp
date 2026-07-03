#include "client.h"
#include "index.h"
#include "reshandler.h"
#include <QMessageBox>
#include<string.h>

ResHandler::ResHandler()
{
    
}

void ResHandler::  handleRegist()
{
    bool ret;
    //把服务器返回的结果拷贝到ret里
    //    目标地址   来源地址   拷贝字节数
    memcpy(&ret,pdu->caData,sizeof(bool));
    if(ret){                      //注 查一下这写的对不对
        QMessageBox::information(&Client::getInstance(),"提示","注册成功");
    }else{
        QMessageBox::information(&Client::getInstance(),"提示","注册失败");
    }
}

void ResHandler::handleLogin()
{
    bool ret;
    memcpy(&ret,pdu->caData,sizeof(bool));
    if(ret){
        //登录成功后显示Index界面
        //调用单例 保证整个程序只有一个主界面 全局共享同一个主界面
        Index::getInstance().show();
        Index::getInstance().getFile()->flushFile();
        Index::getInstance().getFriend()->updateFriend();
//                Index::getInstance().show();
        //隐藏登录窗口界面 this表示当前
        Client::getInstance().hide();
    }else{
        QMessageBox::information(&Client::getInstance(),"提示","登录失败");
    }
}

void ResHandler::handleFinderUser()
{
    int ret;
    memcpy(&ret,pdu->caData,sizeof(int));
    if(ret == -1){
        QMessageBox::information(&Index::getInstance(),"提示","该用户不存在");
    }else if(ret == 0){
        QMessageBox::information(&Index::getInstance(),"提示","该用户不在线");
    }else if(ret == 1){
        QMessageBox::information(&Index::getInstance(),"提示","该用户在线");
    }
}

void ResHandler::handleOnlineUser()
{
    //uisize在线用户的个数
    uint uisize = pdu->uiMsgLen/32;    
    QStringList slName;
    slName.clear();
    char caTmp[32] = {'\0'};
    for (uint i = 0;i < uisize;i++) {
        memcpy(caTmp,pdu->caMsg+32*i,32);
        //字符自动转换成QString ?
        slName.append(caTmp);
    }
    Index::getInstance().getFriend()->m_pOnlineUser->updateListWidget(slName);
}

//
void ResHandler::handleAdjudgeAddFriend()
{
    int ret;
    memcpy(&ret,pdu->caData,sizeof(int));
    if(ret == -1){
        QMessageBox::information(&Index::getInstance(),"提示","添加错误");
    }else if(ret == 0){
        QMessageBox::information(&Index::getInstance(),"提示","该用户不在线");
    }else if(ret == -2){
        QMessageBox::information(&Index::getInstance(),"提示","该用户已是您的好友");
    }
}

void ResHandler::handleAdjudgeAddFriendResend()
{
    //
    char caName[32] = {'\0'};
    //拷贝赋值
    memcpy(caName,pdu->caData,32);
    //
    int ret = QMessageBox::question(&Index::getInstance(),"添加好友",QString("是否同意 %1 的添加好友申请？").arg(caName));
    if(ret!=QMessageBox::Yes){
        return;
    }
    PDU* respdu = initPDU(0);
    memcpy(respdu->caData,pdu->caData,64);
    //这里的响应类型也写错了
//    respdu->uiType = ENUM_TYPE_ADJUDGE_ADD_FRIEND_REQ;
    respdu->uiType = ENUM_TYPE_AGREE_ADD_FRIEND_REQ;
    Client::getInstance().sendMsg(respdu);
}


void ResHandler::handleAgreeAddFriend()
{
    bool ret;
    memcpy(&ret,pdu->caData,sizeof(bool));
    if(ret){
        Index::getInstance().getFriend()->updateFriend();
        //这里不用提示
//        QMessageBox::information(&Client::getInstance(),"提示","添加好友成功");
    }else{
        QMessageBox::information(&Index::getInstance(),"提示","添加好友失败");
    }
}

void ResHandler::handleUpdateFriend()
{
    //同在线用户handleOnlineUser()
    //将好友名放到QStringList
    uint uisize = pdu->uiMsgLen/32;
    QStringList slName;
    slName.clear();
    char caTmp[32] = {'\0'};
    for (uint i = 0;i < uisize;i++) {
        memcpy(caTmp,pdu->caMsg+32*i,32);
        slName.append(caTmp);
    }   
    Index::getInstance().getFriend()->updateListWidget(slName);
}

void ResHandler::handleDeleteFriend()
{
    bool ret;
    memcpy(&ret,pdu->caData,sizeof(bool));
    if(ret){      
        Index::getInstance().getFriend()->updateFriend();
    }else{
        QMessageBox::information(&Index::getInstance(),"提示","删除好友失败");
    }
}

void ResHandler::handleChat()
{
    Chat* c = Index::getInstance().getFriend()->m_pChat;
    if(c->isHidden()){
        c->show();
    }
    char caCurName[32] = {'\0'};
    memcpy(caCurName,pdu->caData,32);
    //输入聊天内容后，点击发送，好友那没有出现聊天内容
//    QString strChatMsg = QString("%1:%2").arg(caCurName).arg(strChatMsg);
    //修改过后显示了乱码问号字符，输入中文你好出现bug，英文和数字正常
    QString strChatMsg = QString("%1:%2").arg(caCurName).arg(pdu->caMsg);

    c->updateShow_TE(strChatMsg);
    c->m_strChatName = caCurName;
}

//客户端接受结果，将结果显示给用户
//定义布尔变量，接受服务器返回的结果
//从pdu的caData中拷贝结果并打印
//根据返回值弹窗提示
void ResHandler::handleMkdir()
{
    bool ret;
    memcpy(&ret,pdu->caData,sizeof (bool));
    qDebug() << "handleMkdir ret" <<ret;
    if(ret){
        Index::getInstance().getFile()->flushFile();
        QMessageBox::information(&Index::getInstance(),"提示","创建文件夹成功");
    }else{
        QMessageBox::information(&Index::getInstance(),"提示","创建文件夹失败");
    }
}

//接受消息函数新增case处理刷新文件的响应
//ResHandler定义处理函数并调用
//处理函数中，uiMsgLen除以结构体FileInfo得到个数iCount
//遍历caMsg 赋值到pFileInfo里
void ResHandler::handleFlushFile()
{
    uint iCount = pdu->uiMsgLen / sizeof (FileInfo);
    QList<FileInfo*>pFileInfoList;
    for(uint i = 0;i < iCount;i++){
        FileInfo* pFileInfo = new FileInfo;
        memcpy(pFileInfo,pdu->caMsg + i * sizeof (FileInfo),sizeof (FileInfo));
        pFileInfoList.append(pFileInfo);
    }
    Index::getInstance().getFile()->updateFileList(pFileInfoList);
}

void ResHandler::handleDeleteFile()
{
    bool ret;
    memcpy(&ret,pdu->caData,sizeof (bool));
    qDebug() << "handleDeleteFile ret" << ret;
    if(ret){
        Index::getInstance().getFile()->flushFile();
        QMessageBox::information(&Index::getInstance(),"提示","删除成功");
    }else{
        QMessageBox::information(&Index::getInstance(),"提示","删除失败");
    }
}

void ResHandler::handleRenameFile()
{
    bool ret;
    memcpy(&ret,pdu->caData,sizeof (bool));
    qDebug() << "handleRename ret" <<ret;
    if(ret){
        Index::getInstance().getFile()->flushFile();
        QMessageBox::information(&Index::getInstance(),"提示","重命名文件夹成功");
    }else{
        QMessageBox::information(&Index::getInstance(),"提示","重命名文件夹失败");
    }
}

void ResHandler::handleUploadFileInit()
{
    bool ret;
    memcpy(&ret,pdu->caData,sizeof (bool));
    qDebug() << "handleUploadFileInit ret" <<ret;
    if(ret){
        Index::getInstance().getFile()->flushFile();
        QMessageBox::information(&Index::getInstance(),"提示","上传文件成功");
    }else{
        QMessageBox::information(&Index::getInstance(),"提示","上传文件失败");
    }
}

void ResHandler::handleUploadFileData()
{
    QMessageBox::information(&Index::getInstance(),"提示","上传文件完成");
    Index::getInstance().getFile()->flushFile();
}

void ResHandler::handleDownloadFile(PDU *pdu)
{
    qDebug() << "handleloDownloadFile start";
    int ret;
    memcpy(&ret,pdu->caData,sizeof (int));
    if(ret == 1){
        QMessageBox::information(&Client::getInstance(),"下载文件","已有文件正在下载");
        return;
    }
    if(ret == -1){
        QMessageBox::information(&Client::getInstance(),"下载文件","下载文件失败");
        return;
    }
    qint64 iFileSize;
    memcpy(&iFileSize,pdu->caData + sizeof (int),sizeof (qint64));
    Index::getInstance().getFile()->downloadFile(iFileSize);
}

void ResHandler::handleDownloadFileData(PDU *pdu)
{
    qDebug() << "handleDownloadFileData start";
    Index::getInstance().getFile()->downloadFileData(pdu->caMsg,pdu->uiMsgLen);
}


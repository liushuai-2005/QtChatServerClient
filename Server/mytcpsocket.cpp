#include "msghandler.h"
#include "mytcpserver.h"
#include "mytcpsocket.h"
#include "operatedb.h"
#include "operatedb.h"
#include "protocol.h"

MyTcpSocket::MyTcpSocket()
{
    m_pmh = new MsgHandler;
}

MyTcpSocket::~MyTcpSocket()
{
    delete m_pmh;
}

//服务器进行响应
//服务器接收函数，用switch处理请求，用户名和密码存放在caData中,

void MyTcpSocket::recvMsg()
{
    qDebug() << "recvMsg 接收消息长度" << this->bytesAvailable();
    QByteArray data = this->readAll();
    buffer.append(data);
    while (buffer.size() >= int(sizeof (PDU))) {
        PDU* pdu = (PDU*)buffer.data();
        if(buffer.size() < int(pdu->uiTotalLen)){
            break;
        }
        PDU* respdu = handleMsg(pdu);
        sendMsg(respdu);
        buffer.remove(0,pdu->uiTotalLen);
    }

}

void MyTcpSocket::clientOffline()
{
    OperateDB::getInstance().handleOffline(m_strLoginName.toStdString().c_str());
    MyTcpServer::getInstance().removeSocket(this);
    
}

void MyTcpSocket::sendMsg(PDU *pdu)
{
    if(pdu == NULL){
        return;
    }
    //不加这个为什么没有出现那个查询用户的结果弹窗呢
    this->write((char*)pdu,pdu->uiTotalLen);

    qDebug() << "send msg  uiTotalLen" << pdu->uiTotalLen
             << "uiMsgLen" << pdu->uiMsgLen
             << "uiTYpe" << pdu->uiType
             << "caData" << pdu->caData
             <<"caData + 32" << pdu->caData+32
             <<"caMsg" << pdu->caMsg;
    free(pdu);
    pdu = NULL;
}

PDU *MyTcpSocket::readPDU()
{
    qDebug() << "readMsg 接收消息长度" << this->bytesAvailable();

    // 1. 先读总长度
    uint uiTotalLen = 0;
    this->read((char*)&uiTotalLen, sizeof(uint));

    // 2. 计算消息长度
    uint uiMsgLen = uiTotalLen - sizeof(PDU);

    // 3. 创建PDU
    PDU* pdu = initPDU(uiMsgLen);

    // 4. 读取剩余所有数据
    this->read((char*)pdu + sizeof(uint), uiTotalLen - sizeof(uint));
    return pdu;
}

PDU *MyTcpSocket::handleMsg(PDU *pdu)
{
    if(pdu == NULL){
            return NULL;
        }
    m_pmh->pdu = pdu;
    PDU* respdu = NULL;
    switch (pdu->uiType) {
    case ENUM_TYPE_REGIST_REQ:{
        respdu = m_pmh->handleRegist();
        break;
    }
    case ENUM_TYPE_LOGIN_REQ:{
        respdu = m_pmh->handleLogin(m_strLoginName);
        break;
    }
    case ENUM_TYPE_FIND_USER_REQ:{
        respdu = m_pmh->handleFindUser();
        break;
    }
    case ENUM_TYPE_ONLINE_USER_REQ:{
        respdu = m_pmh->handleOnlineUser();
        break;
    }
    case ENUM_TYPE_ADJUDGE_ADD_FRIEND_REQ:{
        respdu = m_pmh->handleAdjudgeAddFriend();
        break;
    }
    case ENUM_TYPE_AGREE_ADD_FRIEND_REQ:{
        respdu = m_pmh->handleAgreeAddFriend();
        break;
    }
    case ENUM_TYPE_UPDATE_FRIEND_REQ:{
        respdu = m_pmh->handleUpdateFriend();
        break;
    }
    case ENUM_TYPE_DELETE_FRIEND_REQ:{
        respdu = m_pmh->handleDeleteFriend();
        break;
    }
    case ENUM_TYPE_CHAT_REQ:{
        m_pmh->handleChat();
        break;
    }
    case ENUM_TYPE_MKDIR_REQ:{
        respdu = m_pmh->handleMkdir();
        break;
    }
    case ENUM_TYPE_FLUSH_FILE_REQ:{
        respdu = m_pmh->handleFlushFile();
        break;
    }
    case ENUM_TYPE_DELETE_FILE_REQ:{
        respdu = m_pmh->handleDeleteFile();
        break;
    }
    case ENUM_TYPE_RENAME_FILE_REQ:{
        respdu = m_pmh->handleRenameFile();
        break;
    }
    case ENUM_TYPE_UPLOAD_FILE_INIT_REQ:{
        respdu = m_pmh->handleUploadFileInit();
        break;
    }
    case ENUM_TYPE_UPLOAD_FILE_DATA_REQ:{
        respdu = m_pmh->handleUploadFileData();
        break;
    }
    case ENUM_TYPE_DOWNLOAD_FILE_REQ:{
        respdu = m_pmh->handleDownloadFile(pdu);
        break;
    }
    case ENUM_TYPE_DOWNLOAD_FILE_DATA_REQ:{
        respdu = m_pmh->handleDownloadFileData(this);
        break;
    }
    default:
        break;
    }
    return respdu;
}



#include "client.h"
#include "index.h"
#include "protocol.h"
#include "ui_client.h"

#include <QFile>
#include <QDebug>
#include <QHostAddress>
#include <QMessageBox>

Client::Client(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Client)
{
    ui->setupUi(this);
    loadConfig();
//信号和槽都是类的成员函数，
//信号只负责通知，由Qt框架调用槽函数，槽函数处理业务，
//那么Qt如何调用呢 connect中的地址起到了作用，谁发送的，发送了什么信号，需要谁处理，用什么函数处理
        //     发送者          信号           接收者       槽函数
    connect(&m_socket,&QTcpSocket::connected,this,&Client::showConnected);
    connect(&m_socket,&QTcpSocket::readyRead,this,&Client::recvMsg);
    //connectToHostQt提供的客户端连接函数，不接受字符串
    //把字符串IP转换成QHostAddress类型
    m_socket.connectToHost(QHostAddress(m_strIP),m_usPort);
    m_prh = new ResHandler;

}

Client::~Client()
{
    delete ui;
    delete m_prh;
}

void Client::loadConfig()
{
    //创建一个QFile的file对象，以只读的方式打开，   
    QFile file(":/new/prefix1/client.config");
    if(file.open(QIODevice::ReadOnly)){
        //file,readAll()将内容全读出，返回的是字节数组QByteArray,通过QString转成字符串
        QString strdata = QString (file.readAll());
        //split切割字符串，QStringList用多少自动分配多少
        QStringList strList = strdata.split("\r\n");
        m_strIP = strList[0];
        //toUShort()把字符串类型的数字转成无符号短整型数字
        m_usPort = strList[1].toUShort();
        m_strRootPath = strList[2];
        qDebug()<<"strIP"<<m_strIP<<"usPort"<<m_usPort << "m_strRootPath" << m_strRootPath;

        qDebug()<<"strIP"<<m_strIP<<"usPort"<<m_usPort;
        file.close();//释放
    }else{
        qDebug()<<"打开失败";
    }
}

//& 返回Client对象instance本身
//保证程序只有一个客户端实例
Client &Client::getInstance()
{
    //懒汉单例 用到再创建，节约资源
    //局部静态变量 只创建一次
    static Client instance;
    return instance;
}


void Client::sendMsg(PDU* pdu)
{
    if(pdu == NULL){
        return;
    }
    //发总长度大小，也就是发整个结构体大小，有效数据不一定填满结构体，但必须按总长度发送
    m_socket.write((char*)pdu,pdu->uiTotalLen);
    qDebug() << "send msg  uiTotalLen" << pdu->uiTotalLen
             << "uiMsgLen" << pdu->uiMsgLen
             << "uiTYpe" << pdu->uiType
             << "caData" << pdu->caData
             <<"caData + 32" << pdu->caData+32
             <<"caMsg" << pdu->caMsg;
    free(pdu);
    pdu = NULL;
}

PDU *Client::readPDU()
{
    qDebug() << "recvMsg 接收消息长度" << m_socket.bytesAvailable();
    uint uiTotalLen = 0;
    //先读取4字节，拿到长度 存到uiTotalLen里
    m_socket.read((char*)&uiTotalLen,sizeof(uint));
    //uiMsgLen是caMsg柔性数组的大小
    uint uiMsgLen = uiTotalLen - sizeof(PDU);
    PDU* pdu = initPDU(uiMsgLen);
    //前面已经读过结构体中uiTotalLen的大小了，把剩下所有数据读进PDU结构体里
    m_socket.read((char*)pdu+sizeof (uint),uiTotalLen-sizeof (uint));
    return pdu;
}

void Client::handleMsg(PDU *pdu)
{
    qDebug() << "recv msg  uiTotalLen" << pdu->uiTotalLen
             << "uiMsgLen" << pdu->uiMsgLen
             << "uiTYpe" << pdu->uiType
             << "caData" << pdu->caData
             <<"caData +32" << pdu->caData+32
             <<"caMsg" << pdu->caMsg;
    m_prh->pdu = pdu;
    switch (pdu->uiType) {
    //注册失败 和预期不一致
    //两种查错方式：1.日志\输出打印、调试；2.加断点  先找到出错范围，再不断地缩小范围
    //注册的用户名和密码均和输入的一样，并且还有查询语句，但是返回false
    case ENUM_TYPE_REGIST_RES:{
        m_prh->handleRegist();
        break;
    }
    case ENUM_TYPE_LOGIN_RES:{
        m_prh->handleLogin();
        break;
    }
    case ENUM_TYPE_FIND_USER_RES:{
        m_prh->handleFinderUser();
        break;
    }
    case ENUM_TYPE_ONLINE_USER_RES:{
        m_prh->handleOnlineUser();
        break;
    }
    case ENUM_TYPE_ADJUDGE_ADD_FRIEND_RES:{
        m_prh->handleAdjudgeAddFriend();
        break;
    }
    case ENUM_TYPE_ADJUDGE_ADD_FRIEND_REQ:{
        m_prh->handleAdjudgeAddFriendResend();
        break;
    }
    case ENUM_TYPE_AGREE_ADD_FRIEND_RES:{
        m_prh->handleAgreeAddFriend();
        break;
    }
    case ENUM_TYPE_UPDATE_FRIEND_RES:{
        m_prh->handleUpdateFriend();
        break;
    }
    case ENUM_TYPE_DELETE_FRIEND_RES:{
        m_prh->handleDeleteFriend();
        break;
    }    
    case ENUM_TYPE_CHAT_REQ:{
        m_prh->handleChat();
        break;
    }
    case ENUM_TYPE_MKDIR_RES:{
        m_prh->handleMkdir();
        break;
    }
    case ENUM_TYPE_FLUSH_FILE_RES:{
        m_prh->handleFlushFile();
        break;
    }
    case ENUM_TYPE_DELETE_FILE_RES:{
        m_prh->handleDeleteFile();
        break;
    }
    case ENUM_TYPE_RENAME_FILE_RES:{
        m_prh->handleRenameFile();
        break;
    }
    case ENUM_TYPE_UPLOAD_FILE_INIT_RES:{
        m_prh->handleUploadFileInit();
        break;
    }
    case ENUM_TYPE_UPLOAD_FILE_DATA_RES:{
        m_prh->handleUploadFileData();
        break;
    }
    case ENUM_TYPE_DOWNLOAD_FILE_RES:{
        m_prh->handleDownloadFile(pdu);
        break;
    }
    case ENUM_TYPE_DOWNLOAD_FILE_DATA_RES:{
        m_prh->handleDownloadFileData(pdu);
        break;
    }

    default:
        break;
    }
}

//连接成功信号触发时，Qt自动执行该函数
void Client::showConnected()
{
    qDebug()<<"连接服务器成功";
}


//void Client::on_send_PB_clicked()
//{
//    QString strMsg = ui->input_LE->text();
//    qDebug() << "strMSg" << strMsg;
//    PDU* pdu = initPDU(strMsg.toStdString().size());
//    pdu->uiType = ENUM_TYPE_MIN;
//    memcpy(pdu->caData,strMsg.toStdString().c_str(),strMsg.toStdString().size());
//    memcpy(pdu->caMsg,strMsg.toStdString().c_str(),strMsg.toStdString().size());
//    m_socket.write((char*)pdu,pdu->uiTotalLen);
//    qDebug() << "send msg  uiTotalLen" << pdu->uiTotalLen
//             << "uiMsgLen" << pdu->uiMsgLen
//             << "uiTYpe" << pdu->uiType
//             << "caData" << pdu->caData
//             <<"caData + 32" << pdu->caData+32
//             <<"caMsg" << pdu->caMsg;
//    free(pdu);
//    pdu = NULL;

//}

//注册功能
//on_regist_PB_clicked()是Qt自动生成的槽函数，点击完注册按钮就进入这个槽函数
//用户点注册，获取用户名和密码，检查是否合法，打包发给服务器
//服务器接收函数
void Client::on_regist_PB_clicked()
{
    //获取用户名和密码
    QString strName = ui->name_LE_2->text();//text()获取输入框里面输入的文字
    QString strPwd = ui->pwd_LE->text();
    //判别用户名和密码是否为空以及长度的大小
    if(strName.isEmpty() || strPwd.isEmpty()
            ||strName.toStdString().size() > 32
            || strPwd.toStdString().size() > 32){
        QMessageBox::information(this,"提示","用户名或密码长度非法");
        return;
    }
    //创建一个pdu并进行初始化
    PDU* pdu = initPDU(0);
    pdu->uiType = ENUM_TYPE_REGIST_REQ;
    //用户名存放复制在前32字节[0]~[31]
    memcpy(pdu->caData,strName.toStdString().c_str(),32);
    //密码存放复制在后32字节[32]~[63]
    //caData字符串数组名 也就是首元素地址
    memcpy(pdu->caData+32,strPwd.toStdString().c_str(),32);
    //将账号和密码发给服务器
    m_socket.write((char*)pdu,pdu->uiTotalLen);
    //打印日志
    qDebug() << "send msg  uiTotalLen" << pdu->uiTotalLen
             << "uiMsgLen" << pdu->uiMsgLen
             << "uiTYpe" << pdu->uiType
             << "caData" << pdu->caData
             <<"caData +32" << pdu->caData+32
             <<"caMsg" << pdu->caMsg;
    free(pdu);
    pdu = NULL;
}

//
void Client::recvMsg(){
    qDebug() << "recvMsg接受消息长度" <<m_socket.bytesAvailable();
    QByteArray data = m_socket.readAll();
    buffer.append(data);
    while(buffer.size() >= int(sizeof (PDU))){
        PDU* pdu = (PDU*)buffer.data();
        if(buffer.size() < int(sizeof (PDU))){
            break;
        }
        handleMsg(pdu);
        buffer.remove(0,pdu->uiTotalLen);
    }
}

//登录功能
void Client::on_login_PB_clicked()
{
    QString strName = ui->name_LE_2->text();
    QString strPwd = ui->pwd_LE->text();
    if(strName.isEmpty() || strPwd.isEmpty()
            || strName.toStdString().size()>32
            || strPwd.toStdString().size()>32){
        QMessageBox::information(this,"提示","用户名或密码长度非法");
        return;
    }
    //
    m_strLoginName = strName;
    PDU* pdu = initPDU(0);
    pdu->uiType = ENUM_TYPE_LOGIN_REQ;
    memcpy(pdu->caData,strName.toStdString().c_str(),32);
    memcpy(pdu->caData+32,strPwd.toStdString().c_str(),32);
    m_socket.write((char*)pdu,pdu->uiTotalLen);
    qDebug() << "send msg  uiTotalLen" << pdu->uiTotalLen
             << "uiMsgLen" << pdu->uiMsgLen
             << "uiTYpe" << pdu->uiType
             << "caData" << pdu->caData
             <<"caData + 32" << pdu->caData+32
             <<"caMsg" << pdu->caMsg;
    free(pdu);
    pdu = NULL;
}



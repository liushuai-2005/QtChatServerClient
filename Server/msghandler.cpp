#include "msghandler.h"
#include "mytcpserver.h"
#include "operatedb.h"
#include "server.h"
#include<QDebug>
#include <QDir>
MsgHandler::MsgHandler()
{
    
}

PDU *MsgHandler::handleRegist()
{
    char caName[32] = {'\0'};
    char caPwd[32] = {'\0'};
    //取出用户名和密码并复制
    memcpy(caName,pdu->caData,32);
    memcpy(caPwd,pdu->caData+32,32);
    qDebug() << "regist caName" << caName << "caPwd" << caPwd;
    //调用OperateDB中的注册函数 查询用户是否存在
    bool res = OperateDB::getInstance().handleRegist(caName,caPwd);
    qDebug() << "regist res" << res;
    if(res){
        QDir dir;
        bool ret = dir.mkdir(QString("%1/%2").arg(Server::getInstance().m_strRootPath).arg(caName));
        qDebug() << "mkdir ret" << ret;
    }
    PDU* respdu = initPDU(0);
    respdu->uiType = ENUM_TYPE_REGIST_RES;
    memcpy(respdu->caData,&res,sizeof (bool));
    return respdu;
}
//注册
PDU *MsgHandler::handleLogin(QString &strLoginName)
{
    char caName[32] = {'\0'};
    char caPwd[32] = {'\0'};
    memcpy(caName,pdu->caData,32);
    memcpy(caPwd,pdu->caData+32,32);
    qDebug() << "login caName" << caName << "caPwd" << caPwd;
    bool res = OperateDB::getInstance().handleLogin(caName,caPwd);
    qDebug() << "login res" << res;
    if (res){
        strLoginName = caName;
    }

    PDU* respdu = initPDU(0);
    respdu->uiType = ENUM_TYPE_LOGIN_RES;
    memcpy(respdu->caData,&res,sizeof (bool));
    return respdu;
}

PDU *MsgHandler::handleFindUser()
{
    char caName[32] = {'\0'};
    memcpy(caName,pdu->caData,32);
    qDebug() << "find user caName" << caName;
    int res = OperateDB::getInstance().handleFindUser(caName);
    qDebug() << "find user res" << res;
    PDU* respdu = initPDU(0);
    respdu->uiType = ENUM_TYPE_FIND_USER_RES;
    memcpy(respdu->caData,&res,sizeof (int));
    return respdu;
}

PDU *MsgHandler::handleOnlineUser()
{
    //获取数据集里面的在线用户的个数
    QStringList result = OperateDB::getInstance().handleOnlineUser();
    //响应pdu，大小为32*result.size()
    PDU* respdu = initPDU(32*result.size());
    respdu->uiType = ENUM_TYPE_ONLINE_USER_RES;
    //利用循环将用户名放入caMsg中
    for(int i=0;i<result.size();i++){
        memcpy(respdu->caMsg+32*i,result[i].toStdString().c_str(),32);
    }
    return respdu;
}

PDU *MsgHandler::handleAdjudgeAddFriend()
{
    //读取两个用户名并拷贝
    char caCurName[32] = {'\0'};
    char caTarName[32] = {'\0'};
    memcpy(caCurName,pdu->caData,32);
    memcpy(caTarName,pdu->caData+32,32);
    qDebug() << "login caCurName" << caCurName << "caTarName" << caTarName;
    //调用数据库中的处理添加好友函数并且得到返回值
    int res = OperateDB::getInstance().handleAdjudgeAddFriend(caCurName,caTarName);
    if(res == 1){
        MyTcpServer::getInstance().resend(caTarName,pdu);
        return NULL;
    }else{
        //添加失败 构建响应pdu
       PDU* respdu = initPDU(0);
       //响应pdu的类型
       respdu->uiType = ENUM_TYPE_ADJUDGE_ADD_FRIEND_RES;
       memcpy(respdu->caData,&res,sizeof (int));
       return respdu;
    }
}

PDU *MsgHandler::handleAgreeAddFriend()
{
    char caCurName[32] = {'\0'};
    char caTarName[32] = {'\0'};
    memcpy(caCurName,pdu->caData,32);
    memcpy(caTarName,pdu->caData+32,32);
    qDebug() << "handleAgreeAddFriend caCurName" << caCurName << "caTarName" << caTarName;
    bool res = OperateDB::getInstance().handleAgreeAddFriend(caCurName,caTarName);
    qDebug() << "handleAgreeAddFriend res" << res;
    PDU* respdu = initPDU(0);
    //响应pdu的类型
    //原因是响应类型写错了
//    respdu->uiType = ENUM_TYPE_ADJUDGE_ADD_FRIEND_RES;
    respdu->uiType = ENUM_TYPE_AGREE_ADD_FRIEND_RES;
    memcpy(respdu->caData,&res,sizeof (bool));
    //bug caCur是发起好友申请的一端，caTar是被添加好友的一端，caCur发起申请，caCur点击Yes之后，服务器需要告诉caCur添加成功消息
//    MyTcpServer::getInstance().resend(caTarName,respdu);
    MyTcpServer::getInstance().resend(caCurName,respdu);
    return respdu;
}

PDU *MsgHandler::handleUpdateFriend()
{
    char caName[32] = {'\0'};
    memcpy(caName,pdu->caData,32);
    qDebug() << "handleUpdateFriend caName" <<caName;
    
    //同在线用户 PDU *MsgHandler::handleOnlineUser()
    QStringList result = OperateDB::getInstance().handleUpdateFriend(caName);    
    PDU* respdu = initPDU(32*result.size());
    respdu->uiType = ENUM_TYPE_UPDATE_FRIEND_RES;
    //将用户名放到caMSg中
    for(int i = 0;i < result.size();i++){
        memcpy(respdu->caMsg+32*i,result[i].toStdString().c_str(),32);       
    }
    return respdu;
}

PDU *MsgHandler::handleDeleteFriend()
{
    char caCurName[32] = {'\0'};
    char caTarName[32] = {'\0'};
    memcpy(caCurName,pdu->caData,32);
    memcpy(caTarName,pdu->caData+32,32);
    qDebug() << "DeleteFriend" << caCurName << caTarName;
    bool res = OperateDB::getInstance().handleDeleteFriend(caCurName,caTarName);
    qDebug() << "DeleteFriend res" << res;
    PDU* respdu = initPDU(0);
    respdu->uiType = ENUM_TYPE_DELETE_FRIEND_RES;
    memcpy(respdu->caData,&res,sizeof (bool));
    return respdu;
}

void MsgHandler::handleChat()
{

    char caTarName[32] = {'\0'};
    memcpy(caTarName,pdu->caData+32,32);
    qDebug() << "handleChat" << caTarName;
    MyTcpServer::getInstance().resend(caTarName,pdu);
}

//socket的处理消息函数中新增case
//MsgHandler类新增处理函数并调用
//处理函数中文件夹路径和文件夹名字拼接
//创建文件夹，返回值为bool类型
//构建响应pdu，结果放入cadata中，类型赋值，返回响应pdu
PDU *MsgHandler::handleMkdir()
{
    QString strPath = QString("%1/%2").arg(pdu->caMsg).arg(pdu->caData);
    qDebug() << "handlemkdir strPath" << strPath;
    QDir dir;
    bool ret = dir.mkdir(strPath);
    qDebug() << "handlemkdir ret" << ret;
    PDU* respdu = initPDU(0);
    respdu->uiType = ENUM_TYPE_MKDIR_RES;
    memcpy(respdu->caData,&ret,sizeof (bool));
    return respdu;
}

//用客户端传来的路径初始化QDir
//获取当前目录下的全部文件及文件夹entryInfoList
//创建响应pdu，长度为获取全部文件的个数*FileInfo的大小
//响应pdu赋值类型
//循环遍历文件判断是文件还是文件夹，文件名放入结构体的caName中，打印
//返回
PDU *MsgHandler::handleFlushFile()
{
    QDir dir(pdu->caMsg);
    QFileInfoList fileInfoList = dir.entryInfoList();
    PDU* respdu = initPDU((fileInfoList.size() - 2) * sizeof (FileInfo));
    respdu->uiType = ENUM_TYPE_FLUSH_FILE_RES;
    for(int i = 0,j = 0;i < fileInfoList.size();i++){
        if(fileInfoList[i].fileName() == "." || fileInfoList[i].fileName() == ".."){
            continue;
        }
        FileInfo* pFileInfo = (FileInfo*)respdu->caMsg + j++;
        if(fileInfoList[i].isDir()){
            pFileInfo->iFileType = 0;
        }else{
            pFileInfo->iFileType = 1;
        }
        memcpy(pFileInfo->caName,fileInfoList[i].fileName().toStdString().c_str(),32);
        qDebug() << "pFileInfo->caName" << pFileInfo->caName;
    }
    return respdu;
}

//从caMsg中取出完整删除路径
//定义变量ret存放删除返回值
//判断是文件还是文件夹 如果是文件夹递归删除removeRecursively()；文件的话删文件remove,打印
//创建响应pdu，类型赋值
//把删除结果拷贝到caData中
//返回
PDU *MsgHandler::handleDeleteFile()
{
    QFileInfo fileInfo(pdu->caMsg);
    int ret;
    if(fileInfo.isDir()){
        QDir dir(pdu->caMsg);
        ret = dir.removeRecursively();
    }else {
        QFile file;
        ret = file.remove(pdu->caMsg);
    }
    qDebug() << "handleDeleteFile ret" << ret;
    PDU* respdu = initPDU(0);
    respdu->uiType = ENUM_TYPE_DELETE_FILE_RES;
    memcpy(respdu->caData,&ret,sizeof (bool));
    return respdu;
}

//在接受消息函数中，用switch语句根据类型处理重命名请求
//从pdu的caData中取出旧文件名和新文件名
//从caMsg和文件名中拼接旧文件路径和新文件路径
//调用rename重命名
//构建响应pdu，赋值类型
//将获取的结果放入respdu的caData中
//返回
PDU *MsgHandler::handleRenameFile()
{
    char caOldName[32] = {'\0'};
    char caNewName[32] = {'\0'};
    memcpy(caOldName,pdu->caData,32);
    memcpy(caNewName,pdu->caData + 32,32);
    QString strOldPath = QString("%1/%2").arg(pdu->caMsg).arg(caOldName);
    QString strNewPath = QString("%1/%2").arg(pdu->caMsg).arg(caNewName);
    QDir dir;
    bool ret = dir.rename(strOldPath,strNewPath);
    PDU* respdu = initPDU(0);
    respdu->uiType = ENUM_TYPE_RENAME_FILE_RES;
    memcpy(respdu->caData,&ret,sizeof (bool));
    return respdu;
}

//从caData中取出文件名
//成员变量文件大小先清0
//从caData中取出文件总大小拷贝到成员变量m_iUploadFileSize
//拼接完整存储路径
//成员变量绑定文件路径
//初始化已接受文件大小
//创建文件以只写的模式打开并返回结果
//构建响应pdu,对协议类型赋值
//将结果拷贝到响应pdu的caData中
//返回
PDU *MsgHandler::handleUploadFileInit()
{
    char caFileName[32] = {'\0'};
    memcpy(caFileName,pdu->caData,32);
    m_iUploadFileSize = 0;
    memcpy(&m_iUploadFileSize,pdu->caData + 32,sizeof (qint64));
    QString strPath = QString("%1/%2").arg(pdu->caMsg).arg(caFileName);
    m_fUploadFile.setFileName(strPath);
    m_iUploadFileReceived = 0;
    bool ret = m_fUploadFile.open(QIODevice::WriteOnly);
    PDU* respdu  = initPDU(0);
    respdu->uiType = ENUM_TYPE_UPLOAD_FILE_INIT_RES;
    memcpy(respdu->caData,&ret,sizeof (bool));
    return respdu;
}

//将caMsg写入文件，更新大小
//累加已上传的文件大小
//判断文件是否完全上传
//关闭文件
//构建响应pdu，类型赋值，返回
PDU *MsgHandler::handleUploadFileData()
{
    m_fUploadFile.write(pdu->caMsg,pdu->uiMsgLen);
    m_iUploadFileReceived += pdu->uiMsgLen;
    if(m_iUploadFileReceived < m_iUploadFileSize){
        return NULL;
    }
    m_fUploadFile.close();
    PDU* respdu = initPDU(0);
    respdu->uiType = ENUM_TYPE_UPLOAD_FILE_DATA_RES;
    return respdu;
}

//构建响应pdu,类型赋值
//判断是否正在下载文件
//从caMsg中取出文件全路径，获取大小
//绑定路径，以只读的方式打开
//把结果和文件大小放入caData中
PDU *MsgHandler::handleDownloadFile(PDU *pdu)
{
    qDebug() << "handleDownloadFile start";
    PDU* respdu = initPDU(0);
    respdu->uiType = ENUM_TYPE_DOWNLOAD_FILE_RES;
    int ret = 0;
    if(m_bDownload){
        ret = 1;
        memcpy(respdu->caData,&ret,sizeof (int));
        return respdu;
    }
    QFileInfo fileInfo(pdu->caMsg);
    qint64 fileSize = fileInfo.size();
    qDebug() << "handleDownloadFile pdu->caMsg" << pdu->caMsg;
    m_fDownloadFile.setFileName(pdu->caMsg);
    if(m_fDownloadFile.open(QIODevice::ReadOnly)){
        m_bDownload = true;
    } else {
        ret = -1;
    }
    qDebug() << "handleDownloadFile ret" << ret;
    memcpy(respdu->caData,&ret,sizeof (int));
    memcpy(respdu->caData+sizeof (int),&fileSize,sizeof (qint64));
    qDebug() << "handleDownloadFile end";
    return respdu;
}

//构建响应pdu，类型赋值

PDU *MsgHandler::handleDownloadFileData(MyTcpSocket *socket)
{
    qDebug() << "handleDownloadFileData start";
    PDU* respdu = initPDU(4096);
    respdu->uiType = ENUM_TYPE_DOWNLOAD_FILE_DATA_RES;
    qint64 ret = 0;
    while (true) {
        ret = m_fDownloadFile.read(respdu->caMsg,4096);
        if(ret <= 0){
            break;
        }
        respdu->uiMsgLen = ret;
        respdu->uiTotalLen = ret + sizeof (PDU);
        socket->sendMsg(respdu);
        respdu = initPDU(4096);
    }
    m_bDownload = false;
    m_fDownloadFile.close();
    free(respdu);
    respdu = NULL;
    qDebug() << "handleDownloadFileData end";
    return NULL;
}



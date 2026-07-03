#ifndef MSGHANDLER_H
#define MSGHANDLER_H

//#include "mytcpsocket.h"
#include "protocol.h"
#include<string>
#include<stdlib.h>
#include <QString>
#include <QFile>
class MyTcpSocket;

class MsgHandler
{
public:
    PDU* pdu;
    qint64 m_iUploadFileSize;
    qint64 m_iUploadFileReceived;
    QFile m_fUploadFile;
    MsgHandler();
    PDU* handleRegist();
    PDU* handleLogin(QString& strLoginName);
    PDU* handleFindUser();
    PDU* handleOnlineUser();
    PDU* handleAdjudgeAddFriend();
    PDU* handleAgreeAddFriend();
    PDU* handleUpdateFriend();
    PDU* handleDeleteFriend();
    void handleChat();
    PDU* handleMkdir();
    PDU* handleFlushFile();
    PDU* handleDeleteFile();
    PDU* handleRenameFile();
    PDU* handleUploadFileInit();
    PDU* handleUploadFileData();
    PDU* handleDownloadFile(PDU* pdu);
    PDU* handleDownloadFileData(MyTcpSocket* socket);
private:
    QFile m_fDownloadFile;
    bool m_bDownload;
};

#endif // MSGHANDLER_H

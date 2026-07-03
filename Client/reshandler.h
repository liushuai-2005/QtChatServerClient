#ifndef RESHANDLER_H
#define RESHANDLER_H

#include "protocol.h"



class ResHandler
{
public:
    ResHandler();
    PDU* pdu;
    void handleRegist();
    void handleLogin();
    void handleFinderUser();
    void handleOnlineUser();
    //添加好友
    void handleAdjudgeAddFriend();
    void handleAdjudgeAddFriendResend();
    void handleAgreeAddFriend();
    void handleUpdateFriend(); 
    void handleDeleteFriend();
    void handleChat();
    void handleMkdir();
    void handleFlushFile();
    void handleDeleteFile();
    void handleRenameFile();
    void handleUploadFileInit();
    void handleUploadFileData();
    void handleDownloadFile(PDU* pdu);
    void handleDownloadFileData(PDU* pdu);
};

#endif // RESHANDLER_H

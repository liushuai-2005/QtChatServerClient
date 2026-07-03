#ifndef MYTCPSOCKET_H
#define MYTCPSOCKET_H

//#include "msghandler.h"
#include "protocol.h"

#include <QObject>
#include <qtcpsocket.h>
class MsgHandler;

class MyTcpSocket : public QTcpSocket
{
    Q_OBJECT
public:
    MyTcpSocket();
    ~MyTcpSocket();
    void recvMsg();
    QString m_strLoginName;
    QByteArray buffer;
    void clientOffline();
    void sendMsg(PDU* pdu);
    PDU* readPDU();
    PDU* handleMsg(PDU* pdu);

    MsgHandler*  m_pmh;
    
};

#endif // MYTCPSOCKET_H

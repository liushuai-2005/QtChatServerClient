#ifndef CLIENT_H
#define CLIENT_H

#include "protocol.h"
#include "reshandler.h"

#include <QTcpSocket>
#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class Client; }
QT_END_NAMESPACE

class Client : public QWidget
{
    Q_OBJECT

public:
    ~Client();
    void loadConfig();
    //静态成员函数不属于任何对象，属于类本身
    static Client& getInstance();
    QString m_strLoginName;
    QString m_strRootPath;
    QByteArray buffer;
    PDU* readPDU();
    void handleMsg(PDU* pdu);
    ResHandler* m_prh;
    


public slots:
    void showConnected();
    void recvMsg();
    void sendMsg(PDU* pdu);

private slots:

//    void on_send_PB_clicked();

    void on_regist_PB_clicked();

    void on_login_PB_clicked();

private:
    Client(QWidget *parent = nullptr);
    //删除以确保唯一
    //删掉拷贝构造和赋值重载以确保单例
    Client(const Client& instance) = delete;
    //防止已存在的对象互相拷贝
    Client& operator = (const Client&) = delete;
    Ui::Client *ui;
    //自定义成员变量
    QString m_strIP;
    quint16 m_usPort;
    //m_socket变量是客户端和服务器 通信 的工具
    QTcpSocket m_socket;

};
#endif // CLIENT_H

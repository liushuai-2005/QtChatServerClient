#include "operatedb.h"
#include<QSqlError>
#include<QDebug>
#include <QSqlQuery>
OperateDB &OperateDB::getInstance()
{
    static OperateDB instance;
    return instance;
}

void OperateDB::connect()
{
    m_db.setHostName("localhost");
    m_db.setPort(3306);
    m_db.setUserName("root");
    m_db.setPassword("123456");
    m_db.setDatabaseName("mydb2509");
    if(m_db.open()){
        qDebug() << "数据库连接成功";
    }else{
        qDebug() << "数据库连接失败" << m_db.lastError().text();
    }
}

OperateDB::~OperateDB()
{
    m_db.close();
}

//
bool OperateDB::handleRegist(char *caName, const char *caPwd)
{
    //提前退出
    //对指针参数判空，构建查询用户是否存在的sql语句
    if(caName == NULL || caPwd == NULL){
        return false;
    }
//    QString sql = QString("select * from user_info where name = %1").arg(caName);
    QString sql = QString("select * from user_info where name = '%1'").arg(caName);
    qDebug() << "regist select sql" << sql;
    QSqlQuery q;
    //next 结果集 不成功执行    
    if(!q.exec(sql) || q.next()){
        return false;
    }
//    sql = QString("insert into user_info(name,pwd) values(%1,%2)").arg(caName).arg(caPwd);
    //修正 对查询语句的姓名  加上引号
    sql = QString("insert into user_info(name,pwd) values('%1','%2')").arg(caName).arg(caPwd);
    qDebug() << "regist insert sql" << sql;
    return q.exec(sql);//
}

bool OperateDB::handleLogin(char *caName, const char *caPwd)
{
    //提前退出
    if(caName == NULL || caPwd == NULL){
        return false;
    }
    QString sql = QString("select * from user_info where name = '%1' and pwd = '%2'").arg(caName).arg(caPwd);
    qDebug() << "login select sql" << sql;
    QSqlQuery q;
    if(!q.exec(sql) || !q.next()){
        return false;
    }

    sql = QString("update user_info set online = 1 where name = '%1'").arg(caName);
    qDebug() << "login update sql" << sql;
    return q.exec(sql);
}

void OperateDB::handleOffline(const char *caName)
{
    //变量是指针，先判空
    if(caName == NULL){
        return;
    }
    QString sql = QString("update user_info set online = 0 where name = '%1'").arg(caName);
    qDebug() << "offline update sql" << sql;
    QSqlQuery q;
    q.exec(sql);
}

int OperateDB::handleFindUser(const char *caName)
{
    if(caName == NULL){
        return -2;
    }
    QString sql = QString("select online from user_info where name = '%1'").arg(caName);
    qDebug() << "finderUser select sql" << sql;
    QSqlQuery q;
    q.exec(sql);
    if(q.next()){
        return q.value(0).toInt();
    }
    return -1;
}

QStringList OperateDB::handleOnlineUser()
{
    QString sql = QString("select name from user_info where online = 1");
    qDebug() << "OnlineUser select sql" << sql;
    QSqlQuery q;
    q.exec(sql);
    QStringList result;
    result.clear();
    while(q.next()){
        result.append(q.value(0).toString());
    }
    return result;
}

int OperateDB::handleAdjudgeAddFriend(const char *caCurName, const char *caTarName)
{
    if(caCurName == NULL || caTarName == NULL){
        return -1;
    }
    QString sql = QString(R"(
                          select * from friend where
                          (
                          user_id=(select id from user_info where name = '%1')
                          and
                          friend_id=(select id from user_info where name = '%2')
                          )
                          or
                          (
                          user_id=(select id from user_info where name = '%2')
                          and
                          friend_id=(select id from user_info where name = '%1')
                          );
                          )").arg(caCurName).arg(caTarName);
    qDebug() << "handleAdjudgeAddFriend select friend:" << sql;
    QSqlQuery q;
    q.exec(sql);
    if(q.next()){
        return -2;//已经是好友
    }
    sql = QString("select online from user_info where name = '%1'").arg(caTarName);
    qDebug() << "handleAdjudgeAddFriend select online:" << sql;
    q.exec(sql);
    if(q.next()){
        return q.value(0).toInt();
    }
    return 1;
}

bool OperateDB::handleAgreeAddFriend(const char *caCurName, const char *caTarName)
{
    QString sql = QString(R"(
                          insert into friend(user_id,friend_id)
                          select u1.id,u2.id
                          from user_info u1, user_info u2
                          where u1.name = '%1' and u2.name = '%2';
                          )").arg(caCurName).arg(caTarName);
    qDebug() << "handleAgreeAddFriend select friend:" << sql;
    QSqlQuery q;
    return q.exec(sql);
}

QStringList OperateDB::handleUpdateFriend(const char *caName)
{
    QStringList result;
    result.clear();
    if(caName == NULL){
        return result;
    }
    QString sql = QString(R"(select name from user_info
                          where online = 1 and id in (
                          select user_id  from friend where friend_id =
                          (select id from user_info where name = '%1')
                          union
                          select friend_id from friend where user_id =
                          (select id from user_info where name = '%1')
                          ))").arg(caName);
    qDebug() << "handleUpdateFriend select sql" << sql;
    QSqlQuery q;
    q.exec(sql);    
    while(q.next()){
        result.append(q.value(0).toString());
    }
    return result;
}

bool OperateDB::handleDeleteFriend(const char *caCurName, const char *caTarName)
{
    if(caCurName == NULL || caTarName == NULL){
        return false;
    }
    QString sql = QString(R"(
                          delete from friend where 
                          (
                          user_id = (select id from user_info where name = '%1')
                          and
                          friend_id = (select id from user_info where name = '%2')
                          )
                          or
                          (
                          user_id = (select id from user_info where name = '%2')
                          and
                          friend_id = (select id from user_info where name = '%1')
                          )
                          )").arg(caCurName).arg(caTarName);
    QSqlQuery q;
    return q.exec(sql);
}

OperateDB::OperateDB(QObject *parent) : QObject(parent)
{
    m_db = QSqlDatabase::addDatabase("QMYSQL");
}

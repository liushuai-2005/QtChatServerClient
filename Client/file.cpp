#include "client.h"
#include "file.h"
#include "ui_file.h"
#include "uploader.h"

#include <QFileDialog>
#include <QInputDialog>
#include <QMessageBox>

File::File(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::File)
{
    ui->setupUi(this);
    m_strUserPath = QString("%1/%2").arg(Client::getInstance().m_strRootPath).arg(Client::getInstance().m_strLoginName);
    m_strCurPath = m_strUserPath;
    flushFile();
}

File::~File()
{
    delete ui;
}

//刷新文件转到槽函数
//定义一个刷新文件函数，将刷新内容封装
//构建pdu
//协议新增刷新文件的请求和响应类型并赋值
//将文件当前路径复制到caMsg中
//发送pdu
void File::flushFile()
{
    PDU* pdu = initPDU(m_strCurPath.toStdString().size() + 1);
    pdu->uiType = ENUM_TYPE_FLUSH_FILE_REQ;
    memcpy(pdu->caMsg,m_strCurPath.toStdString().c_str(),m_strCurPath.toStdString().size());
    Client::getInstance().sendMsg(pdu);

}

//上传文件
//绑定文件路径
//以只读的方式打开，打开失败弹窗提示
//循环读取文件内容，构建pdu，类型赋值
//读取的返回值为0时，结束循环，小于0时，弹窗提示
//更新pdu的消息长度和总长度
//发送给服务器 循环结束关闭文件
void File::uploadFile()
{
    Uploader* uploader = new Uploader(m_strUploadFilePath);
    connect(uploader,&Uploader::errorMsg,this,&File::uploadErrorBox,Qt::QueuedConnection);
    connect(uploader,&Uploader::uploadPDU,&Client::getInstance(),&Client::sendMsg,Qt::QueuedConnection);
    uploader->start();
}

//释放上一轮旧数据内存
//
void File::updateFileList(QList<FileInfo *> pFileInfoList)
{
    foreach(FileInfo* pFileInnfo,m_pFileInfoList){
        delete pFileInnfo;
    }
    m_pFileInfoList.clear();
    m_pFileInfoList = pFileInfoList;
    ui->listWidget->clear();
    foreach(FileInfo * pFileInfo,pFileInfoList){
        QListWidgetItem* pItem = new QListWidgetItem;
        if(pFileInfo->iFileType == 0){
            pItem->setIcon(QIcon(QPixmap(":/dir.png")));
        }else {
            pItem->setIcon(QIcon(QPixmap(":/file.png")));
        }
        pItem->setText(pFileInfo->caName);
        ui->listWidget->addItem(pItem);
    }
}

void File::downloadFile(qint64 iFileSize)
{
    qDebug() << "File::downloadFile start";
        m_iDownloadReceived = 0;
        m_iDownloadTotal = iFileSize;
        m_bDownload = true;
        m_DownloadFile.setFileName(m_strSaveFilePath);
        if (!m_DownloadFile.open(QIODevice::WriteOnly)) {
            QMessageBox::warning(this, "下载文件", "打开文件失败");
        }
        PDU *pdu = initPDU(0);
        pdu->uiType = ENUM_TYPE_DOWNLOAD_FILE_DATA_REQ;
        Client::getInstance().sendMsg(pdu);
        qDebug() << "File::downloadFile end";
}

void File::downloadFileData(char *buffer, int size)
{
    m_DownloadFile.write(buffer,size);
    m_iDownloadReceived += size;
    if(m_iDownloadReceived < m_iDownloadTotal){
        return;
    }
    m_DownloadFile.close();
    m_bDownload = false;
    QMessageBox::information(this,"下载文件","下载文件完成");
}

void File::uploadErrorBox(const QString &msg)
{
    QMessageBox::information(this,"提示",msg);
}

//创建文件夹
//转到槽函数，窗口提示，输入新建文件夹名字
//判断输入的名字是否合法
//构建pdu，协议中新增两个创建文件夹类型并赋值
//将文件夹名字放入cadata中，文件路径放入caMsg中
//发送消息给服务器
void File::on_mkdir_PB_clicked()
{
    QString strDirName = QInputDialog::getText(this,"新建文件夹","新建文件夹名字:");
    if(strDirName.isEmpty() || strDirName.size() > 32){
        QMessageBox::information(this,"提示","文件夹名字不合法");
        return;
    }
    PDU* pdu = initPDU(m_strCurPath.toStdString().size() + 1);
    pdu->uiType = ENUM_TYPE_MKDIR_REQ;
    memcpy(pdu->caData,strDirName.toStdString().c_str(),32);
    memcpy(pdu->caMsg,m_strCurPath.toStdString().c_str(),m_strCurPath.toStdString().size());
    Client::getInstance().sendMsg(pdu);
}

void File::on_flush_PB_clicked()
{
    flushFile();
}

//删除按钮转到槽函数
//通过ui获取当前项，判断当前项是否为空
//弹窗提示是否删除文件
//拼接完整路径
//构建pdu，将路径放入caMsg中
//新增删除文件的请求和响应类型，并赋值
//路径放到caMsg中
//发送消息
void File::on_delete_PB_clicked()
{
    QListWidgetItem* pItem = ui->listWidget->currentItem();
    if(!pItem){
        return;
    }
    int ret = QMessageBox::question(this,"提示",QString("是否要删除 %1 文件").arg(pItem->text()));
    if(ret != QMessageBox::Yes){
        return;
    }
    QString strPath = QString("%1/%2").arg(m_strCurPath).arg(pItem->text());
    PDU* pdu = initPDU(strPath.toStdString().size() + 1);
    pdu->uiType = ENUM_TYPE_DELETE_FILE_REQ;
    memcpy(pdu->caMsg,strPath.toStdString().c_str(),strPath.toStdString().size());
    Client::getInstance().sendMsg(pdu);
}

//重命名按钮转到槽函数
//通过ui获取当前项，判断当前项是否为空
//弹窗输入新文件名
//构建pdu，新增请求和响应类型，并赋值
//当前项的原文件名和新文件名放入caData中，当前路径放入caMsg中
//发送
void File::on_rename_PB_clicked()
{
    QListWidgetItem* pItem = ui->listWidget->currentItem();
    if(!pItem){
        return;
    }
    QString strNewName = QInputDialog::getText(this,"重命名","新文件名:");
    PDU* pdu = initPDU(m_strCurPath.toStdString().size() + 1);
    pdu->uiType = ENUM_TYPE_RENAME_FILE_REQ;
    memcpy(pdu->caData,pItem->text().toStdString().c_str(),32);
    memcpy(pdu->caData + 32,strNewName.toStdString().c_str(),32);
    memcpy(pdu->caMsg,m_strCurPath.toStdString().c_str(),m_strCurPath.toStdString().size());
    Client::getInstance().sendMsg(pdu);
}

//遍历文件信息链表
//判断选中的文件是否存在并且是否是文件夹
//拼接新路径原当前路径和文件夹名，赋值更新m_strCurPath
//调用刷新文件函数
void File::on_listWidget_itemDoubleClicked(QListWidgetItem *item)
{
    foreach(FileInfo* pFileInfo,m_pFileInfoList){
        if(pFileInfo->caName == item->text() && pFileInfo->iFileType != 0){
            return;
        }
    }
    m_strCurPath = QString("%1/%2").arg(m_strCurPath).arg(item->text());
    flushFile();
}

//判断当前路径是否等于用户根目录
//查找路径的最后一个'/'的下标
//删除最后一级文件夹路径
//调用刷新函数
void File::on_return_PB_clicked()
{
    if(m_strCurPath == m_strUserPath){
        return;
    }
    int index = m_strCurPath.lastIndexOf('/');
    m_strCurPath.remove(index,m_strCurPath.size() - index);
    flushFile();
}

//弹出文件选择框,路径作为成员变量存下来
//判空
//通过lastIndexOf()获取最后一个'/'的下标
//right截取'/'后面的文件名
//获取文件大小
//构建pdu，新增协议类型并赋值
//将文件名和文件大小放入caData的caData中，将当前路径放入caMsg中
//发送给服务器
void File::on_upload_PB_clicked()
{
    m_strUploadFilePath.clear();
    m_strUploadFilePath = QFileDialog::getOpenFileName();
    qDebug() << "m_strUploadFilePath" << m_strUploadFilePath;
    if(m_strUploadFilePath.isEmpty()){
        return;
    }
    int index = m_strUploadFilePath.lastIndexOf('/');
    QString strFileName = m_strUploadFilePath.right(m_strUploadFilePath.size() - index - 1);
    QFile file(m_strUploadFilePath);
    qint64 iFileSize = file.size();
    PDU* pdu = initPDU(m_strCurPath.toStdString().size() + 1);
    pdu->uiType = ENUM_TYPE_UPLOAD_FILE_INIT_REQ;
    memcpy(pdu->caData,strFileName.toStdString().c_str(),32);
    memcpy(pdu->caData + 32,&iFileSize,sizeof (qint64));
    memcpy(pdu->caMsg,m_strCurPath.toStdString().c_str(),m_strCurPath.toStdString().size());
    Client::getInstance().sendMsg(pdu);
}

void File::on_dowload_PB_clicked()
{
    if(m_bDownload){
        QMessageBox::warning(this,"下载","已有文件下载");
        return;
    }
    QListWidgetItem* pItem = ui->listWidget->currentItem();
    if(pItem == NULL){
        QMessageBox::warning(this,"下载文件","请选择要下载的文件");
        return;
    }
    m_strSaveFilePath.clear();
    m_strSaveFilePath = QFileDialog::getSaveFileName();
    if(m_strSaveFilePath.isEmpty()){
        QMessageBox::warning(this,"下载文件","请指定下载路径:");
        return;
    }
    m_strDownloadfileName = pItem->text();
    QString strPath = QString("%1/%2").arg(m_strCurPath).arg(m_strDownloadfileName);
    PDU* pdu = initPDU(strPath.size() + 1);
    pdu->uiType = ENUM_TYPE_DOWNLOAD_FILE_REQ;
    memcpy(pdu->caMsg,strPath.toStdString().c_str(),strPath.size());
    Client::getInstance().sendMsg(pdu);
}

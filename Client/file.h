#ifndef FILE_H
#define FILE_H

#include "protocol.h"

#include <QFile>
#include <QListWidget>
#include <QWidget>

namespace Ui {
class File;
}

class File : public QWidget
{
    Q_OBJECT

public:
    QString m_strUserPath;
    QString m_strCurPath;
    QString m_strUploadFilePath;
    QList<FileInfo*> m_pFileInfoList;
    explicit File(QWidget *parent = nullptr);
    ~File();
    void flushFile();
    void uploadFile();
    void updateFileList(QList<FileInfo*> pFileInfoList);
    void downloadFile(qint64 iFileSize);
    void downloadFileData(char* buffer,int size);


public slots:
    void uploadErrorBox(const QString& msg);
private slots:
    void on_mkdir_PB_clicked();

    void on_flush_PB_clicked();

    void on_delete_PB_clicked();

    void on_rename_PB_clicked();

    void on_listWidget_itemDoubleClicked(QListWidgetItem *item);

    void on_return_PB_clicked();

    void on_upload_PB_clicked();

    void on_dowload_PB_clicked();

private:
    Ui::File *ui;
    bool m_bDownload;
    QString m_strSaveFilePath;
    QString m_strDownloadfileName;
    QFile m_DownloadFile;
    qint64 m_iDownloadReceived;
    qint64 m_iDownloadTotal;
};

#endif // FILE_H

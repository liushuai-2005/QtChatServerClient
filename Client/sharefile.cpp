#include "index.h"
#include "sharefile.h"
#include "ui_sharefile.h"

ShareFile::ShareFile(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ShareFile)
{
    ui->setupUi(this);
}

ShareFile::~ShareFile()
{
    delete ui;
}

void ShareFile::updateFriend_LW()
{
    ui->friend_LW->clear();
}

void ShareFile::on_allSelect_PB_clicked()
{
    for (int i = 0;i < ui->friend_LW->count();++i) {
        ui->friend_LW->item(i)->setSelected(true);
    }
}

void ShareFile::on_cancelAllSelect_PB_clicked()
{
    for (int i = 0;i < ui->friend_LW->count();++i) {
        ui->friend_LW->item(i)->setSelected(false);
    }
}

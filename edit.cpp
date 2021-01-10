#include "edit.h"
#include "ui_edit.h"

extern CANVAS_ELEM** atlas_array;

namespace Ui {
class Edit;
}

Edit::Edit(QWidget* parent):
    QDialog(parent),
    ui(new Ui::Edit)
{
    ui->setupUi(this);
    setWindowTitle(tr("Edit Atlas"));
    this->resize(1000,1000);

    ui->pushButton->resize(500,100);
    ui->pushButton_2->resize(500,100);
    ui->pushButton_3->resize(500,100);
    ui->pushButton_4->resize(500,100);
    ui->pushButton_5->resize(500,100);

    ai_hint=false;

}

Edit::~Edit()
{
    delete ui;
}



void Edit::on_pushButton_clicked()
{
    ui->pushButton->setEnabled(false);
    emit edit_atlas();
    ui->pushButton->setEnabled(true);
}

void Edit::on_pushButton_2_clicked()
{
    ui->pushButton_2->setEnabled(false);
    emit game_continue();
    this->hide();
    ui->pushButton_2->setEnabled(true);
}

void Edit::on_pushButton_3_clicked()
{
    ui->pushButton_3->setEnabled(false);
    emit save_game_signal();
    this->hide();
    ui->pushButton_3->setEnabled(true);
}


void Edit::on_pushButton_4_clicked()
{
    ui->pushButton_4->setEnabled(false);
    emit return_dialog();
    this->hide();
    ui->pushButton_4->setEnabled(true);
}


void Edit::on_pushButton_5_clicked()
{
    ui->pushButton_5->setEnabled(false);
    ai_hint=true;
    emit ai_mode();

    //wait for 500ms
    QElapsedTimer t;
    t.start();
    while(t.elapsed()<500)
        QCoreApplication::processEvents();

    emit game_continue();
    this->hide();
    ui->pushButton_5->setEnabled(true);
}

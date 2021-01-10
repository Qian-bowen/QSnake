#ifndef EDIT_H
#define EDIT_H
#include <QDialog>
#include <QElapsedTimer>
#include <cstdlib>
#include <string>
#include <fstream>
#include "mainwindow.h"


namespace Ui{
class Edit;
}

class Edit:public QDialog
{
    Q_OBJECT
public:
    explicit Edit(QWidget *parent=nullptr);
    ~Edit();

private slots:
    void on_pushButton_clicked();//edit atlas
    void on_pushButton_2_clicked();//continue
    void on_pushButton_3_clicked();//save
    void on_pushButton_4_clicked();//return
    void on_pushButton_5_clicked();//ai mode begin

signals:
    void edit_atlas();
    void game_continue();
    void save_game_signal();
    void return_dialog();
    void ai_mode();

public:
    Ui::Edit *ui;
    bool ai_hint;
};

#endif // EDIT_H

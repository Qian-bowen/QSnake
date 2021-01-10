#ifndef DIALOG_H
#define DIALOG_H
#include <QDialog>
#include <QLabel>
#include <QDesktopWidget>
#include <fstream>
#include "mainwindow.h"
#include "edit.h"

namespace Ui {
class Dialog;
}

class Dialog:public QDialog
{
    Q_OBJECT
public:
    explicit Dialog(QWidget *parent=nullptr);
    ~Dialog();
    void paintEvent(QPaintEvent *event);

private:
    void game_history_resume();
    void clear_atlas_array();
    void init_new_game();
    void connect_before_game();
    void disconnect_after_game();//disconnect all the signals and slots
    void waiting_time(int ms);//just waiting

private slots:
    void on_pushButton_clicked();//single mode
    void on_pushButton_2_clicked();//double mode
    void on_pushButton_4_clicked();//history
    void on_pushButton_5_clicked();//tutorial

    void edit_show();
    void after_edit_continue();
    void dialog_show();
    void save_game();
    void game_over_return();//return to dialog after game over
    void reset_mode_to_ai();//reset mode and disconnect current timer


private:
    Ui::Dialog *ui;

    Edit edit;
    bool show_tut;
    //User* user_array;
    std::vector<MainWindow*> game_vec;
    int user_num;
    int game_num;
};

#endif // DIALOG_H

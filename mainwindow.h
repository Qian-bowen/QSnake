#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>
#include <QPen>
#include <QBrush>
#include <QKeyEvent>
#include <QTimer>
#include <QImage>


#include "basic_element.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow;}
QT_END_NAMESPACE


class MainWindow : public QMainWindow
{
    Q_OBJECT
friend class Dialog;

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void paintEvent(QPaintEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void mousePressEvent(QMouseEvent *event);

    node point_mapping_window(node n);
    node window_mapping_point(node n);

    void init_user(int user_num);
    void init_timer_after_user();

    void init_mode(MODE m){mode=m;}
    MODE get_mode(){return mode;}



public slots:
    void receive_show_window();
    void edit_atlas_elem_begin();
    void edit_atlas_elem_end();
    void auto_update_player1();
    void auto_update_player2();
    void ai_update_player1();
    void ai_update_player2();



signals:
    void pause_game();
    void game_over();

private:
    void update_by_user(User& user,int user_idx);//TODO: DETAILS
    DIREC key_to_direc(QKeyEvent* event);
    bool is_snake_head(int x,int y,int& head_of_which);//whether the pos is snake head
    int speed_to_interval(int user_idx);//calculate interval based on speed
    bool isKeyValid(int cur,DIREC pre);//whether user input key is valid
    bool isDirecValid(DIREC cur,DIREC pre);
    void deconstruct_timer_after_die();

    bool can_ai_snake_goto_elem(int x,int y,DIREC direc);//strategy function:check whether ai snake can goto this position
    DIREC next_step_finding();//ai:find a path to a food




private:
    Ui::MainWindow *ui;

    //edit is valid iff it is true
    bool edit_atlas_signal;

    bool colorPicker;//is any color in colorPicker
    CANVAS_ELEM picked_color_elem;
    node picked_color_point;//the position in atlas array of the picked color
    //if you just pick the color but not draw, the element will disappear

    int enlarge_scale;
    int real_length;
    int real_width;
    int user_num;

    //std::vector<DIREC> ai_path;

    User* user_array;
    Canvas canvas;
    MODE mode;

    QTimer** snake_timer;
    int update_time;
    int* interval;

    QPixmap* head,*head2;//image of snake



};

#endif // MAINWINDOW_H

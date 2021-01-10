#include "dialog.h"
#include "ui_dialog.h"
#include "ui_edit.h"
#include <queue>

extern CANVAS_ELEM** atlas_array;

namespace Ui {
class Dialog;
}

Dialog::Dialog(QWidget* parent):
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    setWindowTitle(tr("Menu"));
    //set screen
    this->resize(800,600);
    ui->pushButton->resize(500,100);
    ui->pushButton_2->resize(500,100);
    ui->pushButton_4->resize(500,100);
    ui->pushButton_5->resize(500,100);

    game_num=0;
    show_tut=false;

}


Dialog::~Dialog()
{
    //clear all the game
    for(auto it=game_vec.begin();it!=game_vec.end();++it)
    {
        if((*it)!=NULL) delete (*it);
    }
    delete ui;
}

void Dialog::waiting_time(int ms)
{
    QElapsedTimer t;
    t.start();
    while(t.elapsed()<ms)
        QCoreApplication::processEvents();
}

void Dialog::connect_before_game()
{
    connect(game_vec[game_num],SIGNAL(pause_game()),this,SLOT(edit_show()));
    connect(&(edit),SIGNAL(game_continue()),this,SLOT(after_edit_continue()));
    connect(&(edit),SIGNAL(edit_atlas()),game_vec[game_num],SLOT(edit_atlas_elem_begin()));
    connect(&(edit),SIGNAL(game_continue()),game_vec[game_num],SLOT(edit_atlas_elem_end()));
    connect(&(edit),SIGNAL(save_game_signal()),this,SLOT(save_game()));
    connect(game_vec[game_num],SIGNAL(game_over()),this,SLOT(game_over_return()));
    connect(&(edit),SIGNAL(return_dialog()),this,SLOT(game_over_return()));
    connect(&(edit),SIGNAL(ai_mode()),this,SLOT(reset_mode_to_ai()));
}

void Dialog::init_new_game()
{
    MainWindow* game_new=new MainWindow;
    game_vec.push_back(game_new);

    connect_before_game();
}

void Dialog::disconnect_after_game()
{
    disconnect(game_vec[game_num],SIGNAL(pause_game()),this,SLOT(edit_show()));
    disconnect(&(edit),SIGNAL(game_continue()),this,SLOT(after_edit_continue()));
    disconnect(&(edit),SIGNAL(edit_atlas()),game_vec[game_num],SLOT(edit_atlas_elem_begin()));
    disconnect(&(edit),SIGNAL(game_continue()),game_vec[game_num],SLOT(edit_atlas_elem_end()));
    disconnect(&(edit),SIGNAL(save_game_signal()),this,SLOT(save_game()));
    disconnect(game_vec[game_num],SIGNAL(game_over()),this,SLOT(game_over_return()));
    disconnect(&(edit),SIGNAL(return_dialog()),this,SLOT(game_over_return()));
    disconnect(&(edit),SIGNAL(ai_mode()),this,SLOT(reset_mode_to_ai()));

    disconnect(game_vec[game_num]->snake_timer[0], SIGNAL(timeout()),game_vec[game_num],SLOT(ai_update_player1()));
}


void Dialog::edit_show()
{
    edit.show();
    //ai mode
    if(game_vec[game_num]->get_mode()==AI)
    {
        for(int i=0;i<game_vec[game_num]->user_num;++i)
        {
            if(i==0) disconnect(game_vec[game_num]->snake_timer[i], SIGNAL(timeout()),game_vec[game_num],SLOT(ai_update_player1()));
            if(i==1) disconnect(game_vec[game_num]->snake_timer[i], SIGNAL(timeout()),game_vec[game_num],SLOT(ai_update_player2()));
        }
        return;
    }
    //none ai mode
    for(int i=0;i<game_vec[game_num]->user_num;++i)
    {
        if(i==0) disconnect(game_vec[game_num]->snake_timer[i], SIGNAL(timeout()),game_vec[game_num],SLOT(auto_update_player1()));
        if(i==1) disconnect(game_vec[game_num]->snake_timer[i], SIGNAL(timeout()),game_vec[game_num],SLOT(auto_update_player2()));
    }
}

void Dialog::after_edit_continue()
{
    //ai mode
    if(game_vec[game_num]->get_mode()==AI)
    {
        for(int i=0;i<user_num;++i)
        {
            if(i==0) connect(game_vec[game_num]->snake_timer[i], SIGNAL(timeout()),game_vec[game_num],SLOT(ai_update_player1()));
            if(i==1) connect(game_vec[game_num]->snake_timer[i], SIGNAL(timeout()),game_vec[game_num],SLOT(ai_update_player2()));
            game_vec[game_num]->snake_timer[i]->start(game_vec[game_num]->update_time);
            game_vec[game_num]->snake_timer[i]->setInterval(game_vec[game_num]->interval[0]);
        }
        return;
    }
    //none ai mode
    for(int i=0;i<user_num;++i)
    {
        if(i==0) connect(game_vec[game_num]->snake_timer[i], SIGNAL(timeout()),game_vec[game_num],SLOT(auto_update_player1()));
        if(i==1) connect(game_vec[game_num]->snake_timer[i], SIGNAL(timeout()),game_vec[game_num],SLOT(auto_update_player2()));
        game_vec[game_num]->snake_timer[i]->start(game_vec[game_num]->update_time);
        game_vec[game_num]->snake_timer[i]->setInterval(game_vec[game_num]->interval[i]);
    }

}

//single player
void Dialog::on_pushButton_clicked()
{
    //set button false to avoid click repeatedly
    ui->pushButton->setEnabled(false);

    init_new_game();

    user_num=1;   
    game_vec[game_num]->init_user(user_num);
    game_vec[game_num]->init_timer_after_user();
    game_vec[game_num]->init_mode(MAN);

    waiting_time(100);
    game_vec[game_num]->show();
    edit_show();
    this->hide();

    ui->pushButton->setEnabled(true);
}

//double player
void Dialog::on_pushButton_2_clicked()
{
    ui->pushButton_2->setEnabled(false);

    init_new_game();

    user_num=2;  
    game_vec[game_num]->init_user(user_num);
    game_vec[game_num]->init_timer_after_user();
    game_vec[game_num]->init_mode(MAN);

    waiting_time(100);
    game_vec[game_num]->show();
    edit_show();
    this->hide();

    ui->pushButton_2->setEnabled(true);
}

//history resume
void Dialog::on_pushButton_4_clicked()
{
    ui->pushButton_4->setEnabled(false);

    init_new_game();
    game_history_resume();
    game_vec[game_num]->init_timer_after_user();
    game_vec[game_num]->init_mode(MAN);

    //reset interval
    for(int i=0;i<user_num;++i)
    {
        game_vec[game_num]->interval[i]=game_vec[game_num]->speed_to_interval(i);
    }

    waiting_time(100);
    game_vec[game_num]->show();
    edit_show();
    this->hide();

    ui->pushButton_4->setEnabled(true);
}

void Dialog::on_pushButton_5_clicked()
{
    ui->pushButton_4->setEnabled(false);
    show_tut=true;

    this->resize(900,1700);

    ui->pushButton_4->setEnabled(true);
}

void Dialog::paintEvent(QPaintEvent *event)
{
    if(!show_tut) return;
    QPainter painter(this);
    QPixmap pixmap2(":/elem_intro.png");
    QPixmap pixmap1(":/detail.PNG");
    painter.drawPixmap(50,500,800,600, pixmap2);
    painter.drawPixmap(50,1100,800,600, pixmap1);
}


void Dialog::dialog_show()
{
    this->show();
    game_vec[game_num]->hide();
}

void Dialog::save_game()
{
    std::ofstream out;
    out.open("game.txt",std::ios::trunc);

    //save user number
    out<<user_num<<'\n';

    //save atlas element
    for(int i=0;i<ATLAS_LINE_NUM;++i)
    {
        for(int j=0;j<ATLAS_COL_NUM;++j)
        {
            out<<atlas_array[i][j]<<'\t';
        }
        out<<'\n';
    }

    //save user info: direc & snake node
    for(int i=0;i<user_num;++i)
    {
        std::queue<node> snake_body=game_vec[game_num]->user_array[i].snake->get_snake_body();
        out<<snake_body.size()<<'\n';
        out<<game_vec[game_num]->user_array[i].direc<<'\n';

        while(!snake_body.empty())
        {
            node front_node=snake_body.front();
            snake_body.pop();
            out<<front_node.x<<' '<<front_node.y<<'\n';
        }
        out<<'\n';
        //save life_num
        out<<game_vec[game_num]->user_array[i].snake->life_get()<<'\n';
        //save length
        out<<game_vec[game_num]->user_array[i].snake->length_get()<<'\n';
        //save speed
        out<<game_vec[game_num]->user_array[i].snake->speed_get()<<'\n';
    }

    //save canvas element
    out<<game_vec[game_num]->canvas.item_number(BRICK)<<'\n';
    out<<game_vec[game_num]->canvas.item_number(ORDIN_FOOD)<<'\n';
    out<<game_vec[game_num]->canvas.item_number(LIFE_FOOD)<<'\n';
    out<<game_vec[game_num]->canvas.item_number(SUP_FOOD)<<'\n';
    out<<game_vec[game_num]->canvas.item_number(SDOWN_FOOD)<<'\n';
    out<<game_vec[game_num]->canvas.item_number(STRENGTH_FOOD)<<'\n';


    out.close();
    this->show();
    game_vec[game_num]->hide();

    disconnect_after_game();
    clear_atlas_array();

    ++game_num;
    std::cout<<"game num:"<<game_num<<std::endl;
}



void Dialog::game_history_resume()
{
    std::cout<<"game history resume"<<std::endl;

    std::ifstream in;
    in.open("game.txt");

    //user number
    int user_n;
    in>>user_n;

    //initialize user num in dialog and game
    game_vec[game_num]->user_num=user_n;
    user_num=user_n;

    //resume Atlas_array
    int array_elem;
    for(int i=0;i<ATLAS_LINE_NUM;++i)
    {
        for(int j=0;j<ATLAS_COL_NUM;++j)
        {
            in>>array_elem;
            atlas_array[i][j]=CANVAS_ELEM(array_elem);
        }
    }

    int direc[2],life[2],len[2],sp[2];
    std::queue<node> snake_body_array[2];
    for(int i=0;i<user_n;++i)
    {
        int node_num,life_cur,len_cur,sp_cur;
        in>>node_num;
        in>>direc[i];
        for(int j=0;j<node_num;++j)
        {
            int x,y;
            in>>x>>y;
            node n(x,y);
            snake_body_array[i].push(n);
        }
        in>>life_cur;
        in>>len_cur;
        in>>sp_cur;
        life[i]=life_cur;
        len[i]=len_cur;
        sp[i]=sp_cur;
    }

    //read canvas elem num
    int elem_num[6];
    for(int i=0;i<6;++i)
    {
        int cur;
        in>>cur;
        elem_num[i]=cur;
    }

    Canvas can(elem_num[0],elem_num[1],elem_num[2],elem_num[3],elem_num[4],elem_num[5]);
    game_vec[game_num]->canvas=can;


    if(user_n==1)
    {
        Snake* s=new Snake(snake_body_array[0],life[0],len[0],sp[0]);
        User u(s,(DIREC)direc[0]);
        game_vec[game_num]->user_array=new User[user_n]{u};
    }
    else if(user_n==2)
    {
        Snake* s1=new Snake(snake_body_array[0],life[0],len[0],sp[0]);
        Snake* s2=new Snake(snake_body_array[1],life[1],len[1],sp[1]);
        User u1(s1,(DIREC)direc[0]),u2(s2,(DIREC)direc[1]);
        game_vec[game_num]->user_array=new User[user_n]{u1,u2};
    }

}



void Dialog::clear_atlas_array()
{
    for(int i=0;i<ATLAS_LINE_NUM;++i)
    {
        for(int j=0;j<ATLAS_COL_NUM;++j)
        {
            atlas_array[j][i]=NONE;
        }
    }
}

void Dialog::game_over_return()
{
    std::cout<<"game over return"<<std::endl;
    game_vec[game_num]->hide();

    clear_atlas_array();

    disconnect_after_game();

    ++game_num;
    std::cout<<"game num:"<<game_num<<std::endl;

    this->show();
}

void Dialog::reset_mode_to_ai()
{
    game_vec[game_num]->init_mode(AI);
    disconnect(game_vec[game_num]->snake_timer[0], SIGNAL(timeout()),game_vec[game_num],SLOT(auto_update_player1()));
}




#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "basic_element.h"

extern CANVAS_ELEM** atlas_array;


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Qsnake");
    update_time=300;

    enlarge_scale=50;
    real_length=enlarge_scale*(ATLAS_COL_NUM+1);
    real_width=enlarge_scale*(ATLAS_LINE_NUM+1);

    user_array=NULL;
    user_num=0;
    colorPicker=false;
    edit_atlas_signal=false;

//    head=new QPixmap("C:/qt-homework/qsnake_ui/head.jpg");
//    head2=new QPixmap("C:/qt-homework/qsnake_ui/head_2.jpg");
    head=new QPixmap(":/head.jpg");
    head2=new QPixmap(":/head_2.jpg");
}

MainWindow::~MainWindow()
{
    delete[] user_array;
    delete[] interval;
    for(int i=0;i<user_num;++i)
    {
        delete snake_timer[i];
    }
    delete[] snake_timer;
    delete head;
    delete head2;
    delete ui;
}

/*
 * map a point from atlas to window
*/
node MainWindow::point_mapping_window(node n)
{
    int win_x=n.x*enlarge_scale,win_y=(ATLAS_LINE_NUM-n.y)*enlarge_scale;
    node ret(win_x,win_y);
    return ret;
}

node MainWindow::window_mapping_point(node n)
{
    int point_x=n.x/enlarge_scale;
    int point_y=ATLAS_LINE_NUM-n.y/enlarge_scale;
    node ret(point_x,point_y);
    return ret;
}

bool MainWindow::is_snake_head(int x,int y,int& head_of_which)
{
    if(user_num==1)
    {
        node h=user_array[0].snake->get_snake_body().back();
        if(x==h.x&&y==h.y)
        {
            head_of_which=1;
            return true;
        }
    }

    if(user_num==2)
    {
        node h1=user_array[0].snake->get_snake_body().back();
        node h2=user_array[1].snake->get_snake_body().back();
        if(x==h1.x&&y==h1.y)
        {
            head_of_which=1;
            return true;
        }
        if(x==h2.x&&y==h2.y)
        {
            head_of_which=2;
            return true;
        }
    }

    return false;
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    setFixedSize(real_length,real_width);

    for(int i=0;i<ATLAS_LINE_NUM;++i)
    {
        for(int j=0;j<ATLAS_COL_NUM;++j)
        {
            if(atlas_array[i][j]!=NONE)
            {
                switch(atlas_array[i][j])
                {
                case SNAKE:
                    painter.setBrush(Qt::green);
                    break;
                case BRICK:
                    painter.setBrush(Qt::darkGray);
                    break;
                case ORDIN_FOOD:
                    painter.setBrush(Qt::darkGreen);
                    break;
                case LIFE_FOOD:
                    painter.setBrush(Qt::red);
                    break;
                case SUP_FOOD:
                    painter.setBrush(Qt::yellow);
                    break;
                case SDOWN_FOOD:
                    painter.setBrush(Qt::blue);
                    break;
                case STRENGTH_FOOD:
                    painter.setBrush(Qt::darkRed);
                    break;
                }

                node n1(j,i);
                node n=point_mapping_window(n1);
                QRectF rectangle(n.x,n.y,enlarge_scale,enlarge_scale);
                painter.drawRect(rectangle);

                //paint head picture
                int head_of_which;
                if(is_snake_head(j,i,head_of_which))
                {
                    if(head_of_which==1) painter.drawPixmap(n.x,n.y,enlarge_scale,enlarge_scale,*head);
                    if(head_of_which==2) painter.drawPixmap(n.x,n.y,enlarge_scale,enlarge_scale,*head2);

                }

            }
        }
    }

    //paint life num

    QFont font("Arial",QFont::Bold,true);
    font.setPointSize(20);
    painter.setPen(QColor(0, 160, 230));

    if(user_num==1)
    {
        std::string all_info="";

        std::string life_info="life: ";
        int life_number=user_array[0].snake->life_get();
        std::string life_num_str=std::to_string(life_number);

        std::string speed_info=" speed: ";
        int speed_num=user_array[0].snake->speed_get();
        std::string speed_num_str=std::to_string(speed_num);

        all_info+=life_info+life_num_str+speed_info+speed_num_str;

        painter.drawPixmap(0,0,80,80,*head);
        QRectF rect(100,0,1000,400);
        painter.setFont(font);
        painter.drawText(rect,tr(all_info.c_str()));

    }
    else if(user_num==2)
    {
        int life_number1=user_array[0].snake->life_get();
        std::string life_num_str1=std::to_string(life_number1);
        int life_number2=user_array[1].snake->life_get();
        std::string life_num_str2=std::to_string(life_number2);

        int speed_num1=user_array[0].snake->speed_get();
        std::string speed_num_str1=std::to_string(speed_num1);

        int speed_num2=user_array[1].snake->speed_get();
        std::string speed_num_str2=std::to_string(speed_num2);


        std::string player1_info="Player 1 Life: "+life_num_str1+" Speed:"+speed_num_str1;
        std::string player2_info="Player 2 Life: "+life_num_str2+" Speed:"+speed_num_str2;


        painter.drawPixmap(0,0,80,80,*head);
        painter.drawPixmap(0,100,80,80,*head2);

        painter.setFont(font);
        QRectF rect1(100,0,1500,400);
        QRectF rect2(100,100,1500,400);
        painter.drawText(rect1,tr(player1_info.c_str()));
        painter.drawText(rect2,tr(player2_info.c_str()));
    }

    //update the canvas
    //it's position CANNOT be modified !!!
    //canvas.canvas_update();

}

DIREC MainWindow::key_to_direc(QKeyEvent* event)
{
    switch(event->key())
    {
        case Qt::Key_Up:case Qt::Key_W:
            return UP;
            break;
        case Qt::Key_Down: case Qt::Key_S:
            return DOWN;
            break;
        case Qt::Key_Left:case Qt::Key_A:
            return LEFT;
            break;
        case Qt::Key_Right:case Qt::Key_D:
            return RIGHT;
            break;
     }
}

bool MainWindow::isKeyValid(int cur,DIREC pre)
{
    if((cur==Qt::Key_Up&&pre==DOWN)||(cur==Qt::Key_Left&&pre==RIGHT)||
        (cur==Qt::Key_Down&&pre==UP)||(cur==Qt::Key_Right&&pre==LEFT)||
        (cur==Qt::Key_D&&pre==LEFT)||(cur==Qt::Key_S&&pre==UP)||
        (cur==Qt::Key_A&&pre==RIGHT)||(cur==Qt::Key_W&&pre==DOWN))
        return false;
    return true;
}

bool MainWindow::isDirecValid(DIREC cur,DIREC pre)
{
    if((cur==UP&&pre==DOWN)||(cur==LEFT&&pre==RIGHT)||
        (cur==DOWN&&pre==UP)||(cur==RIGHT&&pre==LEFT))
        return false;
    return true;
}


void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if(event->key()==Qt::Key_Space)
    {
        emit pause_game();
    }
    bool player1=true,player2=true;
    //press key frequently to speed up is not allowed
    if(user_num==1&&(user_array[0].direc==key_to_direc(event)))
    {
        return;
    }
    //player1 is invalid
    if(user_num==2&&(user_array[0].direc==key_to_direc(event)))
    {
        player1=false;
    }
    //player2 is invalid
    if(user_num==2&&(user_array[1].direc==key_to_direc(event)))
    {
        player2=false;
    }
    //go back is not valid
    int cur=event->key();

    if(player1&&isKeyValid(cur,user_array[0].direc))
    {
        switch(event->key())
        {
            case Qt::Key_Up:
                user_array[0].direc=UP;
                user_array[0].snake->update_snake(UP);
                user_array[0].snake->move_snack(UP);
                update();
                break;
            case Qt::Key_Down:
                user_array[0].direc=DOWN;
                user_array[0].snake->update_snake(DOWN);
                user_array[0].snake->move_snack(DOWN);
                update();
                break;
            case Qt::Key_Left:
                user_array[0].direc=LEFT;
                user_array[0].snake->update_snake(LEFT);
                user_array[0].snake->move_snack(LEFT);
                update();
                break;
            case Qt::Key_Right:
                user_array[0].direc=RIGHT;
                user_array[0].snake->update_snake(RIGHT);
                user_array[0].snake->move_snack(RIGHT);
                update();
                break;
            default:
                break;
        }
    }

    if(player2&&isKeyValid(cur,user_array[1].direc))
    {
        switch(event->key())
        {
            case Qt::Key_W:
                user_array[1].direc=UP;
                user_array[1].snake->update_snake(UP);
                user_array[1].snake->move_snack(UP);
                update();
                break;
            case Qt::Key_S:
                user_array[1].direc=DOWN;
                user_array[1].snake->update_snake(DOWN);
                user_array[1].snake->move_snack(DOWN);
                update();
                break;
            case Qt::Key_A:
                user_array[1].direc=LEFT;
                user_array[1].snake->update_snake(LEFT);
                user_array[1].snake->move_snack(LEFT);
                update();
                break;
            case Qt::Key_D:
                user_array[1].direc=RIGHT;
                user_array[1].snake->update_snake(RIGHT);
                user_array[1].snake->move_snack(RIGHT);
                update();
                break;
            default:
                break;
        }
    }

}

//if two user are playing, interval are their average value
//range of interval is 150-450
int MainWindow::speed_to_interval(int user_idx)
{
    int val;
    val=300-user_array[user_idx].snake->speed_get()*50;
    if(val<50) val=50;
    if(val>550) val=550;
    //update interval array
    interval[user_idx]=val;
    return val;

}

void MainWindow::update_by_user(User& user,int user_idx)
{
    CANVAS_ELEM next=user.user_update_snake();
    if(next!=SNAKE&&next!=NONE)
    {
        canvas.remove_canvas_elem(next);
    }
    //handle speed case
    switch(next)
    {
    case SUP_FOOD:
        snake_timer[user_idx]->setInterval(speed_to_interval(user_idx));
        canvas.elem_decrease(next);
        break;
    case SDOWN_FOOD:
        snake_timer[user_idx]->setInterval(speed_to_interval(user_idx));
        canvas.elem_decrease(next);
        break;
    case STRENGTH_FOOD:
        snake_timer[user_idx]->setInterval(speed_to_interval(user_idx));
        canvas.elem_decrease(next);
        break;
    case ORDIN_FOOD:case LIFE_FOOD:case BRICK:
        canvas.elem_decrease(next);
    default:
        break;
    }

    if(user.snake->life_get()<=0)
    {
        for(int i=0;i<user_num;++i)
        {
            if(i==0) disconnect(snake_timer[i], SIGNAL(timeout()),this,SLOT(auto_update_player1()));
            if(i==1) disconnect(snake_timer[i], SIGNAL(timeout()),this,SLOT(auto_update_player2()));
        }
        emit game_over();
        return;
    }
    user.snake_move_cur_direc();

    canvas.canvas_update();//try
}

void MainWindow::auto_update_player1()
{
   if(user_array==NULL) return;
    snake_timer[0]->stop();//stop
    update_by_user(user_array[0],0);//update
    snake_timer[0]->start(interval[0]);//continue
    repaint();
}

void MainWindow::auto_update_player2()
{
    if(user_array==NULL) return;
    snake_timer[1]->stop();//stop
    update_by_user(user_array[1],1);//update
    snake_timer[1]->start(interval[1]);//continue
    repaint();
}

void MainWindow::ai_update_player1()
{
    if(user_array==NULL) return;

    snake_timer[0]->stop();//stop
    DIREC cur_direc=next_step_finding();
    user_array[0].ai_set_direc((DIREC)cur_direc);//find
    update_by_user(user_array[0],0);//update
    snake_timer[0]->start(interval[0]);//continue
    repaint();
}

void MainWindow::ai_update_player2()
{
    if(user_array==NULL) return;

    snake_timer[1]->stop();//stop
    DIREC cur_direc=next_step_finding();
    user_array[1].ai_set_direc((DIREC)cur_direc);//find
    update_by_user(user_array[1],1);//update
    snake_timer[1]->start(interval[1]);//continue
    repaint();
}



//move snake element and none element invalid
//these operations will be ignored
void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if(edit_atlas_signal==false) return;
    if(colorPicker==false)//get the color
    {
        QPointF relative_point=event->localPos();
        int x=relative_point.rx();
        int y=relative_point.ry();

        picked_color_point=window_mapping_point(node(x,y));

        picked_color_elem=atlas_array[picked_color_point.y][picked_color_point.x];

        //if pick nothing just return
        //if user pick snake element just return
        if(atlas_array[picked_color_point.y][picked_color_point.x]==NONE||
                atlas_array[picked_color_point.y][picked_color_point.x]==SNAKE)
            return;
        atlas_array[picked_color_point.y][picked_color_point.x]=NONE;
        //update the UI after edit the atlas
        update();
        colorPicker=true;
    }
    else//draw the color
    {
        //just test code
        QPointF relative_point=event->localPos();
        int x=relative_point.rx();
        int y=relative_point.ry();
        picked_color_point=window_mapping_point(node(x,y));
        //refresh the atlas

        atlas_array[picked_color_point.y][picked_color_point.x]=picked_color_elem;
        update();
        colorPicker=false;
    }
}

void MainWindow::edit_atlas_elem_begin()
{
    edit_atlas_signal=true;
}

void MainWindow::edit_atlas_elem_end()
{
    edit_atlas_signal=false;
}


void MainWindow::receive_show_window()
{
    this->show();
}

void MainWindow::init_user(int num)
{
    user_num=num;
    user_array=new User[user_num];
}

void MainWindow::init_timer_after_user()
{
    interval=new int[user_num];
    for(int i=0;i<user_num;++i)
    {
        interval[i]=update_time;
        std::cout<<"interval:"<<interval[i]<<std::endl;//
    }

    snake_timer=new QTimer*[user_num];

    for(int i=0;i<user_num;++i)
    {
        snake_timer[i] = new QTimer(this);
        if(i==0) connect(snake_timer[i], SIGNAL(timeout()),this,SLOT(auto_update_player1()));
        if(i==1) connect(snake_timer[i], SIGNAL(timeout()),this,SLOT(auto_update_player2()));
        snake_timer[i]->start(update_time);
    }
}



bool MainWindow::can_ai_snake_goto_elem(int x,int y,DIREC direc)
{
    //death elem
    if(atlas_array[y][x]==BRICK)
    {
        std::cout<<"brick"<<std::endl;
        return false;
    }
    //in case of meet it own body
    //avoid the snake become a circle
    switch(direc)
    {
    case UP:
        for(int i=y;i<ATLAS_LINE_NUM;++i)
        {
            if(atlas_array[i][x]==SNAKE)
                return false;
        }
        break;
    case DOWN:
        for(int i=y;i>=0;--i)
        {
            if(atlas_array[i][x]==SNAKE)
                return false;
        }
        break;
    case LEFT:
        for(int i=x;i>=0;--i)
        {
            if(atlas_array[y][i]==SNAKE)
                return false;
        }
        break;
    case RIGHT:
        for(int i=x;i<ATLAS_COL_NUM;++i)
        {
            if(atlas_array[y][i]==SNAKE)
                return false;
        }
        break;
    default:
        break;
    }

    return true;
}

DIREC MainWindow::next_step_finding()
{
    //get src namely head of snake
    node head=user_array[0].snake->get_snake_body().back();
    node dist;
    int max_dist=INT_MAX;
    //get dist namely nearest food
    for(int i=0;i<ATLAS_LINE_NUM;++i)
    {
        for(int j=0;j<ATLAS_COL_NUM;++j)
        {
            if(atlas_array[i][j]==ORDIN_FOOD)
            {
                //pay attention to order of i j
                int distPower2=pow((head.x-j),2)+pow((head.y-i),2);
                if(distPower2<max_dist)
                {
                    max_dist=distPower2;
                    dist.x=j;
                    dist.y=i;
                }
            }
        }
    }


    int x_left=(head.x-1+ATLAS_COL_NUM)%ATLAS_COL_NUM;
    int x_right=(head.x+1+ATLAS_COL_NUM)%ATLAS_COL_NUM;
    int y_down=(head.y-1+ATLAS_LINE_NUM)%ATLAS_LINE_NUM;
    int y_up=(head.y+1+ATLAS_LINE_NUM)%ATLAS_LINE_NUM;

    //the second quadrant
    DIREC pre=user_array[0].direc;
    if(dist.x<head.x&&dist.y>head.y)
    {
        if(can_ai_snake_goto_elem(x_left,head.y,LEFT)&&isDirecValid(LEFT,pre))
            return LEFT;
        if(can_ai_snake_goto_elem(head.x,y_up,UP)&&isDirecValid(UP,pre))
            return UP;
        if(can_ai_snake_goto_elem(x_right,head.y,RIGHT)&&isDirecValid(RIGHT,pre))
            return RIGHT;
        if(can_ai_snake_goto_elem(head.x,y_down,DOWN)&&isDirecValid(DOWN,pre))
            return DOWN;
    }

    //the first quadrant
    if(dist.x>head.x&&dist.y>head.y)
    {
        if(can_ai_snake_goto_elem(x_right,head.y,RIGHT)&&isDirecValid(RIGHT,pre))
            return RIGHT;
        if(can_ai_snake_goto_elem(head.x,y_up,UP)&&isDirecValid(UP,pre))
            return UP;
        if(can_ai_snake_goto_elem(x_left,head.y,LEFT)&&isDirecValid(LEFT,pre))
            return LEFT;
        if(can_ai_snake_goto_elem(head.x,y_down,DOWN)&&isDirecValid(DOWN,pre))
            return DOWN;
    }
    //the third quadrant
    if(dist.x<head.x&&dist.y<head.y)
    {
        if(can_ai_snake_goto_elem(x_left,head.y,LEFT)&&isDirecValid(LEFT,pre))
            return LEFT;
        if(can_ai_snake_goto_elem(head.x,y_down,DOWN)&&isDirecValid(DOWN,pre))
            return DOWN;
        if(can_ai_snake_goto_elem(x_right,head.y,RIGHT)&&isDirecValid(RIGHT,pre))
            return RIGHT;
        if(can_ai_snake_goto_elem(head.x,y_up,UP)&&isDirecValid(UP,pre))
            return UP;
    }
    //the fourth quadrant
    if(dist.x>head.x&&dist.y<head.y)
    {
        if(can_ai_snake_goto_elem(x_right,head.y,RIGHT)&&isDirecValid(RIGHT,pre))
            return RIGHT;
        if(can_ai_snake_goto_elem(head.x,y_down,DOWN)&&isDirecValid(DOWN,pre))
            return DOWN;
        if(can_ai_snake_goto_elem(x_left,head.y,LEFT)&&isDirecValid(LEFT,pre))
            return LEFT;
        if(can_ai_snake_goto_elem(head.x,y_up,UP)&&isDirecValid(UP,pre))
            return UP;
    }
    //x array equal situation,first go up or down
    if(dist.x==head.x&&dist.y<head.y)
    {
        if(can_ai_snake_goto_elem(head.x,y_down,DOWN)&&isDirecValid(DOWN,pre))
            return DOWN;
        if(can_ai_snake_goto_elem(x_right,head.y,RIGHT)&&isDirecValid(RIGHT,pre))
            return RIGHT;
        if(can_ai_snake_goto_elem(x_left,head.y,LEFT)&&isDirecValid(LEFT,pre))
            return LEFT;
        if(can_ai_snake_goto_elem(head.x,y_up,UP)&&isDirecValid(UP,pre))
            return UP;
    }
    if(dist.x==head.x&&dist.y>head.y)
    {
        if(can_ai_snake_goto_elem(head.x,y_up,UP)&&isDirecValid(UP,pre))
            return UP;
        if(can_ai_snake_goto_elem(x_right,head.y,RIGHT)&&isDirecValid(RIGHT,pre))
            return RIGHT;
        if(can_ai_snake_goto_elem(x_left,head.y,LEFT)&&isDirecValid(LEFT,pre))
            return LEFT;
        if(can_ai_snake_goto_elem(head.x,y_down,DOWN)&&isDirecValid(DOWN,pre))
            return DOWN;
    }
    //y array equal situation,first go left or right
    if(dist.x<head.x&&dist.y==head.y)
    {
        if(can_ai_snake_goto_elem(x_left,head.y,LEFT)&&isDirecValid(LEFT,pre))
            return LEFT;
        if(can_ai_snake_goto_elem(head.x,y_up,UP)&&isDirecValid(UP,pre))
            return UP;
        if(can_ai_snake_goto_elem(head.x,y_down,DOWN)&&isDirecValid(DOWN,pre))
            return DOWN;
        if(can_ai_snake_goto_elem(x_right,head.y,RIGHT)&&isDirecValid(RIGHT,pre))
            return RIGHT;
    }
    if(dist.x>head.x&&dist.y==head.y)
    {
        if(can_ai_snake_goto_elem(x_right,head.y,RIGHT)&&isDirecValid(RIGHT,pre))
            return RIGHT;
        if(can_ai_snake_goto_elem(head.x,y_up,UP)&&isDirecValid(UP,pre))
            return UP;
        if(can_ai_snake_goto_elem(head.x,y_down,DOWN)&&isDirecValid(DOWN,pre))
            return DOWN;
        if(can_ai_snake_goto_elem(x_left,head.y,LEFT)&&isDirecValid(LEFT,pre))
            return LEFT;
    }


    //error occur
    //阮籍猖狂，岂效穷途之哭！
    std::cout<<"no path"<<std::endl;
    return NO_DIREC;
}





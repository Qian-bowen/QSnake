#include "basic_element.h"

CANVAS_ELEM** atlas_array;

//define atlas_array


Atlas_init::Atlas_init()
{
    atlas_array=new CANVAS_ELEM*[ATLAS_LINE_NUM];
    for(int j=0;j<ATLAS_LINE_NUM;++j)
        atlas_array[j]=new CANVAS_ELEM[ATLAS_COL_NUM];

    for(int i=0;i<ATLAS_LINE_NUM;++i)
    {
        for(int j=0;j<ATLAS_COL_NUM;++j)
        {
            atlas_array[i][j]=NONE;
        }
    }
}

Atlas_init::~Atlas_init()
{
    for(int i=0;i<ATLAS_LINE_NUM;++i)
    {
        delete[] atlas_array[i];
    }
    delete[] atlas_array;
}





/*
show atlas
it is a test function
*/

void test_show_atlas()
{
    for(int i=0;i<ATLAS_LINE_NUM;++i)
    {
        for(int j=0;j<ATLAS_COL_NUM;++j)
        {
            std::cout<<atlas_array[i][j]<<' ';
        }
        std::cout<<std::endl;
    }
    std::cout<<std::endl;
    std::cout<<std::endl;
}

/*
功能：在固定矩形区域内生成一个随机点，包含边界
参数：两个对角点，必须为整数，他们不水平或者垂直
*/
node random_node(node a,node b)
{
    int lower_x=(a.x<b.x)?a.x:b.x;
    int upper_x=(a.x>b.x)?a.x:b.x;
    int lower_y=(a.y<b.y)?a.y:b.y;
    int upper_y=(a.y>b.y)?a.y:b.y;
    int x=lower_x+rand()%(upper_x-lower_x+1);
    int y=lower_y+rand()%(upper_y-lower_y+1);
    node n(x,y);
    return n;
}

/*
功能：返回任意一个方向
*/

DIREC random_direction()
{
    int index=rand()%4;
    DIREC direc;
    switch(index)
    {
        case 0:
            direc=UP;
            break;
        case 1:
            direc=DOWN;
            break;
        case 2:
            direc=LEFT;
            break;
        case 3:
            direc=RIGHT;
            break;
    }
    return direc;
}

/*
功能：擦除atlas_array中的某一个点
注意：xy顺序和数组的意义恰好相反
*/
void erase_atlas_array_point(node n)
{
    atlas_array[n.y][n.x]=NONE;
}

/*
功能：设置atlas_array中的某一个点
*/
void set_atlas_array_point(node n,CANVAS_ELEM canvas_elem)
{
    atlas_array[n.y][n.x]=canvas_elem;
}

/*
功能：返回atlas_array中的某一个点
*/
CANVAS_ELEM get_atlas_array_point(node n)
{
    return atlas_array[n.y][n.x];
}

/*
功能：根据当前的点和方向获取下一个点的坐标
*/
node get_point_by_direc(DIREC direc,node cur)
{
    int x=cur.x,y=cur.y;

    switch(direc)
    {
        case UP:
        y+=1;break;
        case  DOWN:
        y-=1;break;
        case LEFT:
        x-=1;break;
        case RIGHT:
        x+=1;break;
    }

    //当添加的节点超出边界，加到另一侧；否则按原规则添加
    x=(x+ATLAS_COL_NUM)%ATLAS_COL_NUM;
    y=(y+ATLAS_LINE_NUM)%ATLAS_LINE_NUM;
    node n(x,y);
    return n;
}



std::queue<node> Snake::get_snake_body()
{
    return body;
}

/*
功能：向某一个方向构造蛇,新方块在蛇的头部
返回值：构造成功返回true，否则返回false
注意：构造时不考虑障碍物
*/
void Snake::add_node_snake(DIREC direc)
{
    node head=body.back();
    node n=get_point_by_direc(direc,head);
    body.push(n);
    //++length;
    set_atlas_array_point(n,SNAKE);  
}

/*
功能：将蛇的尾部出队
*/
void Snake::pop_tail_snake()
{
    node tail=body.front();
    erase_atlas_array_point(tail);
    body.pop();
}


/*
生成初始蛇，蛇为一条直线，随机生成在canvas中央20*20的区域内。
*/
Snake::Snake()
{
    life_num=1;
    length=5;
    speed=0;
    next_elem=NONE;

    //create the snacke at the center of the canvas
    //the snake has 5 nodes
    DIREC direc=random_direction();
    init_direc=direc;

    int n1_x=ATLAS_COL_NUM/4;
    int n2_x=(ATLAS_COL_NUM/4)*3;
    int n1_y=ATLAS_LINE_NUM/4;
    int n2_y=(ATLAS_LINE_NUM/4)*3;
    node n1(n1_x,n1_y),n2(n2_x,n2_y);
    node ran_node=random_node(n1,n2);

    body.push(ran_node);
    for(int i=0;i<4;++i)
    {
        add_node_snake(direc);
    }

}

Snake::Snake(std::queue<node> snake_body,int life,int len,int sp)
{
    body=snake_body;
    life_num=life;
    length=len;
    speed=sp;
    next_elem=NONE;
}

Snake::Snake(const Snake& s)
{
    body=s.body;
    life_num=s.life_num;
    length=s.length;
    speed=s.speed;
    next_elem=NONE;
}

void Snake::set_snake_to_atlas()
{
    std::queue<node> body_cpy=body;
    while(!body_cpy.empty())
    {
        node cur=body_cpy.front();
        body_cpy.pop();
        set_atlas_array_point(cur,SNAKE);
    }
}


/*
功能：根据玩家操作，变化蛇的位置
*/
void Snake::move_snack(DIREC direc)
{
    switch(direc)
    {
        case UP:
            add_node_snake(UP);break;
        case DOWN:
            add_node_snake(DOWN);break;
        case LEFT:
            add_node_snake(LEFT);break;
        case RIGHT:
            add_node_snake(RIGHT);break;
    }
    pop_tail_snake();
}

/*
功能：执行下一个指令后更新蛇的状态
*/
CANVAS_ELEM Snake::update_snake(DIREC direc)
{

    node head=body.back();
    node next_step=get_point_by_direc(direc,head);
    CANVAS_ELEM next_elem=get_atlas_array_point(next_step);
    switch(next_elem)
    {
        case NONE:
            break;
        case SNAKE:
            --life_num;break;
        case BRICK:
            --life_num;break;
        case ORDIN_FOOD:
            add_node_snake(direc);
            ++length;
            break;
        case LIFE_FOOD:
            life_num++;
            break;
        case SUP_FOOD:
            speed++;
            break;
        case SDOWN_FOOD:
            speed--;
            break;
        case STRENGTH_FOOD:
            speed--;
            life_num++;
            break;
    }
    return next_elem;
}


/*
功能：通过输入的枚举类型返回需要操作的canvas元素
*/
int& Canvas::canvas_elem_switch(CANVAS_ELEM canvas_elem)
{
    switch(canvas_elem)
    {
        case BRICK:
        return brick;break;
        case ORDIN_FOOD:
        return ordin_food;break;
        case LIFE_FOOD:
        return life_food;break;
        case SUP_FOOD:
        return sup_food;break;
        case SDOWN_FOOD:
        return sdown_food;break;
        case STRENGTH_FOOD:
        return strength_food;break;
    }
}

/*
功能：确定新添加的元素位置是否合适
规则：一个元素的上下左右不可放置其他元素(包括蛇),最边界一圈不可以放元素
*/
bool Canvas::check_elem_pos_suit(node n,CANVAS_ELEM canvas_elem)
{
    //handle the boundary problem first
    if(n.x==0||n.x+1==ATLAS_COL_NUM||n.y==0||n.y+1==ATLAS_LINE_NUM)
        return false;
    //if this pos is not NONE
    if(atlas_array[n.y][n.x]!=NONE)
        return false;
    //up down left right should be empty
    if(atlas_array[n.y-1][n.x]!=NONE||atlas_array[n.y+1][n.x]!=NONE||
    atlas_array[n.y][n.x-1]!=NONE||atlas_array[n.y][n.x+1]!=NONE)
        {

        return false;
        }
    
    return true;
}

/*
功能：在canvas上添加某一类型的元素
*/
void Canvas::random_create_elem(int num,CANVAS_ELEM canvas_elem)
{
    int& elem_name=canvas_elem_switch(canvas_elem);
    node n1(1,1),n2(ATLAS_COL_NUM-1,ATLAS_LINE_NUM-1),n;
    for(int i=0;i<num;++i)
    {
        bool flag=false;
        while(!flag)
        {
            n=random_node(n1,n2);
            flag=check_elem_pos_suit(n,canvas_elem);
        }
        elem_name++;
        //update the atlas_array
        set_atlas_array_point(n,canvas_elem);
    } 
}




/*
 * rule:brick max number is 4/generate every 5 turn
 * ordin_food_num max 6
 * life_food generate every 4 update turn if it does not exist
 * other thing total max num is 2 /generate every 3 update turn
*/

void Canvas::canvas_update()
{
    if(brick<4)
    {
        random_create_elem(4-brick,BRICK);
        ++update_turn;
    }
    if(update_turn%5==0)
    {
        random_create_elem(1,BRICK);
        ++update_turn;
    }
    if(ordin_food<6)
    {
        random_create_elem(6-ordin_food,ORDIN_FOOD);
        ++update_turn;
    }
    if(life_food==0&&update_turn%4==0)
    {
        random_create_elem(1,LIFE_FOOD);
        ++update_turn;
    }
    if(sup_food+sdown_food+strength_food<2&&update_turn%3==0)
    {
        int idx=rand()%3;
        switch(idx)
        {
        case 0:
            random_create_elem(1,SUP_FOOD);
        case 1:
            random_create_elem(1,SDOWN_FOOD);
        case 2:
            random_create_elem(1,STRENGTH_FOOD);
        }
        ++update_turn;
    }
}


void Canvas::remove_canvas_elem(CANVAS_ELEM elem)
{
    int& elem_name=canvas_elem_switch(elem);
    --elem_name;
}

int Canvas::item_number(CANVAS_ELEM elem)
{
    return canvas_elem_switch(elem);
}

Canvas::Canvas()
{
    int brick_num=4;
    int ordin_food_num=6;
    update_turn=0;

    brick=0;
    ordin_food=0;
    life_food=0;
    sup_food=0;
    sdown_food=0;
    strength_food=0;

    random_create_elem(brick_num,BRICK);
    random_create_elem(ordin_food_num,ORDIN_FOOD);
}

void User::snake_move_cur_direc()
{
    snake->move_snack(direc);
}

CANVAS_ELEM User::user_update_snake()
{
    return snake->update_snake(direc);
}

void Canvas::elem_decrease(CANVAS_ELEM elem)
{
    --canvas_elem_switch(elem);
}

User::User()
{
    snake=new Snake;
    DIREC d;
    d=snake->init_direc_get();

    direc=d;
    snake->set_snake_to_atlas();
}

User::User(Snake* s,DIREC d)
{
    snake=s;
    direc=d;
    snake->set_snake_to_atlas();
}

User::User(const User& u)
{
    snake=new Snake(u.snake->get_snake_body(),u.snake->life_get(),u.snake->length_get(),u.snake->speed_get());
    direc=u.direc;
}

User::~User()
{
    delete snake;
}



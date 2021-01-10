#ifndef BASIC_ELEMENT_H
#define BASIC_ELEMENT_H

#include<queue>
#include<cstdlib>
#include<time.h>
#include<iostream>
#include<cmath>
#include<string>

//define mode
enum MODE{AI,MAN};
//define operations
enum OPS{UP_op,DOWN_op,LEFT_op,RIGHT_op,W_op,A_op,S_op,D_op};
//define four directions
enum DIREC{UP,DOWN,LEFT,RIGHT,NO_DIREC};
//define element type of canvas
enum CANVAS_ELEM{NONE,SNAKE,BRICK,ORDIN_FOOD,LIFE_FOOD,SUP_FOOD,SDOWN_FOOD,STRENGTH_FOOD};
//define coordinate points

const int ATLAS_LINE_NUM=40;
const int ATLAS_COL_NUM=40;

void test_show_atlas();//test func

struct node
{
    int x;
    int y;
    node(int a,int b):x(a),y(b){}
    node(){x=-1;y=-1;}
};


class Canvas;

class Snake
{
    private:
        std::queue<node> body;//靠后的节点表示头
        int life_num;
        int length;
        int speed;//negative speed is valid, cause its used to calculate interval
        DIREC init_direc;//the direction where body grows

        CANVAS_ELEM next_elem;
        node next_elem_pos;

        void add_node_snake(DIREC direc);
        void pop_tail_snake();


    public:
        Snake();
        Snake(std::queue<node> snake_body,int life,int len,int sp);
        Snake(const Snake& s);
        std::queue<node> get_snake_body();
        void move_snack(DIREC direc);
        CANVAS_ELEM update_snake(DIREC direc);
        void auto_move();//ai snake
        void auto_arrange_pos(Canvas canvas);
        void set_snake_to_atlas();

        void speed_up(){++speed;}
        void speed_down(){--speed;}
        int speed_get(){return speed;}

        void life_add(){++life_num;}
        void life_decrease(){--life_num;}
        int life_get(){return life_num;}
        int length_get(){return length;}
        DIREC init_direc_get(){return init_direc;}


        void test_show_snake();//a test function
};

class Canvas
{
    private:
        int update_turn;

        int brick;
        int ordin_food;
        int life_food;
        int sup_food;
        int sdown_food;
        int strength_food;

        int& canvas_elem_switch(CANVAS_ELEM canvas_elem);
        bool check_elem_pos_suit(node n,CANVAS_ELEM canvas_elem);

    public:
        Canvas();
        Canvas(int bk,int of,int lf,int spf,int sdf,int strf):
            brick(bk),ordin_food(of),life_food(lf),sup_food(spf),sdown_food(sdf),strength_food(strf){}
        void canvas_update();
        void remove_canvas_elem(CANVAS_ELEM elem);
        void modify_canvas();
        void random_create_elem(int num,CANVAS_ELEM canvas_elem);
        int item_number(CANVAS_ELEM elem);//return elem number
        void elem_decrease(CANVAS_ELEM elem);


};

class Atlas_init
{
public:
    Atlas_init();
    ~Atlas_init();
};

class User
{
    public:
        Snake* snake;
        DIREC direc;

    public:
        User();
        User(Snake* s,DIREC d);
        User(const User& u);
        ~User();
        void snake_move_cur_direc();
        void ai_set_direc(DIREC d){direc=d;}
        CANVAS_ELEM user_update_snake();

};


#endif // BASIC_ELEMENT_H

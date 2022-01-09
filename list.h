#ifndef _LIST_H_
#define _LIST_H_
#include <stdio.h>
#include <iostream>
#include <string>
#include <time.h>
using namespace std;

class tank;
class wall;
class bullet;
class bullet_list;
class tank_list;

#define map_length 9
#define map_width 9
#define tank_width 8
#define tank_length 4
#define wall_length 4
#define wall_width 8
#define init_myTank_xpos 33
#define init_myTank_ypos 17
#define bullet_length 2
#define bullet_width 2

void gotoxy(int x,int y)//光标定位函数
{
	COORD p;//定义结构体变量p
	HANDLE handle=GetStdHandle(STD_OUTPUT_HANDLE);//获取当前函数句柄
	p.X=x;p.Y=y;//将光标的目标移动位置传递给结构体
	SetConsoleCursorPosition(handle,p);//移动光标
}

//定义隐藏光标函数
void HideCursor()
{
	CONSOLE_CURSOR_INFO cursor;    
	cursor.bVisible = FALSE;    
	cursor.dwSize = sizeof(cursor);    
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);    
	SetConsoleCursorInfo(handle, &cursor);
}

class bullet
{
    private:
        string name;
        int attack;
        float speed;
        int direct;
        int pos[2];
        clock_t bullet_time = 0; //记录上一次该子弹移动一格的时间点，以便实现计时
    public:
        bullet(string a,int b,int *pos,float c=8,int d=1):name(a),direct(b),pos{*pos,*(pos+1)},speed(c),attack(d){
            //cout<<name<<"发射"<<endl;
        }
        ~bullet(){
            //cout<<name<<"击中目标"<<endl;
        }
        void move_bullet();
        void play_bullet(int cls);
        void cls_bullet();
        int* get_bulletPos() { return pos; }
        bullet* next;
        clock_t get_bullet_time() {return bullet_time;}
        int get_bulletDir()  { return direct; }
        string get_name() { return name;}
        int bullet_attack() { return attack; }
};

class wall
{
    private:
        int pos[2];
        bool is_exist;  //墙存在与否的状态
        int life;
    public:
        wall(int a=0):life(a)  {}
        void set_pos(int x,int y)  { pos[0] = x;pos[1] = y;}
        int* get_pos()    { return pos;}
        void cls_wall();
        void set_state(bool state) {is_exist = state;}
        bool get_state()  { return is_exist; }
        void hit_wall(int attack)   { life = life - attack; }
        int get_life()    { return life; }
        void set_life(int life_)   { life = life_; }
};

void wall::cls_wall()
{
    for(int i=0;i<wall_length;i++)
    {
        gotoxy(pos[1],pos[0]+i);
        for(int j=0;j<wall_width;j++)
        {
            cout<<' ';
        }
    }

    pos[0] = 0;
    pos[1] = 0;
    is_exist = 0;
}
class tank
{
    private:
        string name;    //名字
        int life;       //血量
        float speed;    //移动速度
        int direct;     //方向
        float ack_spd;  //攻击速度
        int pos[2];     //坦克位置
        clock_t tank_time = 0; //记录上一次该tank移动一格的时间点，以便实现计时
        clock_t shoot_time = 0; //记录上一次发射子弹的时间
        clock_t act_time = 0;   //记录上一次坦克行动的时间
        static wall* walls; 
        static int tank_num;   //限制坦克数量
    public:
        tank(string a,int x=0,int y=0,int d=0,int b=3,float c=1,float e=1)
                :name(a),pos{x,y},life(b),direct(d),speed(c),ack_spd(e){
            //cout<<name<<"已生成"<<endl;
            tank_num++;
        }
        ~tank()
        {
            tank_num--;
            //cout<<name<<"被摧毁"<<endl;
        }
        
        void set_life(int enemy_ack);
        int get_life()      { return life;}
        int get_dir()       { return direct;}
        void set_tank();
        void play_tank(int cls);
        int* get_tankPos()   { return pos;}
        void move_tank(int spd,int dir);
        void turn_tankDir(int dir);
        void bullet_shoot(float spd);
        void cls_tank();
        string get_name() { return name; }
        void set_acttime(clock_t new_time)       { act_time = new_time; }
        clock_t get_acttime()                     { return act_time; }
        static void set_wall(wall* walls_)        { walls = walls_; }
        wall* get_wall()  { return walls;}
        static int get_tanknum()                  { return tank_num; }
        void hit_tank(int attack)                 { life = life - attack; }
        static void reduce_tanknum()              { tank_num--; }
        tank *next;
         
};

void tank::cls_tank()
{
    for(int i=0;i<tank_length;i++)
    {
        gotoxy(pos[1],pos[0] + i);
        for(int j=0;j<tank_width;j++)
        {
            cout<<' ';
        }
    } 
}

class tank_list
{
     public:
        tank* head;
        tank* tail;
        tank_list(){head=NULL;tail=NULL;}
        void add_tank(string name,int pos_x,int pos_y,int dir);//链表结点添加
        void del_tank(tank* atank);
        tank* Gethead(){return head;}
        void del_allTank();
};

void tank_list::add_tank(string name,int pos_x,int pos_y,int dir=2)
{
    tank* atank = new tank(name,pos_x,pos_y,dir);
    if(head == NULL)
    {
        head = atank;
        atank->next = NULL;
        tail = atank;
        
    }
    else
    {
        tail->next = atank;
        tail = atank;
        tail->next = NULL;
        
        
    }
}

void tank_list::del_tank(tank* atank)
{
    tank *p = head,*last_p = p;
    tank *temp;
    //如果只有一个节点
    if(p == tail)
    {
        p->cls_tank();
        delete p;
        head = NULL;
        tail = NULL; 
    }
    //如果删除的是头节点
    else if(head == atank)
    {
        head = head->next;
        p->cls_tank();
        delete p;
    }
    else{
        while(p != NULL)
        {
            temp = p;
            //删除尾节点
            if(p == tail && p == atank)
            {
                tail = last_p;
                tail->next = NULL;
                p->cls_tank();
                delete p;
                break;

            }
            else if(p == atank)
            {
                last_p->next = p->next;
                p->cls_tank();
                delete p;
                break;
            }
            last_p = temp;
            p = temp->next;
        }
    }
    
    
    
};

void tank_list::del_allTank()
{
    tank *p = head;
    while(p!= NULL)
    {
        delete p;
        p = p->next;
    }
}

class bullet_list
{
    public:
        bullet* head;
        bullet* tail;
        bullet_list(){head=NULL;tail=NULL;}
        void add_bullet(string name,int dir,int *pos,float spd);//链表结点添加
        void del_bullet(bullet* abullet);
        bullet* Gethead(){return head;}
};

void bullet_list::add_bullet(string name,int dir,int *pos,float spd = 8)
{
    bullet* abullet = new bullet(name,dir,pos,spd);
    if(head == NULL)
    {
        head = abullet;
        abullet->next = NULL;
        tail = abullet;
    }
    else
    {
        tail->next = abullet;
        abullet->next = NULL;
        tail = abullet;
    }
}

void bullet_list::del_bullet(bullet* abullet)
{
    bullet *p = head,*last_p = p;
    //如果只有一个节点
    if(p == tail)
    {
        p->cls_bullet();
        delete p;
        head = NULL;
        tail = NULL; 
    }
    //如果删除的是头节点
    else if(head == abullet)
    {
        head  = head->next;
        p->cls_bullet();
        delete p;
    }
    else
    {
        while(p != NULL)
        {
            if(p == abullet)
            {
                last_p->next = p->next;
                p->cls_bullet();
                delete p;
                break;
            }
            last_p = p;
            p = p->next;
        }
    }
}



#endif
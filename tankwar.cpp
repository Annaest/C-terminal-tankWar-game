/*
C/c++实验作业
项目标题：坦克大战
作者：Anna
日期：2021/12/24
*/
#include <iostream>
#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include <time.h>
#include <string>
#include "list.h"
#include <stdlib.h>

using namespace std;

//地图边界
#define map_left_limite   1
#define map_right_limite  72  //map_width*wall_width
#define map_up_limite     1
#define map_down_limite   37  //map_length*wall_length


void keyboard_down(tank* szTank);
void keyboard_up(tank* szTank);
void wait_game();
void init_game(tank* myTank,wall* walls);
void enter_game(tank* myTank,wall* walls);
void creat_map(wall* walls);
void control_enemyTank(tank* mytank);
void control_bullet(wall* walls,tank* mytank);
void enmeyTank_system(tank* sztank);

//tank与其他物体碰撞
int tank_collide(int *pos,int dir,tank* szTank);
int tank_collide(int *pos,int dir,wall* walls);
int tank_collide(int *pos,int dir,bullet* bullets);
int tank_tank_collide(int *pos,int direct);
//子弹与其他物体碰撞
int bullet_collide(bullet* abullet);
int bullet_bullet_collide(bullet* bullets);

int collide(tank* szTank,wall* walls);
int collide(wall* walls,bullet* bullets);
int collide(tank* szTank,bullet* bullets);
int bullet_collide(bullet* abullet,wall *walls);
int border(string name,int *pos,int dir);
int border(bullet* abullet);
int chance(float chance);
int game_state = 0;
int game_time = 50;
bullet_list* bullet_alist = new bullet_list();
tank_list* tank_alist = new tank_list();
wall* tank::walls;
int tank::tank_num = 0;
char tank_up[tank_length][tank_width+1] = {
                                              {" _/||\\_ "},
                                              {"/**##**\\"},
                                              {"|*####*|"},
                                              {"\\_*##*_/"}};
char tank_down[tank_length][tank_width+1] = {
                                              {"/_*##*_\\"},
                                              {"|*####*|"},
                                              {"\\_*##*_/"},
                                              {"  \\||/  "}};
char tank_left[tank_length][tank_width+1] = {
                                              {"  /+--+\\"},
                                              {"=/#####|"},
                                              {"=\\#####|"},
                                              {"  \\+--+/"}};
char tank_right[tank_length][tank_width+1] = {
                                              {"/+--+\\  "},
                                              {"|#####\\="},
                                              {"|#####/="},
                                              {"\\+--+/  "}};
char wall_type[wall_length][wall_width+1] = {{"########"},
                                             {"########"},
                                             {"########"},
                                             {"########"}};
char home_type[wall_length][wall_width+1] = {{"   **   "},
                                             {" ****** "},
                                             {" ****** "},
                                             {"   **   "}};
char wall1_type[wall_length][wall_width+1] = {{"++++++++"},
                                              {"++++++++"},
                                              {"++++++++"},
                                              {"++++++++"}};
char bullet_up[bullet_length][bullet_width+1] =    {{"/\\"},
                                                    {"||"}};
char bullet_right[bullet_length][bullet_width+1] = {{"-\\"},
                                                    {"-/"}};
char bullet_down[bullet_length][bullet_width+1] =  {{"||"},
                                                    {"\\/"}};
char bullet_left[bullet_length][bullet_width+1] =  {{"/-"},
                                                    {"\\-"}};

void bullet::move_bullet()
{
    if((clock() - bullet_time)/1000.0*speed>=1.0)
    {
        //每次移动的格数
        play_bullet(1);
        bullet_time = clock();
    } 
}
void bullet::play_bullet(int cls)
{
    if(direct == 0)
    {
        pos[0] = pos[0] - cls;
        gotoxy(pos[1],pos[0]+bullet_length);
        for(int j=0;j<bullet_width;j++)
        {
            cout<<' ';
        }
        cout<<endl;
        for(int i=0;i<bullet_length;i++)
        {
            gotoxy(pos[1],pos[0]+i);
            for(int j=0;j<bullet_width;j++)
            {
                cout<<bullet_up[i][j];
            }
        }
    }
    else if(direct == 1)
    {
        pos[1] = pos[1] + cls;
        for(int i=0;i<bullet_length;i++)
        {
            gotoxy(pos[1]-1,pos[0]+i);
            cout<<' ';
        }
        for(int j=0;j<bullet_width;j++)
        {
            gotoxy(pos[1]+j,pos[0]);
            for(int i=0;i<bullet_length;i++)
            {
                gotoxy(pos[1]+j,pos[0]+i);
                cout<<bullet_right[i][j];
            }
        }
    }
    else if(direct == 2)
    {
        pos[0] = pos[0] + cls;
        gotoxy(pos[1],pos[0]-1);
        for(int j=0;j<bullet_width;j++)
        {
            cout<<' ';
        }
        cout<<endl;
        for(int i=0;i<bullet_length;i++)
        {
            gotoxy(pos[1],pos[0]+i);
            for(int j=0;j<bullet_width;j++)
            {
               
                cout<<bullet_down[i][j];
            }
        }
    }
    else if(direct == 3)
    {
        pos[1] = pos[1] - cls;
        for(int i=0;i<bullet_length;i++)
        {
            gotoxy(pos[1]+bullet_width,pos[0]+i);
            cout<<' ';
        }
        for(int j=0;j<bullet_width;j++)
        {
            gotoxy(pos[1]+j,pos[0]);
            for(int i=0;i<bullet_length;i++)
            {
                gotoxy(pos[1]+j,pos[0]+i);
                cout<<bullet_left[i][j];
            }
        }
    }
}
void bullet::cls_bullet()
{
    for(int i=0;i<bullet_width;i++)
        {
            gotoxy(pos[1],pos[0] + i);
            for(int j=0;j<bullet_length;j++)
            {
                cout<<' ';
            }
        } 
}

void tank::set_life(int enemy_ack)
{
    life = life - enemy_ack;
}
void tank::play_tank(int cls)
{
    if(direct == 0)
    {
        pos[0] = pos[0] - cls;
        gotoxy(pos[1],pos[0]+tank_length);
        for(int j=0;j<tank_width;j++)
        {
            cout<<' ';
        }
        cout<<endl;
        for(int i=0;i<tank_length;i++)
        {
            gotoxy(pos[1],pos[0]+i);
            for(int j=0;j<tank_width;j++)
            {
               
                cout<<tank_up[i][j];
            }
        }
    }
    else if(direct == 1)
    {
        pos[1] = pos[1] + cls;
        for(int i=0;i<tank_length;i++)
        {
            gotoxy(pos[1]-1,pos[0]+i);
            cout<<' ';
        }
        for(int j=0;j<tank_width;j++)
        {
            gotoxy(pos[1]+j,pos[0]);
            for(int i=0;i<tank_length;i++)
            {
                gotoxy(pos[1]+j,pos[0]+i);
                cout<<tank_right[i][j];
            }
        }
    }
    else if(direct == 2)
    {
        pos[0] = pos[0] + cls;
        gotoxy(pos[1],pos[0]-1);
        for(int j=0;j<tank_width;j++)
        {
            cout<<' ';
        }
        cout<<endl;
        for(int i=0;i<tank_length;i++)
        {
            gotoxy(pos[1],pos[0]+i);
            for(int j=0;j<tank_width;j++)
            {
               
                cout<<tank_down[i][j];
            }
        }
    }
    else if(direct == 3)
    {
        pos[1] = pos[1] - cls;
        for(int i=0;i<tank_length;i++)
        {
            gotoxy(pos[1]+tank_width,pos[0]+i);
            cout<<' ';
        }
        for(int j=0;j<tank_width;j++)
        {
            gotoxy(pos[1]+j,pos[0]);
            for(int i=0;i<tank_length;i++)
            {
                gotoxy(pos[1]+j,pos[0]+i);
                cout<<tank_left[i][j];
            }
        }
    }
}
void tank::move_tank(int spd,int dir)
{   
    speed = spd;
    direct = dir;
    turn_tankDir(dir);
    if(dir == 1)  speed = spd*2;
    else if(dir == 3)  speed = spd*2;
    //边界限制，与墙体碰撞，与坦克碰撞
    if(border(name,pos,direct) && tank_collide(pos,direct,walls) && tank_tank_collide(pos,direct))
    {
        if((clock() - tank_time)/1000.0*speed>=1.0)
        {
            //每次移动的格数
            play_tank(1);
            tank_time = clock();
        }
    }      
}
void tank::turn_tankDir(int dir)
{
    direct = dir;
    if(direct == 0)
    {
        for(int i=0;i<tank_length;i++)
        {
            gotoxy(pos[1],pos[0]+i);
            for(int j=0;j<tank_width;j++)
            {
                cout<<tank_up[i][j];
            }
        }
    }
    else if(direct == 1)
    {
        for(int i=0;i<tank_length;i++)
        {
            gotoxy(pos[1],pos[0]+i);
            for(int j=0;j<tank_width;j++)
            {
                cout<<tank_right[i][j];
            }
        }
    }
    else if(direct == 2)
    {
        for(int i=0;i<tank_length;i++)
        {
            gotoxy(pos[1],pos[0]+i);
            for(int j=0;j<tank_width;j++)
            {
                cout<<tank_down[i][j];
            }
        }
    }
    else if(direct == 3)
    {
        for(int i=0;i<tank_length;i++)
        {
            gotoxy(pos[1],pos[0]+i);
            for(int j=0;j<tank_width-1;j++)
            {
                cout<<tank_left[i][j];
            }
        }
    }
}
//坦克发射子弹
void tank::bullet_shoot(float bullet_spd)
{
    //记录当前发射子弹的坦克位置
    int bullet_pos[2];
    //限制子弹发射间隔
    if(clock() - shoot_time > 500)
    {
        if(direct == 0)
        {
            //计算子弹生成位置
            bullet_pos[0] = pos[0] - 2;
            bullet_pos[1] = pos[1] + 3;
        }
        else if(direct == 1)
        {
            bullet_pos[0] = pos[0] + 1;
            bullet_pos[1] = pos[1] + 8;
            bullet_spd = bullet_spd*2;
        }
        else if(direct == 2)
        {
            bullet_pos[0] = pos[0] + 4;
            bullet_pos[1] = pos[1] + 3;
        }
        else if(direct == 3)
        {
            bullet_pos[0] = pos[0] + 1;
            bullet_pos[1] = pos[1] - 2;
            bullet_spd = bullet_spd*2;
        }
        //添加子弹进入子弹链表，传入子弹生成位置
        bullet_alist->add_bullet(name + "_bullet",direct,bullet_pos,bullet_spd);
        //计时
        shoot_time = clock();
    }
}

//游戏开始与结束界面
void wait_game()
{
    char word_tank[][100] =  
    {{"_____________________________________________________________"},
     {"| 作者: rjh                                                 |"},
     {"|                                                           |"},
     {"|         #########      #       #     #   #   #            |"},
     {"|             #         # #      # #   #   # #              |"},
     {"|             #        #####     #  #  #   ##               |"},
     {"|             #       #     #    #    ##   # #              |"},
     {"|             #      #       #   #     #   #   #            |"},
     {"|                                                           |"},
     {"|           #       #       #    #       ###                |"},
     {"|            #     # #     #    # #      #  #               |"},
     {"|             #   #   #   #    #####     ###                |"},
     {"|              # #     # #    #     #    #  #               |"},
     {"|               #       #    #       #   #   #              |"},
     {"|                                                           |"},
     {"|                         坦克大战                          |"},
     {"|          使用w,a,s,d控制坦克方向,按j发射子弹              |"},
     {"|_______________________按P开始游戏_________________________|"}};
    for(int i=0;i<sizeof(word_tank) / sizeof(word_tank[0]);i++)
    {
        for(int j=0;j<100;j++)
        {
            cout<<word_tank[i][j];
        }
        cout<<endl;
    }
  
}

void creat_map(wall* walls)
{
    int x_pos=0,y_pos=0;
    
    //创建地图,1:墙壁，2基地
    int map[map_length][map_width] =   {{0,0,0,0,0,0,0,0,0},
                                        {0,1,1,1,1,1,1,1,0},
                                        {0,1,0,0,1,0,0,1,0},
                                        {0,1,1,1,1,1,1,1,0},
                                        {0,1,0,0,1,0,0,1,0},
                                        {1,1,1,1,1,1,1,1,1},
                                        {1,0,0,0,1,0,0,0,1},
                                        {0,1,0,3,3,3,0,1,0},
                                        {0,1,0,3,2,3,0,1,0}};
    //绘制地图边界
    for(int i=0;i<=map_length*wall_length+1;i++)
    {
        if(i==1)     {gotoxy(0,i);cout<<'|';}
        if(i==0) cout<<'_';
        else if(i>1) cout<<'|';
        for(int j=0;j<map_width*wall_width;j++)
        {
            
            if(i==0 || i==map_length*wall_length+1) cout<<'_';
            else cout<<' ';
        }
        if(i>=1) cout<<'|'<<endl;
    }
    //绘制地图
    for(int i=0,p=0;i<map_length;i++)       //地图竖轴
    {
        for(int j=0;j<map_width;j++,p++)
        {
            x_pos = i*wall_length + 1;
            y_pos = j*wall_width + 1;
            if(map[i][j])   
            {
                (walls+p)->set_pos(x_pos,y_pos);
                if(map[i][j] == 1)
                {
                    for(int k=0;k<wall_length;k++)
                    {
                        gotoxy(y_pos,x_pos+k);
                        cout<<wall_type[k];
                        (walls+p)->set_life(2);
                    }
                }
                //cout<<x_pos<<endl;
                if(map[i][j] == 2)
                {
                    for(int k=0;k<wall_length;k++)
                    {
                        gotoxy(y_pos,x_pos+k);
                        cout<<home_type[k];
                        (walls+p)->set_life(1);
                    }
                }
                if(map[i][j] == 3)
                {
                    for(int k=0;k<wall_length;k++)
                    {
                        gotoxy(y_pos,x_pos+k);
                        cout<<wall1_type[k];
                        (walls+p)->set_life(3);
                    }
                }
                (walls+p)->set_state(1);
            }
            else 
            {
                (walls+p)->set_pos(0,0);
                (walls+p)->set_state(0);
            } 
        }   
    }
    //cout<<"载入地图"<<endl;
    //绘制信息框
     for(int i=0;i<=map_length*wall_length+1;i++)
    {
        gotoxy(map_width*wall_width+5,i);
        if(i==1)     {gotoxy(map_width*wall_width+5,i);cout<<'|';}
        if(i==0) cout<<'_';
        else if(i>1) cout<<'|';
        for(int j=0;j<20;j++)
        {
            if(i==0 || i==map_length*wall_length+1) cout<<'_';
            else cout<<' ';
        }
        if(i>=1) cout<<'|'<<endl;
    }
}

void init_game(tank* myTank,wall* walls)
{
    system("cls");
    creat_map(walls);
    myTank->play_tank(0);
    game_state = 2;
    //cout<<endl<<"游戏初始化"<<endl;  
}

void enter_game(tank* myTank,wall* walls)
{
    //游戏中键盘响应
    keyboard_down(myTank);
    control_enemyTank(myTank);
    control_bullet(walls,myTank);
    gotoxy(map_right_limite+8,15);
    cout<<"生命:";
    int num = myTank->get_life();
    for(int i=0;i<num;i++)  cout<<" # ";
    cout<<"   ";
    if((walls + 76)->get_state() == 0 || myTank->get_life() <= 0 || (_kbhit() && _getch() == 'p')) game_state = 3;
    if(game_time<=0)                 game_state = 4;
}
void exec_game()
{
    cout<<"                                                                       "<<endl;
    cout<<"                  --------------游戏结束-----------------              "<<endl;
    cout<<"                                                                       "<<endl;
    Sleep(2000);
    //system("cls");
    exit(0);
    
}

void keyboard_down(tank* myTank)
{
    //tank移动功能
    if(GetKeyState('W')<0 || GetKeyState('w')<0)  
    { 
       myTank->move_tank(8.0,0);
    }
    else if(GetKeyState('d')<0 || GetKeyState('D')<0)  
    { 
        myTank->move_tank(8.0,1);
    }
    else if(GetKeyState('s')<0 || GetKeyState('S')<0)  
    { 
        myTank->move_tank(8.0,2);
    }
    else if(GetKeyState('a')<0 || GetKeyState('A')<0)  
    { 
        myTank->move_tank(8.0,3);
    }
    if(GetKeyState('j')<0 || GetKeyState('J')<0)
    {
        //这里应加入tank的位置
        myTank->bullet_shoot(20);
    }
}


//坦克与墙碰撞
int tank_collide(int *pos,int dir,wall* walls)
{
    int flag=1;
    int last_num = -1;
    int x_pos,y_pos;
    x_pos = *pos;
    y_pos = *(pos+1);
    //地图中以大格计数,第几个格子
    int num = 0;
    //坦克前方墙的坐标
    int tank_wall_xpos = 0;
    int tank_wall_ypos = 0;
    //碰撞扫描坐标
    int tankScan_length=0,tankScan_width=0,wallScan_length,wallScan_width;
    bool walls_state;
    tankScan_length = x_pos + tank_length;
    tankScan_width = y_pos + tank_width;
    //可进行优化减少循环次数
    //向上碰撞
    if(dir == 0)
    {
        for(int j=y_pos;j<tankScan_width;j++)
        {
            //地图中以大格计数,num为几个格子
            num = (x_pos-1)/tank_length*map_width + (j-1)/tank_width - map_width;
            if(num == last_num) continue;    //避免同一个墙壁重复计算
            //坦克前方墙的坐标
            tank_wall_xpos = *((walls+num)->get_pos());
            tank_wall_ypos = *((walls + num)->get_pos() + 1);
            wallScan_length = tank_wall_xpos + wall_length;
            wallScan_width = tank_wall_ypos + wall_width;
            walls_state = (walls+num)->get_state();
            for(int k=tank_wall_ypos;k<wallScan_width;k++)
            {
                if(x_pos-wall_length == tank_wall_xpos && j==k && walls_state) 
                {
                    flag = 0;
                    break;
                }
            }  
            last_num = num;
        }
    }
    //向右碰撞
    else if(dir == 1)
    {
        for(int j=x_pos;j<tankScan_length;j++)
        {
            //地图中以大格计数,第几个格子
            num = (j-1)/tank_length*map_width + (y_pos-1)/tank_width + 1;
            if(num == last_num) continue;
            //坦克前方墙的坐标
            tank_wall_xpos = *((walls+num)->get_pos());
            tank_wall_ypos = *((walls + num)->get_pos() + 1);
            wallScan_length = tank_wall_xpos + wall_length;
            wallScan_width = tank_wall_ypos + wall_width;
            walls_state = (walls+num)->get_state();
            for(int k=tank_wall_xpos;k<wallScan_length;k++)
            {
                if(y_pos+wall_width == tank_wall_ypos && j==k && walls_state) 
                {
                    flag = 0;
                    break;
                }
            }  
            last_num = num;
        }
    }
    //向下碰撞
    else if(dir == 2)
    {
        for(int j=y_pos;j<tankScan_width;j++)
        {
            //地图中以大格计数,第几个格子
            num = (x_pos-1)/tank_length*map_width + (j-1)/tank_width + map_width;
            if(num == last_num) continue;
            //坦克前方墙的坐标
            tank_wall_xpos = *((walls+num)->get_pos());
            tank_wall_ypos = *((walls + num)->get_pos() + 1);
            wallScan_length = tank_wall_xpos + wall_length;
            wallScan_width = tank_wall_ypos + wall_width;
            walls_state = (walls+num)->get_state();
            for(int k=tank_wall_ypos;k<wallScan_width;k++)
            {
                if(x_pos+wall_length == tank_wall_xpos && j==k && walls_state) 
                {
                    flag = 0;
                    break;
                }
            }  
            last_num = num;
        }
    }
    //向左碰撞
    else if(dir == 3)
    {
        for(int j=x_pos;j<tankScan_length;j++)
        {
            //地图中以大格计数,第几个格子
            num = (j-1)/tank_length*map_width + (y_pos-1)/tank_width - 1;
            if(num == last_num) continue;
            //坦克前方墙的坐标
            tank_wall_xpos = *((walls+num)->get_pos());
            tank_wall_ypos = *((walls + num)->get_pos() + 1);
            wallScan_length = tank_wall_xpos + wall_length;
            wallScan_width = tank_wall_ypos + wall_width;
            walls_state = (walls+num)->get_state();
            for(int k=tank_wall_xpos;k<wallScan_length;k++)
            {
                if(y_pos-wall_width == tank_wall_ypos && j==k && walls_state) 
                {
                    flag = 0;
                    break;
                }
            }  
            last_num = num;
        }
    }
    
    return flag;
}
//坦克与坦克碰撞
int tank_tank_collide(int *pos,int dir)
{
    int flag = 1;
    tank *p = tank_alist->head,*temp;
    //子弹碰撞范围
    int first_tankScan_length,first_tankScan_width;
    int second_tankScan_length,second_tankScan_width;
    int first_tank_xpos,first_tank_ypos;
    int second_tank_xpos,second_tank_ypos;
    int tank_dir = dir;
    first_tank_xpos = *pos;
    first_tank_ypos = *(pos+1);
    while(p)
    {
        if(*p->get_tankPos() == *pos && *(p->get_tankPos()+1) == *(pos+1)) 
        {
            p = p->next;
            continue;
        }
        second_tank_xpos = *p->get_tankPos();
        second_tank_ypos = *(p->get_tankPos() + 1);
        //碰撞范围检索
        first_tankScan_length = first_tank_xpos + tank_length;
        first_tankScan_width = first_tank_ypos + tank_width;
        second_tankScan_length = second_tank_xpos + tank_length;
        second_tankScan_width = second_tank_ypos + tank_width;
        //向上碰撞
        if(tank_dir == 0)
        {
            for(int j=first_tank_ypos;j<first_tankScan_width;j++)
            {
                for(int k=second_tank_ypos;k<second_tankScan_width;k++)
                {
                    if(first_tank_xpos - tank_length == second_tank_xpos && j==k) 
                    {
                        flag = 0;
                        return flag;
                    }
                }  
            }
        }
        //向右碰撞
        else if(tank_dir == 1)
        {
            for(int j=first_tank_xpos;j<first_tankScan_length;j++)
            {
                for(int k=second_tank_xpos;k<second_tankScan_length;k++)
                {
                    if(first_tank_ypos + tank_width == second_tank_ypos && j==k) 
                    {
                        flag = 0;
                        return flag;
                    }
                }  
            }
        }
        //向下碰撞
        else if(tank_dir == 2)
        {
            for(int j=first_tank_ypos;j<first_tankScan_width;j++)
            {
                for(int k=second_tank_ypos;k<second_tankScan_width;k++)
                {
                    if(first_tank_xpos + tank_length == second_tank_xpos && j==k) 
                    {
                        flag = 0;
                        return flag;
                    }
                } 
            }
        }
        //向左碰撞
        else if(tank_dir == 3)
        {
            for(int j=first_tank_xpos;j<first_tankScan_length;j++)
            {
                for(int k=second_tank_xpos;k<second_tankScan_length;k++)
                {
                    if(first_tank_ypos - tank_width == second_tank_ypos && j==k) 
                    {
                        flag = 0;
                        return flag;
                    }
                }  
            }
        }
        p = p->next;
    }

    return flag;
}
//子弹击中坦克
int bullet_collide(bullet* abullet)
{
    //flag为子弹消失标志
    int flag = 1,atk=0;
    tank* p = tank_alist->head,*temp;
    int bullet_dir = abullet->get_bulletDir();
    int bulletScan_length,bulletScan_width,bullet_xpos,bullet_ypos,tank_xpos,tank_ypos;
    int tankScan_length,tankScan_width;
    if(abullet->get_name() == "enemy_tank_bullet")
    {
        bullet_xpos = *abullet->get_bulletPos();
        bullet_ypos = *(abullet->get_bulletPos()+1);
        tank_xpos = *p->get_tankPos();
        tank_ypos = *(p->get_tankPos() + 1);
        bulletScan_length = bullet_xpos + bullet_length;
        bulletScan_width = bullet_ypos + bullet_width;
        tankScan_length = tank_xpos + tank_length;
        tankScan_width = tank_ypos + tank_width;
        if(bullet_dir == 0)
        {
            for(int j=bullet_ypos;j<bulletScan_width;j++)
            {
                for(int k=tank_ypos;k<tankScan_width;k++)
                {
                    if(bullet_xpos - tank_xpos <= 3 && bullet_xpos - tank_xpos >=-1 && j==k) 
                    {
                        flag = 0;
                        atk = 1;
                        break;
                    }
                }  
                if(atk == 1) 
                {
                    p->hit_tank(abullet->bullet_attack());
                    if(p->get_life() <= 0) tank_alist->del_tank(p);
                    break;
                } 
            }
        }
        //向右碰撞
        else if(bullet_dir == 1)
        {
            for(int j=bullet_xpos;j<bulletScan_length;j++)
            {
                for(int k=tank_xpos;k<tankScan_length;k++)
                {
                    if(tank_ypos - bullet_ypos <= 1 && tank_ypos - bullet_ypos >= -3 && j==k) 
                    {
                        flag = 0;
                        atk = 1;
                        break;
                    }
                }  
                if(atk == 1) 
                {
                    p->hit_tank(abullet->bullet_attack());
                    if(p->get_life() <= 0) tank_alist->del_tank(p);
                    break;
                } 
            }
        }
        //向下碰撞
        else if(bullet_dir == 2)
        {
            for(int j=bullet_ypos;j<bulletScan_width;j++)
            {
                for(int k=tank_ypos;k<tankScan_width;k++)
                {
                    if(tank_xpos - bullet_xpos <= 1 && tank_xpos - bullet_xpos >=-3 && j==k) 
                    {
                        flag = 0;
                        atk = 1;
                        break;
                    }
                } 
                if(atk == 1) 
                {
                    p->hit_tank(abullet->bullet_attack());
                    if(p->get_life() <= 0) tank_alist->del_tank(p);
                    break;
                } 
            }
        }
        //向左碰撞
        else if(bullet_dir == 3)
        {
            for(int j=bullet_xpos;j<bulletScan_length;j++)
            {
                for(int k=tank_xpos;k<tankScan_length;k++)
                {
                    if(bullet_ypos - tank_ypos <= 3 && bullet_ypos - tank_ypos >= -1 && j==k) 
                    {
                        flag = 0;
                        atk = 1;
                        break;
                    }
                }  
                if(atk == 1) 
                {
                    p->hit_tank(abullet->bullet_attack());
                    if(p->get_life() <= 0) tank_alist->del_tank(p);
                    break;
                } 
            }
        }
    }
    else if(abullet->get_name() == "my_tank_bullet")
    {
        p = p->next;
        while(p)
        {
            temp = p;
            atk = 0;   //击中标志
            bullet_xpos = *abullet->get_bulletPos();
            bullet_ypos = *(abullet->get_bulletPos()+1);
            tank_xpos = *p->get_tankPos();
            tank_ypos = *(p->get_tankPos() + 1);
            bulletScan_length = bullet_xpos + bullet_length;
            bulletScan_width = bullet_ypos + bullet_width;
            tankScan_length = tank_xpos + tank_length;
            tankScan_width = tank_ypos + tank_width;
            if(bullet_dir == 0)
            {
                for(int j=bullet_ypos;j<bulletScan_width;j++)
                {
                    for(int k=tank_ypos;k<tankScan_width;k++)
                    {
                        if(bullet_xpos - tank_xpos <= 3 && bullet_xpos - tank_xpos >=-1 && j==k) 
                        {
                            flag = 0;
                            atk = 1;
                            break;
                        }
                    }  
                    if(atk == 1) 
                    {
                        p->hit_tank(abullet->bullet_attack());
                        if(p->get_life() <= 0) tank_alist->del_tank(p);
                        break;
                    } 
                }
            }
            //向右碰撞
            else if(bullet_dir == 1)
            {
                for(int j=bullet_xpos;j<bulletScan_length;j++)
                {
                    for(int k=tank_xpos;k<tankScan_length;k++)
                    {
                        if(tank_ypos - bullet_ypos <= 1 && tank_ypos - bullet_ypos >= -3 && j==k) 
                        {
                            flag = 0;
                            atk = 1;
                            break;
                        }
                    }  
                    if(atk == 1) 
                    {
                        p->hit_tank(abullet->bullet_attack());
                        if(p->get_life() <= 0) tank_alist->del_tank(p);
                        break;
                    } 
                }
            }
            //向下碰撞
            else if(bullet_dir == 2)
            {
                for(int j=bullet_ypos;j<bulletScan_width;j++)
                {
                    for(int k=tank_ypos;k<tankScan_width;k++)
                    {
                        if(tank_xpos - bullet_xpos <= 1 && tank_xpos - bullet_xpos >=-3 && j==k) 
                        {
                            flag = 0;
                            atk = 1;
                            break;
                        }
                    } 
                    if(atk == 1) 
                    {
                        p->hit_tank(abullet->bullet_attack());
                        if(p->get_life() <= 0) tank_alist->del_tank(p);
                        break;
                    } 
                }
            }
            //向左碰撞
            else if(bullet_dir == 3)
            {
                for(int j=bullet_xpos;j<bulletScan_length;j++)
                {
                    for(int k=tank_xpos;k<tankScan_length;k++)
                    {
                        if(bullet_ypos - tank_ypos <= 3 && bullet_ypos - tank_ypos >= -1 && j==k) 
                        {
                            flag = 0;
                            atk = 1;
                            break;
                        }
                    }  
                    if(atk == 1) 
                    {
                        p->hit_tank(abullet->bullet_attack());
                        if(p->get_life() <= 0) tank_alist->del_tank(p);
                        break;
                    } 
                }
            }
            p = temp->next;
        }
    }
    
    return flag;
}
//子弹击中子弹
int bullet_bullet_collide(bullet* abullet)
{
    int flag = 1;
    bullet *p = bullet_alist->head,*temp;
    //子弹碰撞范围
    int first_bulletScan_length,first_bulletScan_width;
    int second_bulletScan_length,second_bulletScan_width;
    int first_bullet_xpos,first_bullet_ypos;
    int second_bullet_xpos,second_bullet_ypos;
    int bullet_dir = abullet->get_bulletDir();
    while(p && p != abullet)
    {
        temp = p;
        first_bullet_xpos = *abullet->get_bulletPos();
        first_bullet_ypos = *(abullet->get_bulletPos()+1);
        second_bullet_xpos = *p->get_bulletPos();
        second_bullet_ypos = *(p->get_bulletPos() + 1);
        //碰撞范围检索
        first_bulletScan_length = first_bullet_xpos + bullet_length;
        first_bulletScan_width = first_bullet_ypos + bullet_width;
        second_bulletScan_length = second_bullet_xpos + bullet_length;
        second_bulletScan_width = second_bullet_ypos + bullet_width;
        //向上碰撞
        if(bullet_dir == 0)
        {
            for(int j=first_bullet_ypos;j<first_bulletScan_width;j++)
            {
                for(int k=second_bullet_ypos;k<second_bulletScan_width;k++)
                {
                    if(first_bullet_xpos - second_bullet_xpos <= 1 && first_bullet_xpos - second_bullet_xpos >=-1 && j==k) 
                    {
                        flag = 0;
                        break;
                    }
                }  
                if(flag == 0)  bullet_alist->del_bullet(p);break;
            }
        }
        //向右碰撞
        else if(bullet_dir == 1)
        {
            for(int j=first_bullet_xpos;j<first_bulletScan_length;j++)
            {
                for(int k=second_bullet_xpos;k<second_bulletScan_length;k++)
                {
                    if(second_bullet_ypos - first_bullet_ypos <= 1 && second_bullet_ypos - first_bullet_ypos >= -1 && j==k) 
                    {
                        flag = 0;
                        break;
                    }
                }  
                if(flag == 0)  bullet_alist->del_bullet(p);break;
            }
        }
        //向下碰撞
        else if(bullet_dir == 2)
        {
            for(int j=first_bullet_ypos;j<first_bulletScan_width;j++)
            {
                for(int k=second_bullet_ypos;k<second_bulletScan_width;k++)
                {
                    if(second_bullet_xpos - first_bullet_xpos <= 1 && second_bullet_xpos - first_bullet_xpos >=-1 && j==k) 
                    {
                        flag = 0;
                        break;
                    }
                } 
                if(flag == 0)  bullet_alist->del_bullet(p);break;
            }
        }
        //向左碰撞
        else if(bullet_dir == 3)
        {
            for(int j=first_bullet_xpos;j<first_bulletScan_length;j++)
            {
                for(int k=second_bullet_xpos;k<second_bulletScan_length;k++)
                {
                    if(first_bullet_ypos - second_bullet_ypos <= 1 && first_bullet_ypos - second_bullet_ypos >= -1 && j==k) 
                    {
                        flag = 0;
                        break;
                    }
                }  
               if(flag == 0)  bullet_alist->del_bullet(p);break;
            }
        }
        p = temp->next;
    }
    return flag;
}
//子弹击中墙壁
int bullet_collide(bullet* abullet,wall *walls)
{
    int flag=1;
    int x_pos,y_pos,dir;
    x_pos = *abullet->get_bulletPos();
    y_pos = *(abullet->get_bulletPos()+1);
    dir = abullet->get_bulletDir();
    //地图中以大格计数,第几个格子
    int num = 0,last_num=-1;
    //坦克前方墙的坐标
    int bullet_wall_xpos = 0;
    int bullet_wall_ypos = 0;
    //碰撞扫描坐标
    int bulletScan_length=0,bulletScan_width=0,wallScan_length,wallScan_width;
    bulletScan_length = x_pos + bullet_length;
    bulletScan_width = y_pos + bullet_width;
    //可进行优化减少循环次数
    //向上碰撞
    if(dir == 0)
    {
        for(int j=y_pos;j<bulletScan_width;j++)
        {
            //地图中以大格计数,第几个格子
            num = (x_pos-1)/wall_length*map_width + (j-1)/wall_width;
            if(num == last_num) continue;
            if((walls+num)->get_state())   
            {
                //坦克前方墙的坐标
                bullet_wall_xpos = *((walls + num)->get_pos());
                bullet_wall_ypos = *((walls + num)->get_pos() + 1);
                wallScan_length = bullet_wall_xpos + wall_length;
                wallScan_width = bullet_wall_ypos + wall_width;
                for(int k=bullet_wall_ypos;k<wallScan_width;k++)
                {
                    if(x_pos - bullet_wall_xpos <= wall_length-1 && j==k) 
                    {
                        flag = 0;   
                        break;
                    }
                }  
                if(flag == 0)  (walls + num)->hit_wall(abullet->bullet_attack());
                if((walls + num)->get_life() <=0) (walls + num)->cls_wall();
            }
            last_num = num;
        }  
    }
    //向右碰撞
    else if(dir == 1)
    {
        for(int j=x_pos;j<bulletScan_length;j++)
        {
            //地图中以大格计数,第几个格子
            num = (j-1)/wall_length*map_width + (y_pos-1)/wall_width;
            if(num == last_num) continue;
            if((walls+num)->get_state())   
            {
                //坦克前方墙的坐标
                bullet_wall_xpos = *((walls+num)->get_pos());
                bullet_wall_ypos = *((walls + num)->get_pos() + 1);
                wallScan_length = bullet_wall_xpos + wall_length;
                wallScan_width = bullet_wall_ypos + wall_width;
            
                for(int k=bullet_wall_xpos;k<wallScan_length;k++)
                {
                    if(bullet_wall_ypos - y_pos <= wall_width && j==k) 
                    {
                        flag = 0;
                        break;
                    }
                } 
                if(flag == 0)  (walls + num)->hit_wall(abullet->bullet_attack());
                if((walls + num)->get_life() <=0) (walls + num)->cls_wall(); 
            }
            last_num = num;
        }
    }
    //向下碰撞
    else if(dir == 2)
    {
        for(int j=y_pos;j<bulletScan_width;j++)
        {
            //地图中以大格计数,第几个格子
            num = (x_pos-1)/wall_length*map_width + (j-1)/wall_width;
            if(num == last_num) continue;
            if((walls+num)->get_state()) 
            {
                 //坦克前方墙的坐标
                bullet_wall_xpos = *((walls+num)->get_pos());
                bullet_wall_ypos = *((walls + num)->get_pos() + 1);
                wallScan_length = bullet_wall_xpos + wall_length;
                wallScan_width = bullet_wall_ypos + wall_width;
                for(int k=bullet_wall_ypos;k<wallScan_width;k++)
                {
                    if(bullet_wall_xpos - x_pos <= wall_length && j==k) 
                    {
                        flag = 0;
                        break;
                    }
                }  
                if(flag == 0)  (walls + num)->hit_wall(abullet->bullet_attack());
                if((walls + num)->get_life() <=0) (walls + num)->cls_wall();
            }
            last_num = num;
        }
    }
    //向左碰撞
    else if(dir == 3)
    {
        for(int j=x_pos;j<bulletScan_length;j++)
        {
            //地图中以大格计数,第几个格子
            num = (j-1)/wall_length*map_width + (y_pos-1)/wall_width;
            if(num == last_num) continue;
            if((walls+num)->get_state()) 
            {
                //坦克前方墙的坐标
                bullet_wall_xpos = *((walls+num)->get_pos());
                bullet_wall_ypos = *((walls + num)->get_pos() + 1);
                wallScan_length = bullet_wall_xpos + wall_length;
                wallScan_width = bullet_wall_ypos + wall_width;
                for(int k=bullet_wall_xpos;k<wallScan_length;k++)
                {
                    if(y_pos - bullet_wall_ypos <= wall_width && j==k) 
                    {
                        flag = 0;
                        break;
                    }
                }  
                if(flag == 0)  (walls + num)->hit_wall(abullet->bullet_attack());
                if((walls + num)->get_life() <=0) (walls + num)->cls_wall();
            }
            last_num = num;
        }
    }
    
    return flag;
}
//坦克的边界限制
int border(string name,int *pos,int dir)
{
    //是否可移动的标志
    int flag=1;
    if(name == "my_tank" || name == "enemy_tank")
    {
        if(*pos == map_up_limite && dir == 0)  flag = 0;
        else if(*(pos+1) + tank_width-1 == map_right_limite && dir==1)  flag = 0;
        else if(*pos + tank_length == map_down_limite && dir == 2)  flag = 0;
        else if(*(pos+1) == map_left_limite && dir==3)  flag = 0;
    }
    return flag;
}
//子弹的边界限制
int border(bullet* abullet)
{
    //是否可移动的标志
    int flag=1;
    int pos[2];
    int dir;
    pos[0] = *abullet->get_bulletPos();
    pos[1] = *(abullet->get_bulletPos() + 1);
    dir = abullet->get_bulletDir();
    if(*pos - map_up_limite <= bullet_length-1 && dir == 0)  flag = 0;
    else if(map_right_limite - *(pos+1) <= bullet_width-1 && dir==1)  flag = 0;
    else if(map_down_limite - *pos <= bullet_length && dir == 2)  flag = 0;
    else if(*(pos+1) - map_left_limite <= bullet_width  && dir==3)  flag = 0;
    return flag;
}
//敌方坦克控制
void control_enemyTank(tank* mytank)
{
    string name;
    int pos[2];
    int dir;
    tank *p = NULL;
    int i=0;
    int creat_time = 3;   //创建一辆tank的间隔
    static clock_t start_time = clock();
    srand(clock());
    int num = 0;
    //随机出现方式
    if(chance(0.4))       num = rand()%(map_width*wall_width/2) + map_width*wall_width/2-wall_width;
    else if(chance(0.4))  num = rand()%(map_width*wall_width/2);
    else                  num = rand()%31 + 3;
    if((clock() - start_time)/1000 >= creat_time && tank::get_tanknum() <= 3)
    {
        //只能更改位置一次
        int flag = 0;
        p = tank_alist->head;
        while(p)
        {
            if(num - *(p->get_tankPos()+1) > -tank_width && num - *(p->get_tankPos()+1) <= 0)     
            {
                num = num - (tank_width - (*(p->get_tankPos()+1) - num));
                flag++;
            }
            else if(num - *(p->get_tankPos()+1) < tank_width && num - *(p->get_tankPos()+1) >= 0) 
            {
                num = num + (tank_width - (num - *(p->get_tankPos()+1)));
                flag++;
            }
            p = p->next;
        }
        if((num >=1 || num <= 34) && flag !=2)  tank_alist->add_tank("enemy_tank",1,num);
        start_time = clock(); 
    }
    tank *temp;
    p = tank_alist->head;
    int j=0,k=0;
    //遍历每一辆敌方坦克
    while(p)
    {
        temp = p;
        if(p == mytank)
        {
            p = temp->next;
            continue;
        }
        name = p->get_name();
        pos[0] = *p->get_tankPos();
        pos[1] = *(p->get_tankPos() + 1);
        dir = p->get_dir();
        //坦克动作间隔为500ms
        if(clock() - p->get_acttime() >= 500) 
        {
            enmeyTank_system(p);
            //记录上一次活动时间
            p->set_acttime(clock());
        }
        p = temp->next;
        j++;
    }
    gotoxy(map_right_limite+8,10);
    cout<<"敌方坦克数量:"<<tank::get_tanknum()-1<<" ";
}
//子弹的控制
void control_bullet(wall* walls,tank* mytank)
{
    bullet *p = NULL;
    int i=0;
    p = bullet_alist->head;
     bullet *temp;
    //子弹击中其他物体
    while(p)
    {
        temp = p->next; 
        //判断子弹是否到达边界,击中墙壁，击中坦克，击中子弹
        if(border(p) && bullet_collide(p,walls) && bullet_collide(p) && bullet_bullet_collide(p))
        {
            p->move_bullet();
            p = p->next;
        }
        else{
            bullet_alist->del_bullet(p);
        }
        p = temp;
    }
}

void enmeyTank_system(tank* sztank)
{
    int tank_dir,tank_pos[2];
    tank_pos[0] = *sztank->get_tankPos();
    tank_pos[1] = *(sztank->get_tankPos() + 1);
    tank_dir = sztank->get_dir();

    //坦克依一定概率行动
    if(chance(0.5)) tank_dir = 2;       //0.5概率坦克方向朝下
    else if(tank_pos[1] <= map_width*wall_width/2-wall_width-1)   //坦克位于左侧
    {
        if(chance(0.7))  tank_dir = 1;   //坦克方向朝右
    }
    else if(tank_pos[1] >= map_width*wall_width/2-wall_width-1)  //坦克位于右侧
    {
        if(chance(0.7))  tank_dir = 3;   //坦克方向朝左
    }
    else         tank_dir = 0;           //坦克方向朝上
    if(chance(0.5)) sztank->bullet_shoot(20);    //发射子弹
    sztank->move_tank(8,tank_dir);      //移动坦克
}
int chance(float chance)
{
    int event = 0;
    srand(clock());
    if(rand()%100/100.0 <= chance)  event = 1;
    return event;
}
int main()

{
    //键盘状态
    int key_state = 0;
    wait_game();
    HideCursor();
   
    //接入链表
    tank_alist->add_tank("my_tank",init_myTank_xpos,init_myTank_ypos,0);
    tank *my_tank = tank_alist->head;
    wall *walls = new wall[map_length*map_width]();
    tank::set_wall(walls);
    clock_t start_time,end_time,now_time;
    while(true)

    {
        //游戏状态控制
        switch(game_state)
        {
            //游戏等待开始
            case 0:
                //开始与结束
                if(_kbhit() && _getch() == 'p')  game_state = 1;
                break;
            //游戏初始化
            case 1:
                start_time = clock();
                init_game(my_tank,walls);  //游戏初始化
                break;
            //运行游戏
            case 2:
                enter_game(my_tank,walls); 
                if((clock() - start_time)/1000 >= 1)
                {
                    game_time = game_time - 1;
                    start_time = clock();
                    gotoxy(map_right_limite+8,5);
                    cout<<"剩余时间:"<<game_time<<"s"<<" "<<endl;
                    gotoxy(map_right_limite+8,30);
                    cout<<"按p结束游戏";
                }
                break;    
            //游戏结束
            case 3:
                gotoxy(map_right_limite+8,20);
                cout<<"你输了？";
                delete tank_alist;
                delete walls;
                delete bullet_alist;
                exec_game();
                
            case 4:
                gotoxy(map_right_limite+8,20);
                cout<<"你赢了!";
                delete tank_alist;
                delete walls;
                delete bullet_alist;
                exec_game();
               
            default:
                break;
        }
    }
    return 0;
}
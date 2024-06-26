// 全局变量,类和函数的声明和定义

// 防止重定义
#ifndef __1__
#define __1__

// 头文件库
#include <cmath>
#include<iostream>
#include<fstream>
#include <easyx.h>
#include <stdio.h>
#include <time.h>
#include <conio.h>
#pragma comment( lib, "MSIMG32.LIB")

using namespace std;

#define TURRET_COUNT 9
#define WID 1280
#define HEI 720
#define TURRET_WIDTH 75
#define TURRET_HEIGHT 60
#define HOME_WIDTH 75
#define HOME_HEIGHT 46
#define SHELL_SIZE 11
#define G 9.8
#define PI 3.1415

//-------------------------------------------------------------------------------------------------------------

/*全局变量的定义*/
ExMessage Mouse_msg = { 0 };//定义鼠标消息结构体变量
IMAGE bombimg[4];         // 定义爆炸图片变量
IMAGE sbombimg[4];
IMAGE hpimg[4];           // 定义血量图片变量
IMAGE backgroundimg;      // 定义背景图片变量
IMAGE img_turret_attack;   // 定义进攻方炮塔图片变量
IMAGE img_shell_attack;    // 定义进攻方炮弹图片变量
IMAGE img_turret_defend;   // 定义防守方炮塔图片变量
IMAGE img_home;            // 定义防守方大本营图片变量
IMAGE img_shell_defend;    // 定义防守方炮弹图片变量
IMAGE img_button[2];

int winner=0;
int hit[TURRET_COUNT *2+1] = { 0 };     // 定义碰撞变量
int hit_idx[TURRET_COUNT *2+1] = { 0 }; // 定义爆炸索引变量
int hit_s[TURRET_COUNT] = { 0 };
int hit_idx_s[TURRET_COUNT] = { 0 };

double unit_time = 300.0;

//-------------------------------------------------------------------------------------------------------------
/*类的定义*/

//爆炸坐标
class xy
{
public:
    xy() { x = 0; y = 0; }
    int x;
    int y;
};

// 定义炮塔碰撞箱变量
class Hitbox
{
public:
    int x;
    int y;
    int w;
    int h;
};

// 定义进攻方炮弹结构体变量
class Shell_attack
{
public:
    int x0;    // 炮弹初始横坐标
    int y0;    // 炮弹初始纵坐标
    int x;     // 炮弹横坐标
    int y;     // 炮弹纵坐标
    double t0; // 炮弹发射初始时间
    double t;// 炮弹发射后时间
    

    void location(); // 更新炮弹位置

    void checkhit(); // 碰撞检测

    Shell_attack(int, int, double, double, int);

private:
    double speed;   // 炮弹初速度
    double degrees; // 炮弹角度
    int idx;        // 定义索引
};

// 定义进攻方炮塔结构体变量
class Turret_attack
{
public:
    int x;           // 炮塔横坐标
    int y;           // 炮塔纵坐标
    int ball_count;  // 炮塔的炮弹数量
    int state;      // 定义炮塔状态
    int idx;

    // 定义碰撞箱
    Hitbox Attack_Hitbox;

    void draw_Ta();  // 炮塔生成

    void fire(int);  // 炮塔发射炮弹

    void strategy(); // 进攻策略(计算初速度和角度)

private:
    double speed;   // 炮弹初速度
    double degrees; // 炮弹发射角度
    
};

// 定义防御方炮弹结构体变量
class Shell_defend
{
public:
    int x0;          // 炮弹初始横坐标
    int y0;          // 炮弹初始纵坐标
    int x;           // 炮弹横坐标
    int y;           // 炮弹纵坐标
    double t0; // 炮弹发射初始时间
    double t;// 炮弹发射后时间
    
    void location(); // 炮弹位置

    void checkhit();//碰撞检测

    Shell_defend(int, int, double, double, int);

private:
    double speed;   // 炮弹初速度
    double degrees; // 炮弹角度
    int idx;
};

// 定义防御方炮塔结构体变量
class Turret_defend
{
public:
    int x;          // 炮塔横坐标
    int y;          // 炮塔纵坐标
    int ball_count; // 炮塔的炮弹数量
    int state;      // 定义炮塔状态
    int idx;

    double A_vx, A_vy;
    double A_x[2], A_y[2], A_t[2];
    
    double A_vy0;
    double A_tx;
    double ty;
    double xv, yv;
    int  flag1, flag2;

    Turret_defend()
    {
        flag1 = 0; flag2 = 0;
    }

    // 定义碰撞箱
    Hitbox Defend_Hitbox;

    void draw_Td();  // 炮塔生成

    void fire(int );     // 炮塔发射炮弹

    bool point1();//判断是否得到x2的坐标
    bool point2();//判断是否得到x2的坐标

    void CHECK_speed(); // 防御策略(计算初速度和角度)



private:
    double speed;   // 炮弹初速度
    double degrees; // 炮弹发射角度
};

// 定义大本营结构体变量
class home
{
public:
    int x;
    int y;
    int state;

    void draw();


};

home H;                                //定义防御方大本营变量
Turret_attack A[TURRET_COUNT];              // 定义进攻方炮塔数组变量
Turret_defend D[TURRET_COUNT];              // 定义防御方炮塔数组变量
Shell_attack* p1[TURRET_COUNT] = { nullptr }; // 进攻方炮弹
Shell_defend* p2[TURRET_COUNT] = { nullptr }; // 防御方炮弹
xy p0[TURRET_COUNT];

//-------------------------------------------------------------------------------------------------------------

/*普通函数的声明*/

void putimage_tp(IMAGE*, int, int,IMAGE*); // 透明图片函数
void init_turret();                  // 炮塔初始化
void init_loadimage();               // 加载图片
void draw_turret();                  // 绘制炮塔
void check_boom();                   // 爆炸检测与绘制
void init_shell();
void check_shell_boom();
int end();
bool Next();
bool inArea(int, int, int, int, int, int);
bool button(int, int, int, int, const char*);
void createfile();

//-------------------------------------------------------------------------------------------------------------

/* 类中成员函数的定义 */

// (1) Turret_attack类中成员函数的定义

// Turret_attack成员函数：进攻方发射炮弹函数(同时修改初速度和角度)
void Turret_attack::fire(int idx)
{
    p1[idx] = new Shell_attack(x - 7, y + 3, speed, degrees, idx);
    A[idx].ball_count--;
}

// Turret_attack成员函数：进攻方炮塔绘制
void Turret_attack::draw_Ta()
{
    putimage_tp(NULL, x, y, &img_turret_attack);
    putimage_tp(NULL, x + 20, y, &hpimg[state - 1]);
    TCHAR s[5], a[5];
    _stprintf_s(s, _T("%d"), state);
    _stprintf_s(a, _T("%d"), ball_count);
    settextcolor(BLUE);
    settextstyle(20, 0, _T("黑体"));
    outtextxy(x + 20, y + 3, s);
    outtextxy(x + 20, y + 17, a);
}

// Turret_attack成员函数：进攻策略(计算初速度和角度)
void Turret_attack::strategy() {
    double min_time = 1000; // 初始最小时间设置为一个较大值
    double optimal_speed = 0; // 最优速度
    double optimal_angle = 0; // 最优角度

    int enemy_count = 0; // 统计未被摧毁的敌人数量
    for (int i = 0; i < TURRET_COUNT; i++) {
        if (D[i].state == 0)
            enemy_count++;
    }

    // 如果剩余敌人较少，则改变进攻目标为家
    if (enemy_count <= 5) {
        for (double angle = 0; angle < PI / 2; angle += PI / 180) {
            double initial_speed = sqrt(-((G * (H.x + HOME_WIDTH / 2 - (x - 10)) * (H.x + HOME_WIDTH / 2 - (x - 10))) / (2 * cos(angle) * cos(angle) * (tan(angle) * (H.x + HOME_WIDTH / 2 - (x - 10)) - (H.y + HOME_HEIGHT / 2 - (y + 4))))));

            // 计算到达目标所需的时间
            double time_to_target = -(H.x + HOME_WIDTH / 2 - (x - 10)) / (initial_speed * cos(angle));

            // 如果该时间小于当前最小时间且速度在合理范围内，则更新最小时间和最优速度、角度
            if (time_to_target < min_time && initial_speed < 120) {
                min_time = time_to_target;
                optimal_speed = initial_speed;
                optimal_angle = angle;
            }
        }
    }
    else { // 如果剩余敌人较多，则继续攻击敌人
        for (int i = 0; i < TURRET_COUNT; i++) {
            if (D[i].state == 0) {
                for (double angle = 0; angle < PI / 2; angle += PI / 180) {
                    double initial_speed = sqrt(-((G * (D[i].x + TURRET_WIDTH / 2 - (x - 10)) * (D[i].x + TURRET_WIDTH / 2 - (x - 10))) / (2 * cos(angle) * cos(angle) * (tan(angle) * (D[i].x + TURRET_WIDTH / 2 - (x - 10)) - (D[i].y + TURRET_HEIGHT / 2 - (y + 4))))));

                    // 计算到达目标所需的时间
                    double time_to_target = -(D[i].x + TURRET_WIDTH / 2 - (x - 10)) / (initial_speed * cos(angle));

                    // 如果该时间小于当前最小时间且速度在合理范围内，则更新最小时间和最优速度、角度
                    if (time_to_target < min_time && initial_speed < 120) {
                        min_time = time_to_target;
                        optimal_speed = initial_speed;
                        optimal_angle = angle;
                    }
                }
            }
        }
    }

    // 使用最优速度和角度进行射击
    speed = optimal_speed;
    degrees = optimal_angle;
}



//-------------------------------------------------------------------------------------------------------------

/* (2) Shell_attack类中成员函数的定义 */


// Shell_attack构造函数：炮弹位置，初速度，角度初始化
Shell_attack::Shell_attack(int x1, int y1, double init_speed, double init_angle, int i) :
    x0(x1), y0(y1), t(0), speed(init_speed), degrees(init_angle), idx(i) {
    t0 = GetTickCount();
}

// Shell_attack成员函数：实时位置与碰撞检测
void Shell_attack::location()
{
    x = x0 - speed * t * cos(degrees);                     // 炮弹横坐标
    y = y0 - (speed * t * sin(degrees) - 0.5 * G * t * t); // 炮弹纵位坐标
    putimage_tp(NULL, x, y, &img_shell_attack); // 绘制炮弹位置
}


// Shell_attack成员函数：碰撞检测
void Shell_attack::checkhit()
{
    // 如果炮弹飞离边界，删除炮弹
    if (x < -SHELL_SIZE || x > WID || y > HEI)
    {
        // delete p1;
        p1[idx] = nullptr;
    }

    if (p1[idx] != nullptr)
    {
        //与大本营的碰撞检测
        if (H.state)
            if ((x + SHELL_SIZE / 2) >= H.x && (x + SHELL_SIZE / 2) <= H.x + HOME_WIDTH && (y + SHELL_SIZE / 2) >= H.y && (y + SHELL_SIZE / 2) <= H.y + HOME_HEIGHT)
            {
                // delete p1;
                p1[idx] = nullptr;
                H.state--;
                hit[TURRET_COUNT] = 1;
                hit_idx[TURRET_COUNT] = 0;
            }

        if (p1[idx] != nullptr)
        {
            // 与炮塔的碰撞检测
            for (int i = 0; i < TURRET_COUNT; i++)
            {
                if (D[i].state > 0)
                {
                    if ((x + SHELL_SIZE / 2) >= D[i].Defend_Hitbox.x &&
                        (x + SHELL_SIZE / 2) <= D[i].Defend_Hitbox.x + D[i].Defend_Hitbox.w &&
                        (y + SHELL_SIZE / 2) >= D[i].Defend_Hitbox.y &&
                        (y + SHELL_SIZE / 2) <= D[i].Defend_Hitbox.y + D[i].Defend_Hitbox.h)
                    {
                        // delete p1;
                        p1[idx] = nullptr;
                        D[i].state--;
                        if (D[i].state == 0)
                        {
                            hit[i] = 1;
                            hit_idx[i] = 0;
                            D[i].ball_count = 0;
                        }
                        else
                        {
                            hit[i] = 1;
                            hit_idx[i] = 0;
                        }
                        break;
                    }
                }
            }
        }
    }
}

// (3) Turret_defend类中成员函数的定义

// Turret_defend成员函数：防御方发射炮弹函数(同时修改初速度和角度)
void Turret_defend::fire(int idx)
{
    p2[idx] = new Shell_defend(x + 57, y + 3, speed, degrees, idx);
    D[idx].ball_count--;
}


// Turret_defend成员函数：防御方炮塔绘制
void Turret_defend::draw_Td()
{
    putimage_tp(NULL, x, y, &img_turret_defend);
    putimage_tp(NULL, x + 20, y, &hpimg[state - 1]);
    TCHAR s[5], a[5];
    _stprintf_s(s, _T("%d"), state);
    _stprintf_s(a, _T("%d"), ball_count);
    settextcolor(BLUE);
    settextstyle(20, 0, _T("黑体"));
    outtextxy(x + 20, y + 3, s);
    outtextxy(x + 20, y + 16, a);

}

bool Turret_defend::point1()
{
    if (p1[idx] != nullptr)
        if (p1[idx]->x >= p1[idx]->x0 - 10 && p1[idx]->x <= p1[idx]->x0 && flag1 == 0)
        {
            A_x[0] = p1[idx]->x;
            A_y[0] = p1[idx]->y;
            A_t[0] = GetTickCount();
            flag1 = 1;
        }
    if (flag1 == 1)
        return true;
    else
        return false;
}

bool Turret_defend::point2()
{
    if (p1[idx] != nullptr)
        if (p1[idx]->x >= p1[idx]->x0 - 90 && p1[idx]->x <= p1[idx]->x0 - 80 && flag2 == 0)
        {
            A_x[1] = p1[idx]->x;
            A_y[1] = p1[idx]->y;
            A_t[1] = GetTickCount();
            flag2 = 1;
        }
    if (flag2 == 1)
        return true;
    else
        return false;
}

void Turret_defend::CHECK_speed() {
    flag1 = 0;
    flag2 = 0;

    // 计算攻击方炮弹的水平速度和垂直速度
    double A_tx = (A_t[1] - A_t[0]) / unit_time;
    double A_vx = (A_x[0] - A_x[1]) / A_tx;
    double ty = (p1[idx]->x0 - A_x[1]) / A_vx;
    double A_vy0 = (p1[idx]->y0 + 0.5 * G * ty * ty - A_y[1]) / ty;
    double A_vy = A_vy0 - G * ty;

    // 计算预计的炮弹到达时间
    double interval_t = GetTickCount() - A_t[1];

    // 根据炮弹的预计位置和速度计算防守方的移动速度和角度
    double xv = (A_x[1] - A_vx * interval_t - (x + 57)) / 3 - A_vx;
    double yv = -((A_y[1] - A_vy * interval_t - (y + 3) + 0.5 * G * interval_t * interval_t) / 3 + G * interval_t - A_vy);

    // 计算速度和角度
    speed = sqrt(xv * xv + yv * yv);
    degrees = atan(yv / xv);
}




// (4) Shell_defend类中成员函数的定义


// Shell_defend构造函数：炮弹位置，初速度，角度初始化
Shell_defend::Shell_defend(int x1, int y1, double init_speed, double init_angle, int i) :
    x0(x1), y0(y1), t(0), speed(init_speed), degrees(init_angle), idx(i) {
    t0 = GetTickCount();
};

// Shell_defend成员函数：实时位置更新
void Shell_defend::location()
{
    x = x0 + speed * t * cos(degrees);                     // 炮弹横坐标
    y = y0 - (speed * t * sin(degrees) - 0.5 * G * t * t); // 炮弹纵位坐标
    putimage_tp(NULL, x, y, &img_shell_defend);                  // 刷新炮弹位置
}

// Shell_defend成员函数：碰撞检测
void Shell_defend::checkhit()
{
    // 如果炮弹飞离边界，删除炮弹
    if (x > WID || y > HEI || x < SHELL_SIZE)
    {
        p2[idx] = nullptr;
    }

    if (p2[idx] != nullptr)
    {
        // 与炮塔的碰撞检测
        for (int i = 0; i < TURRET_COUNT; i++)
        {
            if (A[i].state > 0)
            {
                if ((x + SHELL_SIZE / 2) >= A[i].Attack_Hitbox.x &&
                    (x + SHELL_SIZE / 2) <= A[i].Attack_Hitbox.x + A[i].Attack_Hitbox.w &&
                    (y + SHELL_SIZE / 2) >= A[i].Attack_Hitbox.y &&
                    (y + SHELL_SIZE / 2) <= A[i].Attack_Hitbox.y + A[i].Attack_Hitbox.h)
                {
                    // delete p1;
                    p2[idx] = nullptr;
                    A[i].state--;
                    if (A[i].state == 0)
                    {
                        hit[i + TURRET_COUNT] = 1;
                        hit_idx[i + TURRET_COUNT] = 0;
                        A[i].ball_count = 0;
                    }
                    else
                    {
                        hit[i + TURRET_COUNT] = 1;
                        hit_idx[i + TURRET_COUNT] = 0;
                    }
                    break;
                }
            }
        }
    }
}


// (5) home类中成员函数的定义

// home成员函数：绘制大本营
void home::draw()
{
    putimage_tp(NULL, x, y, &img_home);
    putimage_tp(NULL, x + 20, y, &hpimg[state - 1]);
}


//-----------------------------------------------------------------------------------------------------------


/*普通函数的定义*/


// 透明图片函数
void putimage_tp(IMAGE* dstimg, int x, int y, IMAGE* srcimg) // x为载入图片的X坐标，y为Y坐标
{
    HDC dstDC = GetImageHDC(dstimg);
    HDC srcDC = GetImageHDC(srcimg);
    int w = srcimg->getwidth();
    int h = srcimg->getheight();
    BLENDFUNCTION bf = { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };
    AlphaBlend(dstDC, x, y, w, h, srcDC, 0, 0, w, h, bf);
}

// 炮塔初始化
void init_turret()
{
    srand(time(NULL)); // 随机方式

    // 进攻方炮塔初始化
    for (int i = 0; i < TURRET_COUNT; i++)
    {
        int re = 0;
        do
        {
            re = 0;
            A[i].x = WID - rand() % (WID / 5) - TURRET_WIDTH; // 横坐标随机
            A[i].y = rand() % (HEI - TURRET_HEIGHT);           // 纵坐标随机
            A[i].idx = i;
            // 防止生成炮塔重叠
            for (int j = 0; j < i; j++)
            {
                if ((A[i].x > A[j].x - TURRET_WIDTH) && (A[i].x < A[j].x + TURRET_WIDTH) && (A[i].y > A[j].y - TURRET_HEIGHT) && (A[i].y < A[j].y + TURRET_HEIGHT))
                {
                    re = 1;
                }
            }
        } while (re);

        A[i].state = 4;       // 炮塔状态初始化为4格生命值
        A[i].ball_count = 40; // 炮塔炮弹数量初始化为40

        // 碰撞箱初始化
        A[i].Attack_Hitbox.x = A[i].x + 16;
        A[i].Attack_Hitbox.y = A[i].y + 12;
        A[i].Attack_Hitbox.w = 38;
        A[i].Attack_Hitbox.h = 40;
    }


    H.x = rand() % ((WID / 5) - HOME_WIDTH);
    H.y = HEI / 2 + rand() % (HEI / 2 - HOME_HEIGHT);
    H.state = 4;

    // 防御方炮塔初始化
    for (int i = 0; i < TURRET_COUNT; i++)
    {
        int re = 0;
        do
        {
            re = 0;
            D[i].x = rand() % ((WID / 5) - TURRET_WIDTH); // 横坐标随机
            D[i].y = rand() % (HEI - TURRET_HEIGHT);       // 纵坐标随机
            D[i].idx = i;
            // 防止生成炮塔重叠
            if ((D[i].x > H.x - HOME_WIDTH) && (D[i].x < H.x + HOME_WIDTH) && (D[i].y > H.y - HOME_HEIGHT) && (D[i].y < H.y + HOME_HEIGHT))
                re = 1;
            for (int j = 0; j < i; j++)
            {
                if ((D[i].x > D[j].x - TURRET_WIDTH) && (D[i].x < D[j].x + TURRET_WIDTH) && (D[i].y > D[j].y - TURRET_HEIGHT) && (D[i].y < D[j].y + TURRET_HEIGHT))
                {
                    re = 1;
                }
            }
        } while (re);

        D[i].state = 4;       // 炮塔状态初始化为4格生命值
        D[i].ball_count = 60; // 炮塔炮弹数量初始化为60

        // 碰撞箱初始化
        D[i].Defend_Hitbox.x = D[i].x + 16;
        D[i].Defend_Hitbox.y = D[i].y + 12;
        D[i].Defend_Hitbox.w = 38;
        D[i].Defend_Hitbox.h = 40;
    }
}



//初始化炮弹
void init_shell()
{
    for (int i = 0; i < TURRET_COUNT; i++)
    {
        p1[i] = { nullptr }; // 进攻方炮弹
        p2[i] = { nullptr };// 防御方炮弹
    }

}

// 加载图片
void init_loadimage()
{
    loadimage(&backgroundimg, "work_img/background.png", 1280, 720);                // 加载背景图片
    loadimage(&img_turret_attack, "work_img/Turret_attack.png", TURRET_WIDTH, TURRET_HEIGHT); // 加载进攻方炮塔
    loadimage(&img_turret_defend, "work_img/Turret_defend.png", TURRET_WIDTH, TURRET_HEIGHT); // 加载防御方炮塔
    loadimage(&img_shell_attack, "work_img/shell_attack.png", SHELL_SIZE, SHELL_SIZE);   // 加载进攻方炮弹
    loadimage(&img_shell_defend, "work_img/shell_defend.png", SHELL_SIZE, SHELL_SIZE);   // 加载防御方炮弹
    loadimage(&img_home, "work_img/Home.png", HOME_WIDTH, HOME_HEIGHT);                   // 加载防御方大本营
    //按钮   
    loadimage(img_button + 0, "work_img/button.png");
    loadimage(img_button + 1, "work_img/buttonc.png");
    // 加载爆炸图片
    for (int i = 0; i < 4; i++)
    {
        char str1[20], str2[20],str3[20];
        sprintf_s(str1, "work_img/bomb_%d.png", i);
        sprintf_s(str2, "work_img/HP%d.png", i + 1);
        sprintf_s(str3, "work_img/bomb_%d.png", i);
        loadimage(&bombimg[i], str1, 75, 75);
        loadimage(&hpimg[i], str2, 31, 6);
        loadimage(&sbombimg[i], str3, 40, 40);
    }
}

void check_boom()
{
    for (int i = 0; i < TURRET_COUNT * 2 + 1; i++)
    {
        if (hit[i])
        {
            if (i < TURRET_COUNT)
                putimage_tp(NULL, D[i].x, D[i].y, &bombimg[hit_idx[i] / 50]);
            else if (i == TURRET_COUNT)
                putimage_tp(NULL, H.x, H.y, &bombimg[hit_idx[i] / 50]);
            else
                putimage_tp(NULL, A[i - TURRET_COUNT - 1].x, A[i - TURRET_COUNT - 1].y, &bombimg[hit_idx[i] / 50]);
            if (hit_idx[i] == 199)
            {
                hit_idx[i] = 0;
                hit[i] = 0;
            }
            else
            {
                hit_idx[i]++;
            }
        }
        if (i < TURRET_COUNT && hit_s[i])
        {
            putimage_tp(NULL, p0[i].x, p0[i].y, &sbombimg[hit_idx_s[i] / 50]);
            if (hit_idx_s[i] == 199)
            {
                hit_idx_s[i] = 0;
                hit_s[i] = 0;
            }
            else
            {
                hit_idx_s[i]++;
            }
        }
    }
}

// 绘制炮塔及其血量
void draw_turret()
{
    for (int i = 0; i < TURRET_COUNT; i++)
    {
        if (A[i].state)
            A[i].draw_Ta();
        if (D[i].state)
            D[i].draw_Td();
    }
    if (H.state)
        H.draw();
}

void check_shell_boom()//检测双方炮弹的碰撞
{
    // 双方炮弹的碰撞检测
    for (int i = 0; i < TURRET_COUNT; i++)
    {
        if (p2[i] != nullptr)
        {
            for (int k = 0; k < TURRET_COUNT; k++)
            {
                if (p1[k] != nullptr)
                {
                    if ((p1[k]->x - p2[i]->x) * (p1[k]->x - p2[i]->x) + (p1[k]->y - p2[i]->y) * (p1[k]->y - p2[i]->y) < SHELL_SIZE * SHELL_SIZE)
                    {
                        // delete p2,p1;
                        hit_s[i] = 1; // 修改为hit_s[i]
                        hit_idx_s[i] = 0; // 修改为hit_idx_s[i]
                        p0[i].x = p1[k]->x; // 修改为p0[i]
                        p0[i].y = p1[k]->y; // 修改为p0[i]
                        p2[i] = nullptr;
                        p1[k] = nullptr;
                        break;
                    }
                }
            }
        }
    }
}


//检测鼠标是否在区域内
bool inArea(int mx, int my, int x, int y, int w, int h)
{
    if (mx >= x && mx <= x + w && my >= y && my <= y + h)
    {
        return true;
    }
    else return false;
}

//按钮打印与状态检测
bool button(int x, int y, int w, int h, const char* str)
{
    //检测鼠标是否在按钮上
    if (inArea(Mouse_msg.x, Mouse_msg.y, x, y, w, h))
    {
        putimage_tp(NULL, x, y, img_button + 1);
    }
    else
    {
        putimage_tp(NULL, x, y, img_button + 0);
    }

    outtextxy(x + (w - textwidth(str)) / 2, y + (h - textheight(str)) / 2, str);
    //检测按钮是否被点击
    if (Mouse_msg.message == WM_LBUTTONDOWN && inArea(Mouse_msg.x, Mouse_msg.y, x, y, w, h))
    {
        return true; //点击返回true
    }
    else return false; //未点击返回false
}

//输赢判断
int end()
{
    if (H.state == 0)
        return 1;
    for (int i = 0; i < TURRET_COUNT; i++)
        if (A[i].ball_count > 0)
            return 0;
    return -1;
}

//开始界面
bool start()
{
    BeginBatchDraw();

    settextcolor(WHITE);
    while (1)
    {
        //获取鼠标消息
        peekmessage(&Mouse_msg, EX_MOUSE);
        putimage(0, 0, &backgroundimg);
        settextstyle(100, 0, "行书");
        outtextxy((WID - textwidth("钢铁苍穹")) / 2,(HEI - textheight("钢铁苍穹")) / 2 - 180, "钢铁苍穹");
        settextstyle(35, 0, "行书");
        //玩家点击Start，开始/继续游戏
        if (button((WID - 183) / 2, (HEI - 48) / 2, 183, 48, "开始"))
        {
            Mouse_msg.x = 0;
            Mouse_msg.y = 0;
            Mouse_msg.message = 0;
            return true;
        }
        //玩家点击QUIT，结束程序
        if (button((WID - 183) / 2, (HEI - 48) / 2 + 60, 183, 48, "退出"))
        {
            return false;
        }
        FlushBatchDraw();

        //重置消息为0
        Mouse_msg.message = 0;
    }
    EndBatchDraw();
}

//关卡间界面
bool Next()
{
    BeginBatchDraw();
    settextstyle(35, 0, "隶书");
    settextcolor(WHITE);

    if (end() > 0)
    {
        winner = 1;
    }
    else
    {
        winner = 0;
    }
    createfile();

    while (1)
    {
        //获取鼠标消息
        peekmessage(&Mouse_msg, EX_MOUSE);
        putimage_tp(NULL, 0, 0, &backgroundimg);
        if (end() > 0)
        {
            outtextxy((WID - textwidth("进攻方胜")) / 2, (HEI - textheight("进攻方胜")) / 2 - 60, "进攻方胜");
        }
        else
        {
            outtextxy((WID - textwidth("防御方胜")) / 2, (HEI - textheight("防御方胜")) / 2 - 60, "防御方胜");
        }
        //玩家点击Next level,跳到下一关
        if (button((WID - 183) / 2, (HEI - 48) / 2, 183, 48, "再来一次"))
        {
            //重置鼠标消息
            Mouse_msg.x = 0;
            Mouse_msg.y = 0;
            Mouse_msg.message = 0;
            return true;
        }
        //玩家点击ＭAIN MENU，回到主菜单
        if (button((WID - 183) / 2, (HEI - 48) / 2 + 60, 183, 48, "返回"))
        {
            //重置鼠标消息
            Mouse_msg.x = 0;
            Mouse_msg.y = 0;
            Mouse_msg.message = 0;
            return false;
        }

        FlushBatchDraw();

        //重置消息为0
        Mouse_msg.message = 0;
    }
    EndBatchDraw();
}

//文件操作
void createfile()
{
    static int n1 = 0;
    static int n2 = 0;
    ofstream a;
    if (winner)
    {
        n1++;
    }
    else
    {
        n2++;
    }
    a.open("record.txt", ios::out);
    a << "队伍\t" << "得分\n";
    a << "甲队:\t" << n1<<endl;
    a << "乙队:\t" << n2<<endl;
    a.close();
}

#endif
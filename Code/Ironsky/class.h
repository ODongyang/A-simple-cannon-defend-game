// ȫ�ֱ���,��ͺ����������Ͷ���

// ��ֹ�ض���
#ifndef __1__
#define __1__

// ͷ�ļ���
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

/*ȫ�ֱ����Ķ���*/
ExMessage Mouse_msg = { 0 };//���������Ϣ�ṹ�����
IMAGE bombimg[4];         // ���屬ըͼƬ����
IMAGE sbombimg[4];
IMAGE hpimg[4];           // ����Ѫ��ͼƬ����
IMAGE backgroundimg;      // ���屳��ͼƬ����
IMAGE img_turret_attack;   // �������������ͼƬ����
IMAGE img_shell_attack;    // ����������ڵ�ͼƬ����
IMAGE img_turret_defend;   // ������ط�����ͼƬ����
IMAGE img_home;            // ������ط���ӪͼƬ����
IMAGE img_shell_defend;    // ������ط��ڵ�ͼƬ����
IMAGE img_button[2];

int winner=0;
int hit[TURRET_COUNT *2+1] = { 0 };     // ������ײ����
int hit_idx[TURRET_COUNT *2+1] = { 0 }; // ���屬ը��������
int hit_s[TURRET_COUNT] = { 0 };
int hit_idx_s[TURRET_COUNT] = { 0 };

double unit_time = 300.0;

//-------------------------------------------------------------------------------------------------------------
/*��Ķ���*/

//��ը����
class xy
{
public:
    xy() { x = 0; y = 0; }
    int x;
    int y;
};

// ����������ײ�����
class Hitbox
{
public:
    int x;
    int y;
    int w;
    int h;
};

// ����������ڵ��ṹ�����
class Shell_attack
{
public:
    int x0;    // �ڵ���ʼ������
    int y0;    // �ڵ���ʼ������
    int x;     // �ڵ�������
    int y;     // �ڵ�������
    double t0; // �ڵ������ʼʱ��
    double t;// �ڵ������ʱ��
    

    void location(); // �����ڵ�λ��

    void checkhit(); // ��ײ���

    Shell_attack(int, int, double, double, int);

private:
    double speed;   // �ڵ����ٶ�
    double degrees; // �ڵ��Ƕ�
    int idx;        // ��������
};

// ��������������ṹ�����
class Turret_attack
{
public:
    int x;           // ����������
    int y;           // ����������
    int ball_count;  // �������ڵ�����
    int state;      // ��������״̬
    int idx;

    // ������ײ��
    Hitbox Attack_Hitbox;

    void draw_Ta();  // ��������

    void fire(int);  // ���������ڵ�

    void strategy(); // ��������(������ٶȺͽǶ�)

private:
    double speed;   // �ڵ����ٶ�
    double degrees; // �ڵ�����Ƕ�
    
};

// ����������ڵ��ṹ�����
class Shell_defend
{
public:
    int x0;          // �ڵ���ʼ������
    int y0;          // �ڵ���ʼ������
    int x;           // �ڵ�������
    int y;           // �ڵ�������
    double t0; // �ڵ������ʼʱ��
    double t;// �ڵ������ʱ��
    
    void location(); // �ڵ�λ��

    void checkhit();//��ײ���

    Shell_defend(int, int, double, double, int);

private:
    double speed;   // �ڵ����ٶ�
    double degrees; // �ڵ��Ƕ�
    int idx;
};

// ��������������ṹ�����
class Turret_defend
{
public:
    int x;          // ����������
    int y;          // ����������
    int ball_count; // �������ڵ�����
    int state;      // ��������״̬
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

    // ������ײ��
    Hitbox Defend_Hitbox;

    void draw_Td();  // ��������

    void fire(int );     // ���������ڵ�

    bool point1();//�ж��Ƿ�õ�x2������
    bool point2();//�ж��Ƿ�õ�x2������

    void CHECK_speed(); // ��������(������ٶȺͽǶ�)



private:
    double speed;   // �ڵ����ٶ�
    double degrees; // �ڵ�����Ƕ�
};

// �����Ӫ�ṹ�����
class home
{
public:
    int x;
    int y;
    int state;

    void draw();


};

home H;                                //�����������Ӫ����
Turret_attack A[TURRET_COUNT];              // ��������������������
Turret_defend D[TURRET_COUNT];              // ��������������������
Shell_attack* p1[TURRET_COUNT] = { nullptr }; // �������ڵ�
Shell_defend* p2[TURRET_COUNT] = { nullptr }; // �������ڵ�
xy p0[TURRET_COUNT];

//-------------------------------------------------------------------------------------------------------------

/*��ͨ����������*/

void putimage_tp(IMAGE*, int, int,IMAGE*); // ͸��ͼƬ����
void init_turret();                  // ������ʼ��
void init_loadimage();               // ����ͼƬ
void draw_turret();                  // ��������
void check_boom();                   // ��ը��������
void init_shell();
void check_shell_boom();
int end();
bool Next();
bool inArea(int, int, int, int, int, int);
bool button(int, int, int, int, const char*);
void createfile();

//-------------------------------------------------------------------------------------------------------------

/* ���г�Ա�����Ķ��� */

// (1) Turret_attack���г�Ա�����Ķ���

// Turret_attack��Ա�����������������ڵ�����(ͬʱ�޸ĳ��ٶȺͽǶ�)
void Turret_attack::fire(int idx)
{
    p1[idx] = new Shell_attack(x - 7, y + 3, speed, degrees, idx);
    A[idx].ball_count--;
}

// Turret_attack��Ա��������������������
void Turret_attack::draw_Ta()
{
    putimage_tp(NULL, x, y, &img_turret_attack);
    putimage_tp(NULL, x + 20, y, &hpimg[state - 1]);
    TCHAR s[5], a[5];
    _stprintf_s(s, _T("%d"), state);
    _stprintf_s(a, _T("%d"), ball_count);
    settextcolor(BLUE);
    settextstyle(20, 0, _T("����"));
    outtextxy(x + 20, y + 3, s);
    outtextxy(x + 20, y + 17, a);
}

// Turret_attack��Ա��������������(������ٶȺͽǶ�)
void Turret_attack::strategy() {
    double min_time = 1000; // ��ʼ��Сʱ������Ϊһ���ϴ�ֵ
    double optimal_speed = 0; // �����ٶ�
    double optimal_angle = 0; // ���ŽǶ�

    int enemy_count = 0; // ͳ��δ���ݻٵĵ�������
    for (int i = 0; i < TURRET_COUNT; i++) {
        if (D[i].state == 0)
            enemy_count++;
    }

    // ���ʣ����˽��٣���ı����Ŀ��Ϊ��
    if (enemy_count <= 5) {
        for (double angle = 0; angle < PI / 2; angle += PI / 180) {
            double initial_speed = sqrt(-((G * (H.x + HOME_WIDTH / 2 - (x - 10)) * (H.x + HOME_WIDTH / 2 - (x - 10))) / (2 * cos(angle) * cos(angle) * (tan(angle) * (H.x + HOME_WIDTH / 2 - (x - 10)) - (H.y + HOME_HEIGHT / 2 - (y + 4))))));

            // ���㵽��Ŀ�������ʱ��
            double time_to_target = -(H.x + HOME_WIDTH / 2 - (x - 10)) / (initial_speed * cos(angle));

            // �����ʱ��С�ڵ�ǰ��Сʱ�����ٶ��ں���Χ�ڣ��������Сʱ��������ٶȡ��Ƕ�
            if (time_to_target < min_time && initial_speed < 120) {
                min_time = time_to_target;
                optimal_speed = initial_speed;
                optimal_angle = angle;
            }
        }
    }
    else { // ���ʣ����˽϶࣬�������������
        for (int i = 0; i < TURRET_COUNT; i++) {
            if (D[i].state == 0) {
                for (double angle = 0; angle < PI / 2; angle += PI / 180) {
                    double initial_speed = sqrt(-((G * (D[i].x + TURRET_WIDTH / 2 - (x - 10)) * (D[i].x + TURRET_WIDTH / 2 - (x - 10))) / (2 * cos(angle) * cos(angle) * (tan(angle) * (D[i].x + TURRET_WIDTH / 2 - (x - 10)) - (D[i].y + TURRET_HEIGHT / 2 - (y + 4))))));

                    // ���㵽��Ŀ�������ʱ��
                    double time_to_target = -(D[i].x + TURRET_WIDTH / 2 - (x - 10)) / (initial_speed * cos(angle));

                    // �����ʱ��С�ڵ�ǰ��Сʱ�����ٶ��ں���Χ�ڣ��������Сʱ��������ٶȡ��Ƕ�
                    if (time_to_target < min_time && initial_speed < 120) {
                        min_time = time_to_target;
                        optimal_speed = initial_speed;
                        optimal_angle = angle;
                    }
                }
            }
        }
    }

    // ʹ�������ٶȺͽǶȽ������
    speed = optimal_speed;
    degrees = optimal_angle;
}



//-------------------------------------------------------------------------------------------------------------

/* (2) Shell_attack���г�Ա�����Ķ��� */


// Shell_attack���캯�����ڵ�λ�ã����ٶȣ��Ƕȳ�ʼ��
Shell_attack::Shell_attack(int x1, int y1, double init_speed, double init_angle, int i) :
    x0(x1), y0(y1), t(0), speed(init_speed), degrees(init_angle), idx(i) {
    t0 = GetTickCount();
}

// Shell_attack��Ա������ʵʱλ������ײ���
void Shell_attack::location()
{
    x = x0 - speed * t * cos(degrees);                     // �ڵ�������
    y = y0 - (speed * t * sin(degrees) - 0.5 * G * t * t); // �ڵ���λ����
    putimage_tp(NULL, x, y, &img_shell_attack); // �����ڵ�λ��
}


// Shell_attack��Ա��������ײ���
void Shell_attack::checkhit()
{
    // ����ڵ�����߽磬ɾ���ڵ�
    if (x < -SHELL_SIZE || x > WID || y > HEI)
    {
        // delete p1;
        p1[idx] = nullptr;
    }

    if (p1[idx] != nullptr)
    {
        //���Ӫ����ײ���
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
            // ����������ײ���
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

// (3) Turret_defend���г�Ա�����Ķ���

// Turret_defend��Ա�����������������ڵ�����(ͬʱ�޸ĳ��ٶȺͽǶ�)
void Turret_defend::fire(int idx)
{
    p2[idx] = new Shell_defend(x + 57, y + 3, speed, degrees, idx);
    D[idx].ball_count--;
}


// Turret_defend��Ա��������������������
void Turret_defend::draw_Td()
{
    putimage_tp(NULL, x, y, &img_turret_defend);
    putimage_tp(NULL, x + 20, y, &hpimg[state - 1]);
    TCHAR s[5], a[5];
    _stprintf_s(s, _T("%d"), state);
    _stprintf_s(a, _T("%d"), ball_count);
    settextcolor(BLUE);
    settextstyle(20, 0, _T("����"));
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

    // ���㹥�����ڵ���ˮƽ�ٶȺʹ�ֱ�ٶ�
    double A_tx = (A_t[1] - A_t[0]) / unit_time;
    double A_vx = (A_x[0] - A_x[1]) / A_tx;
    double ty = (p1[idx]->x0 - A_x[1]) / A_vx;
    double A_vy0 = (p1[idx]->y0 + 0.5 * G * ty * ty - A_y[1]) / ty;
    double A_vy = A_vy0 - G * ty;

    // ����Ԥ�Ƶ��ڵ�����ʱ��
    double interval_t = GetTickCount() - A_t[1];

    // �����ڵ���Ԥ��λ�ú��ٶȼ�����ط����ƶ��ٶȺͽǶ�
    double xv = (A_x[1] - A_vx * interval_t - (x + 57)) / 3 - A_vx;
    double yv = -((A_y[1] - A_vy * interval_t - (y + 3) + 0.5 * G * interval_t * interval_t) / 3 + G * interval_t - A_vy);

    // �����ٶȺͽǶ�
    speed = sqrt(xv * xv + yv * yv);
    degrees = atan(yv / xv);
}




// (4) Shell_defend���г�Ա�����Ķ���


// Shell_defend���캯�����ڵ�λ�ã����ٶȣ��Ƕȳ�ʼ��
Shell_defend::Shell_defend(int x1, int y1, double init_speed, double init_angle, int i) :
    x0(x1), y0(y1), t(0), speed(init_speed), degrees(init_angle), idx(i) {
    t0 = GetTickCount();
};

// Shell_defend��Ա������ʵʱλ�ø���
void Shell_defend::location()
{
    x = x0 + speed * t * cos(degrees);                     // �ڵ�������
    y = y0 - (speed * t * sin(degrees) - 0.5 * G * t * t); // �ڵ���λ����
    putimage_tp(NULL, x, y, &img_shell_defend);                  // ˢ���ڵ�λ��
}

// Shell_defend��Ա��������ײ���
void Shell_defend::checkhit()
{
    // ����ڵ�����߽磬ɾ���ڵ�
    if (x > WID || y > HEI || x < SHELL_SIZE)
    {
        p2[idx] = nullptr;
    }

    if (p2[idx] != nullptr)
    {
        // ����������ײ���
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


// (5) home���г�Ա�����Ķ���

// home��Ա���������ƴ�Ӫ
void home::draw()
{
    putimage_tp(NULL, x, y, &img_home);
    putimage_tp(NULL, x + 20, y, &hpimg[state - 1]);
}


//-----------------------------------------------------------------------------------------------------------


/*��ͨ�����Ķ���*/


// ͸��ͼƬ����
void putimage_tp(IMAGE* dstimg, int x, int y, IMAGE* srcimg) // xΪ����ͼƬ��X���꣬yΪY����
{
    HDC dstDC = GetImageHDC(dstimg);
    HDC srcDC = GetImageHDC(srcimg);
    int w = srcimg->getwidth();
    int h = srcimg->getheight();
    BLENDFUNCTION bf = { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };
    AlphaBlend(dstDC, x, y, w, h, srcDC, 0, 0, w, h, bf);
}

// ������ʼ��
void init_turret()
{
    srand(time(NULL)); // �����ʽ

    // ������������ʼ��
    for (int i = 0; i < TURRET_COUNT; i++)
    {
        int re = 0;
        do
        {
            re = 0;
            A[i].x = WID - rand() % (WID / 5) - TURRET_WIDTH; // ���������
            A[i].y = rand() % (HEI - TURRET_HEIGHT);           // ���������
            A[i].idx = i;
            // ��ֹ���������ص�
            for (int j = 0; j < i; j++)
            {
                if ((A[i].x > A[j].x - TURRET_WIDTH) && (A[i].x < A[j].x + TURRET_WIDTH) && (A[i].y > A[j].y - TURRET_HEIGHT) && (A[i].y < A[j].y + TURRET_HEIGHT))
                {
                    re = 1;
                }
            }
        } while (re);

        A[i].state = 4;       // ����״̬��ʼ��Ϊ4������ֵ
        A[i].ball_count = 40; // �����ڵ�������ʼ��Ϊ40

        // ��ײ���ʼ��
        A[i].Attack_Hitbox.x = A[i].x + 16;
        A[i].Attack_Hitbox.y = A[i].y + 12;
        A[i].Attack_Hitbox.w = 38;
        A[i].Attack_Hitbox.h = 40;
    }


    H.x = rand() % ((WID / 5) - HOME_WIDTH);
    H.y = HEI / 2 + rand() % (HEI / 2 - HOME_HEIGHT);
    H.state = 4;

    // ������������ʼ��
    for (int i = 0; i < TURRET_COUNT; i++)
    {
        int re = 0;
        do
        {
            re = 0;
            D[i].x = rand() % ((WID / 5) - TURRET_WIDTH); // ���������
            D[i].y = rand() % (HEI - TURRET_HEIGHT);       // ���������
            D[i].idx = i;
            // ��ֹ���������ص�
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

        D[i].state = 4;       // ����״̬��ʼ��Ϊ4������ֵ
        D[i].ball_count = 60; // �����ڵ�������ʼ��Ϊ60

        // ��ײ���ʼ��
        D[i].Defend_Hitbox.x = D[i].x + 16;
        D[i].Defend_Hitbox.y = D[i].y + 12;
        D[i].Defend_Hitbox.w = 38;
        D[i].Defend_Hitbox.h = 40;
    }
}



//��ʼ���ڵ�
void init_shell()
{
    for (int i = 0; i < TURRET_COUNT; i++)
    {
        p1[i] = { nullptr }; // �������ڵ�
        p2[i] = { nullptr };// �������ڵ�
    }

}

// ����ͼƬ
void init_loadimage()
{
    loadimage(&backgroundimg, "work_img/background.png", 1280, 720);                // ���ر���ͼƬ
    loadimage(&img_turret_attack, "work_img/Turret_attack.png", TURRET_WIDTH, TURRET_HEIGHT); // ���ؽ���������
    loadimage(&img_turret_defend, "work_img/Turret_defend.png", TURRET_WIDTH, TURRET_HEIGHT); // ���ط���������
    loadimage(&img_shell_attack, "work_img/shell_attack.png", SHELL_SIZE, SHELL_SIZE);   // ���ؽ������ڵ�
    loadimage(&img_shell_defend, "work_img/shell_defend.png", SHELL_SIZE, SHELL_SIZE);   // ���ط������ڵ�
    loadimage(&img_home, "work_img/Home.png", HOME_WIDTH, HOME_HEIGHT);                   // ���ط�������Ӫ
    //��ť   
    loadimage(img_button + 0, "work_img/button.png");
    loadimage(img_button + 1, "work_img/buttonc.png");
    // ���ر�ըͼƬ
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

// ������������Ѫ��
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

void check_shell_boom()//���˫���ڵ�����ײ
{
    // ˫���ڵ�����ײ���
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
                        hit_s[i] = 1; // �޸�Ϊhit_s[i]
                        hit_idx_s[i] = 0; // �޸�Ϊhit_idx_s[i]
                        p0[i].x = p1[k]->x; // �޸�Ϊp0[i]
                        p0[i].y = p1[k]->y; // �޸�Ϊp0[i]
                        p2[i] = nullptr;
                        p1[k] = nullptr;
                        break;
                    }
                }
            }
        }
    }
}


//�������Ƿ���������
bool inArea(int mx, int my, int x, int y, int w, int h)
{
    if (mx >= x && mx <= x + w && my >= y && my <= y + h)
    {
        return true;
    }
    else return false;
}

//��ť��ӡ��״̬���
bool button(int x, int y, int w, int h, const char* str)
{
    //�������Ƿ��ڰ�ť��
    if (inArea(Mouse_msg.x, Mouse_msg.y, x, y, w, h))
    {
        putimage_tp(NULL, x, y, img_button + 1);
    }
    else
    {
        putimage_tp(NULL, x, y, img_button + 0);
    }

    outtextxy(x + (w - textwidth(str)) / 2, y + (h - textheight(str)) / 2, str);
    //��ⰴť�Ƿ񱻵��
    if (Mouse_msg.message == WM_LBUTTONDOWN && inArea(Mouse_msg.x, Mouse_msg.y, x, y, w, h))
    {
        return true; //�������true
    }
    else return false; //δ�������false
}

//��Ӯ�ж�
int end()
{
    if (H.state == 0)
        return 1;
    for (int i = 0; i < TURRET_COUNT; i++)
        if (A[i].ball_count > 0)
            return 0;
    return -1;
}

//��ʼ����
bool start()
{
    BeginBatchDraw();

    settextcolor(WHITE);
    while (1)
    {
        //��ȡ�����Ϣ
        peekmessage(&Mouse_msg, EX_MOUSE);
        putimage(0, 0, &backgroundimg);
        settextstyle(100, 0, "����");
        outtextxy((WID - textwidth("�������")) / 2,(HEI - textheight("�������")) / 2 - 180, "�������");
        settextstyle(35, 0, "����");
        //��ҵ��Start����ʼ/������Ϸ
        if (button((WID - 183) / 2, (HEI - 48) / 2, 183, 48, "��ʼ"))
        {
            Mouse_msg.x = 0;
            Mouse_msg.y = 0;
            Mouse_msg.message = 0;
            return true;
        }
        //��ҵ��QUIT����������
        if (button((WID - 183) / 2, (HEI - 48) / 2 + 60, 183, 48, "�˳�"))
        {
            return false;
        }
        FlushBatchDraw();

        //������ϢΪ0
        Mouse_msg.message = 0;
    }
    EndBatchDraw();
}

//�ؿ������
bool Next()
{
    BeginBatchDraw();
    settextstyle(35, 0, "����");
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
        //��ȡ�����Ϣ
        peekmessage(&Mouse_msg, EX_MOUSE);
        putimage_tp(NULL, 0, 0, &backgroundimg);
        if (end() > 0)
        {
            outtextxy((WID - textwidth("������ʤ")) / 2, (HEI - textheight("������ʤ")) / 2 - 60, "������ʤ");
        }
        else
        {
            outtextxy((WID - textwidth("������ʤ")) / 2, (HEI - textheight("������ʤ")) / 2 - 60, "������ʤ");
        }
        //��ҵ��Next level,������һ��
        if (button((WID - 183) / 2, (HEI - 48) / 2, 183, 48, "����һ��"))
        {
            //���������Ϣ
            Mouse_msg.x = 0;
            Mouse_msg.y = 0;
            Mouse_msg.message = 0;
            return true;
        }
        //��ҵ����AIN MENU���ص����˵�
        if (button((WID - 183) / 2, (HEI - 48) / 2 + 60, 183, 48, "����"))
        {
            //���������Ϣ
            Mouse_msg.x = 0;
            Mouse_msg.y = 0;
            Mouse_msg.message = 0;
            return false;
        }

        FlushBatchDraw();

        //������ϢΪ0
        Mouse_msg.message = 0;
    }
    EndBatchDraw();
}

//�ļ�����
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
    a << "����\t" << "�÷�\n";
    a << "�׶�:\t" << n1<<endl;
    a << "�Ҷ�:\t" << n2<<endl;
    a.close();
}

#endif
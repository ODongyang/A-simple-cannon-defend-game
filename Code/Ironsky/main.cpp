#include "class.h" // 包含全局变量、类和函数库
#include <easyx.h>
#include <time.h>

// 游戏主逻辑函数
int main()
{
    int flag = 1;

    initgraph(WID, HEI);    // 创建一个1280*720的窗口

    setbkmode(TRANSPARENT); // 设置背景模式

    cleardevice();
    init_loadimage();       // 加载图片
    // 开始界面
    if (!start())
    {
        return 0;
    }
    if (0)
    {
    again:
        for (int i = 0; i < TURRET_COUNT + 1; i++)
            hit[i] = 0;
        init_shell();
        if (0)
        {
        menu:
            for (int i = 0; i < TURRET_COUNT + 1; i++)
                hit[i] = 0;
            init_shell();
            if (!start())
            {
                return 0;
            }
        }

    }
    init_turret();          // 炮塔初始化
    // 双缓冲绘图防闪屏
    BeginBatchDraw();
    // 双缓冲绘图防闪屏
    BeginBatchDraw();

    while (1)
    {
        cleardevice();

        putimage(0, 0, &backgroundimg); // 输出背景图片

        line(4 * WID / 5 - TURRET_WIDTH, 0, 4 * WID / 5 - TURRET_WIDTH, HEI);
        line(WID / 5, 0, WID / 5, HEI);
        line(0, HEI / 2, WID / 5, HEI / 2);

        // 绘制炮台
        draw_turret();

        // 炮弹发射(生成炮弹)
        for (int i = 0; i < TURRET_COUNT; i++)
        {
            if (p1[i] == nullptr && A[i].ball_count > 0)
            {
                A[i].strategy();
                A[i].fire(i);
            }

            if (p2[i] == nullptr && D[i].ball_count > 0)
            {
                if (D[i].point1() == true)
                {
                    if (D[i].point2() == true)
                    {
                        D[i].CHECK_speed();
                        D[i].fire(i);
                    }
                }
            }
        }

        // 炮弹位置更新与碰撞检测，同时绘制炮弹
        for (int i = 0; i < TURRET_COUNT; i++)
        {
            DWORD time = GetTickCount();
            if (p1[i] != nullptr)
            {
                p1[i]->t = (time - p1[i]->t0) / unit_time;
                p1[i]->location();
                p1[i]->checkhit();
            }
            // 更新炮弹位置
            if (p2[i] != nullptr)
            {
                p2[i]->t = (time - p2[i]->t0) / unit_time;
                p2[i]->location();                      // 更新炮弹位置
                p2[i]->checkhit();
            }
        }

        check_shell_boom();//检测双方炮弹的碰撞

        check_boom(); // 爆炸检测与绘制


        FlushBatchDraw();
        if (end())
            if (Next())
                goto again;
            else
                goto menu;

    }

    EndBatchDraw();
}

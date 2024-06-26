#include "class.h" // ����ȫ�ֱ�������ͺ�����
#include <easyx.h>
#include <time.h>

// ��Ϸ���߼�����
int main()
{
    int flag = 1;

    initgraph(WID, HEI);    // ����һ��1280*720�Ĵ���

    setbkmode(TRANSPARENT); // ���ñ���ģʽ

    cleardevice();
    init_loadimage();       // ����ͼƬ
    // ��ʼ����
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
    init_turret();          // ������ʼ��
    // ˫�����ͼ������
    BeginBatchDraw();
    // ˫�����ͼ������
    BeginBatchDraw();

    while (1)
    {
        cleardevice();

        putimage(0, 0, &backgroundimg); // �������ͼƬ

        line(4 * WID / 5 - TURRET_WIDTH, 0, 4 * WID / 5 - TURRET_WIDTH, HEI);
        line(WID / 5, 0, WID / 5, HEI);
        line(0, HEI / 2, WID / 5, HEI / 2);

        // ������̨
        draw_turret();

        // �ڵ�����(�����ڵ�)
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

        // �ڵ�λ�ø�������ײ��⣬ͬʱ�����ڵ�
        for (int i = 0; i < TURRET_COUNT; i++)
        {
            DWORD time = GetTickCount();
            if (p1[i] != nullptr)
            {
                p1[i]->t = (time - p1[i]->t0) / unit_time;
                p1[i]->location();
                p1[i]->checkhit();
            }
            // �����ڵ�λ��
            if (p2[i] != nullptr)
            {
                p2[i]->t = (time - p2[i]->t0) / unit_time;
                p2[i]->location();                      // �����ڵ�λ��
                p2[i]->checkhit();
            }
        }

        check_shell_boom();//���˫���ڵ�����ײ

        check_boom(); // ��ը��������


        FlushBatchDraw();
        if (end())
            if (Next())
                goto again;
            else
                goto menu;

    }

    EndBatchDraw();
}

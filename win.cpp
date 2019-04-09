#include "snake.h"

void win(int score) {
	BeginBatchDraw();
	IMAGE pause_a;
	IMAGE win;
	IMAGE back_a;
	IMAGE next_selected;
	IMAGE next_unselected;
	loadimage(&pause_a, _T("IMAGE"), _T("pause_a"));
	loadimage(&win, _T("IMAGE"), _T("win"));
	loadimage(&back_a, _T("IMAGE"), _T("back_a"));
	loadimage(&next_selected, _T("IMAGE"), _T("continue_selected"));
	loadimage(&next_unselected, _T("IMAGE"), _T("continue_unselected"));
	putimage(208, 188, &pause_a, SRCAND);
	putimage(208, 188, &win, SRCPAINT);
	putimage(382, 426, &back_a, SRCAND);
	putimage(382, 426, &next_unselected, SRCPAINT);
	//---------------------------------��������
	LOGFONT winfont;
	gettextstyle(&winfont);                     // ��ȡ��ǰ��������
	winfont.lfHeight = 40;                      // ��������߶�Ϊ 30
	_tcscpy_s(winfont.lfFaceName, _T("��������"));//��������
	winfont.lfQuality = ANTIALIASED_QUALITY;    // �������Ч��Ϊ�����  
	winfont.lfWeight = FW_BOLD;
	settextstyle(&winfont);
	settextcolor(BLACK);
	//---------------------------------
	char s[15];
	sprintf(s, "%d", score);
	outtextxy(400- textwidth(s)/2, 314, s);
	MOUSEMSG mouse;
	bool nextselected = 1;
	double nextmo;
	FlushBatchDraw();
	FlushMouseMsgBuffer();
	Sleep(100);
	while (1) {
		if (MouseHit()) {
			mouse = GetMouseMsg();
			nextmo = ((mouse.x - 407.0)*(mouse.x - 407.0) + (mouse.y - 451.0)*(mouse.y - 451.0)) / 625.0;
			if (nextmo <= 1 && nextselected == 1) {
				putimage(382, 426, &back_a, SRCAND);
				putimage(382, 426, &next_selected, SRCPAINT);
				FlushBatchDraw();
				PlaySound((LPCSTR)"botton", NULL, SND_RESOURCE | SND_ASYNC);
				nextselected = 0;
			}
			else if (nextmo > 1 && nextselected == 0) {
				putimage(382, 426, &back_a, SRCAND);
				putimage(382, 426, &next_unselected, SRCPAINT);
				FlushBatchDraw();
				PlaySound((LPCSTR)"botton", NULL, SND_RESOURCE | SND_ASYNC);
				nextselected = 1;
			}
			else if (nextmo <= 1 && mouse.uMsg == WM_LBUTTONUP) {
				PlaySound((LPCSTR)"bottondown", NULL, SND_RESOURCE | SND_ASYNC);
				EndBatchDraw();
				return;
			}
		}
	}
}
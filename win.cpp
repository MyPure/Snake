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
	//---------------------------------字体设置
	LOGFONT winfont;
	gettextstyle(&winfont);                     // 获取当前字体设置
	winfont.lfHeight = 40;                      // 设置字体高度为 30
	_tcscpy_s(winfont.lfFaceName, _T("方正舒体"));//设置字体
	winfont.lfQuality = ANTIALIASED_QUALITY;    // 设置输出效果为抗锯齿  
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
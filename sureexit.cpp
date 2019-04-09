#include "snake.h"

int sureexit() {
	BeginBatchDraw();
	IMAGE pause_a;
	IMAGE sure_b;
	IMAGE yes_selected;
	IMAGE yes_unselected;
	IMAGE no_selected;
	IMAGE no_unselected;
	loadimage(&pause_a, _T("IMAGE"), _T("pause_a"));
	loadimage(&sure_b, _T("IMAGE"), _T("sure_b"));
	loadimage(&yes_selected, _T("IMAGE"), _T("yes_selected"));
	loadimage(&yes_unselected, _T("IMAGE"), _T("yes_unselected"));
	loadimage(&no_selected, _T("IMAGE"), _T("no_selected"));
	loadimage(&no_unselected, _T("IMAGE"), _T("no_unselected"));
	putimage(208, 188, &pause_a, SRCAND);
	putimage(208, 188, &sure_b, SRCPAINT);
	putimage(290, 290, &yes_unselected);
	putimage(430, 290, &no_unselected);
	MOUSEMSG mouse;
	bool yesselected = 1;
	bool noselected = 1;
	double yesmo;
	double nomo;
	FlushBatchDraw();
	FlushMouseMsgBuffer();
	Sleep(100);
	while (1) {
		if (MouseHit()) {
			mouse = GetMouseMsg();
			yesmo = ((mouse.x - 340.0)*(mouse.x - 340.0) + (mouse.y - 340.0)*(mouse.y - 340.0)) / 2209.0;
			nomo = ((mouse.x - 480.0)*(mouse.x - 480.0) + (mouse.y - 340.0)*(mouse.y - 340.0)) / 2209.0;
			if (yesmo <= 1 && yesselected == 1) {
				putimage(290, 290, &yes_selected);
				FlushBatchDraw();
				PlaySound((LPCSTR)"botton", NULL, SND_RESOURCE | SND_ASYNC);
				yesselected = 0;
			}
			else if (yesmo > 1 && yesselected == 0) {
				putimage(290, 290, &yes_unselected);
				FlushBatchDraw();
				PlaySound((LPCSTR)"botton", NULL, SND_RESOURCE | SND_ASYNC);
				yesselected = 1;
			}
			else if (yesmo <= 1 && mouse.uMsg == WM_LBUTTONUP) {
				PlaySound((LPCSTR)"bottondown", NULL, SND_RESOURCE | SND_ASYNC);
				EndBatchDraw();
				return SUREEXIT;
			}
			else if (nomo <= 1 && noselected == 1) {
				putimage(430, 290, &no_selected);
				FlushBatchDraw();
				PlaySound((LPCSTR)"botton", NULL, SND_RESOURCE | SND_ASYNC);
				noselected = 0;
			}
			else if (nomo > 1 && noselected == 0) {
				putimage(430, 290, &no_unselected);
				FlushBatchDraw();
				PlaySound((LPCSTR)"botton", NULL, SND_RESOURCE | SND_ASYNC);
				noselected = 1;
			}
			else if (nomo <= 1 && mouse.uMsg == WM_LBUTTONUP) {
				PlaySound((LPCSTR)"bottondown", NULL, SND_RESOURCE | SND_ASYNC);
				EndBatchDraw();
				return NOEXIT;
			}
		}
	}
}
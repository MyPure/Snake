#include "snake.h"

int option() {
	BeginBatchDraw();
	IMAGE pause_a;
	IMAGE option;
	IMAGE reset_selected;
	IMAGE reset_unselected;
	IMAGE about_selected;
	IMAGE about_unselected;
	IMAGE back_a;
	IMAGE back_selected;
	IMAGE back_unselected;
	loadimage(&pause_a, _T("IMAGE"), _T("pause_a"));
	loadimage(&option, _T("IMAGE"), _T("option"));
	loadimage(&reset_selected, _T("IMAGE"), _T("reset_selected"));
	loadimage(&reset_unselected, _T("IMAGE"), _T("reset_unselected"));
	loadimage(&about_selected, _T("IMAGE"), _T("about_selected"));
	loadimage(&about_unselected, _T("IMAGE"), _T("about_unselected"));
	loadimage(&back_a, _T("IMAGE"), _T("back_a"));
	loadimage(&back_selected, _T("IMAGE"), _T("back_selected"));
	loadimage(&back_unselected, _T("IMAGE"), _T("back_unselected"));
	putimage(208, 188, &pause_a, SRCAND);
	putimage(208, 188, &option, SRCPAINT);
	putimage(290, 290, &reset_unselected);
	putimage(430, 290, &about_unselected);
	putimage(578, 163, &back_a, SRCAND);
	putimage(578, 163, &back_unselected, SRCPAINT);
	MOUSEMSG mouse;
	bool resetselected = 1;
	bool aboutselected = 1;
	bool backselected = 1;
	double resetmo;
	double aboutmo;
	double backmo;
	FlushBatchDraw();
	FlushMouseMsgBuffer();
	Sleep(100);
	while (1) {
		if (MouseHit()) {
			mouse = GetMouseMsg();
			resetmo = ((mouse.x - 340.0)*(mouse.x - 340.0) + (mouse.y - 326.0)*(mouse.y - 326.0)) / 625.0;
			aboutmo = ((mouse.x - 480.0)*(mouse.x - 480.0) + (mouse.y - 326.0)*(mouse.y - 326.0)) / 625.0;
			backmo = ((mouse.x - 603.0)*(mouse.x - 603.0) + (mouse.y - 188.0)*(mouse.y - 188.0)) / 625.0;
			if (resetmo <= 1 && aboutselected == 1) {
				putimage(290, 290, &reset_selected);
				FlushBatchDraw();
				PlaySound((LPCSTR)"botton", NULL, SND_RESOURCE | SND_ASYNC);
				aboutselected = 0;
			}
			else if (resetmo > 1 && aboutselected == 0) {
				putimage(290, 290, &reset_unselected);
				FlushBatchDraw();
				PlaySound((LPCSTR)"botton", NULL, SND_RESOURCE | SND_ASYNC);
				aboutselected = 1;
			}
			else if (resetmo <= 1 && mouse.uMsg == WM_LBUTTONUP) {
				PlaySound((LPCSTR)"bottondown", NULL, SND_RESOURCE | SND_ASYNC);
				EndBatchDraw();
				return SURERESET;
			}
			else if (aboutmo <= 1 && resetselected == 1) {
				putimage(430, 290, &about_selected);
				FlushBatchDraw();
				PlaySound((LPCSTR)"botton", NULL, SND_RESOURCE | SND_ASYNC);
				resetselected = 0;
			}
			else if (aboutmo > 1 && resetselected == 0) {
				putimage(430, 290, &about_unselected);
				FlushBatchDraw();
				PlaySound((LPCSTR)"botton", NULL, SND_RESOURCE | SND_ASYNC);
				resetselected = 1;
			}
			else if (aboutmo <= 1 && mouse.uMsg == WM_LBUTTONUP) {
				PlaySound((LPCSTR)"bottondown", NULL, SND_RESOURCE | SND_ASYNC);
				EndBatchDraw();
				//about();
			}
			else if (backmo <= 1 && backselected == 1) {
				putimage(578, 163, &back_a, SRCAND);
				putimage(578, 163, &back_selected, SRCPAINT);
				FlushBatchDraw();
				PlaySound((LPCSTR)"botton", NULL, SND_RESOURCE | SND_ASYNC);
				backselected = 0;
			}
			else if (backmo > 1 && backselected == 0) {
				putimage(578, 163, &back_a, SRCAND);
				putimage(578, 163, &back_unselected, SRCPAINT);
				FlushBatchDraw();
				PlaySound((LPCSTR)"botton", NULL, SND_RESOURCE | SND_ASYNC);
				backselected = 1;
			}
			else if (backmo <= 1 && mouse.uMsg == WM_LBUTTONUP) {
				PlaySound((LPCSTR)"bottondown", NULL, SND_RESOURCE | SND_ASYNC);
				EndBatchDraw();
				return BACK;
			}
		}
	}
}

int surereset() {
	BeginBatchDraw();
	IMAGE pause_a;
	IMAGE surereset;
	IMAGE yes_selected;
	IMAGE yes_unselected;
	IMAGE no_selected;
	IMAGE no_unselected;
	loadimage(&pause_a, _T("IMAGE"), _T("pause_a"));
	loadimage(&surereset, _T("IMAGE"), _T("surereset"));
	loadimage(&yes_selected, _T("IMAGE"), _T("yes_selected"));
	loadimage(&yes_unselected, _T("IMAGE"), _T("yes_unselected"));
	loadimage(&no_selected, _T("IMAGE"), _T("no_selected"));
	loadimage(&no_unselected, _T("IMAGE"), _T("no_unselected"));
	putimage(208, 188, &pause_a, SRCAND);
	putimage(208, 188, &surereset, SRCPAINT);
	FlushBatchDraw();
	Sleep(1000);//!
	putimage(430, 290, &yes_unselected);
	putimage(290, 290, &no_unselected);
	MOUSEMSG mouse;
	bool yesselected = 1;
	bool noselected = 1;
	double yesmo;
	double nomo;
	FlushBatchDraw();
	FlushMouseMsgBuffer();
	while (1) {
		if (MouseHit()) {
			mouse = GetMouseMsg();
			yesmo = ((mouse.x - 480.0)*(mouse.x - 480.0) + (mouse.y - 340.0)*(mouse.y - 340.0)) / 2209.0;
			nomo = ((mouse.x - 340.0)*(mouse.x - 340.0) + (mouse.y - 340.0)*(mouse.y - 340.0)) / 2209.0;
			if (yesmo <= 1 && yesselected == 1) {
				putimage(430, 290, &yes_selected);
				FlushBatchDraw();
				PlaySound((LPCSTR)"botton", NULL, SND_RESOURCE | SND_ASYNC);
				yesselected = 0;
			}
			else if (yesmo > 1 && yesselected == 0) {
				putimage(430, 290, &yes_unselected);
				FlushBatchDraw();
				PlaySound((LPCSTR)"botton", NULL, SND_RESOURCE | SND_ASYNC);
				yesselected = 1;
			}
			else if (yesmo <= 1 && mouse.uMsg == WM_LBUTTONUP) {
				PlaySound((LPCSTR)"bottondown", NULL, SND_RESOURCE | SND_ASYNC);
				//---------------------------------字体设置
				LOGFONT pausefont;
				gettextstyle(&pausefont);                     // 获取当前字体设置
				pausefont.lfHeight = 16;                      // 设置字体高度为 14
				_tcscpy_s(pausefont.lfFaceName, _T("黑体"));//设置字体
				pausefont.lfQuality = ANTIALIASED_QUALITY;    // 设置输出效果为抗锯齿  
				pausefont.lfWeight = FW_MEDIUM;
				settextstyle(&pausefont);
				settextcolor(BLACK);
				//---------------------------------
				outtextxy(249, 394, "资料重置申请已提交");
				outtextxy(249, 414, "请重新启动游戏");
				EndBatchDraw();
				Sleep(2000);
				return RESETGAME;
			}
			else if (nomo <= 1 && noselected == 1) {
				putimage(290, 290, &no_selected);
				FlushBatchDraw();
				PlaySound((LPCSTR)"botton", NULL, SND_RESOURCE | SND_ASYNC);
				noselected = 0;
			}
			else if (nomo > 1 && noselected == 0) {
				putimage(290, 290, &no_unselected);
				FlushBatchDraw();
				PlaySound((LPCSTR)"botton", NULL, SND_RESOURCE | SND_ASYNC);
				noselected = 1;
			}
			else if (nomo <= 1 && mouse.uMsg == WM_LBUTTONUP) {
				PlaySound((LPCSTR)"bottondown", NULL, SND_RESOURCE | SND_ASYNC);
				EndBatchDraw();
				return BACK;
			}
		}
	}
}
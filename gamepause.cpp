#include "snake.h"

int gamepause() {
	PlaySound((LPCSTR)"bottondown", NULL, SND_RESOURCE | SND_ASYNC);
	BeginBatchDraw();
	IMAGE pause_a;
	IMAGE pause_b;
	IMAGE btomenu_selected;
	IMAGE btomenu_unselected;
	IMAGE restartgame_selected;
	IMAGE restartgame_unselected;
	IMAGE back_a;
	IMAGE back_selected;
	IMAGE back_unselected;
	loadimage(&pause_a, _T("IMAGE"), _T("pause_a"));
	loadimage(&pause_b, _T("IMAGE"), _T("pause_b"));
	loadimage(&btomenu_selected, _T("IMAGE"), _T("btomenu_selected"));
	loadimage(&btomenu_unselected, _T("IMAGE"), _T("btomenu_unselected"));
	loadimage(&restartgame_selected, _T("IMAGE"), _T("restartgame_selected"));
	loadimage(&restartgame_unselected, _T("IMAGE"), _T("restartgame_unselected"));
	loadimage(&back_a, _T("IMAGE"), _T("back_a"));
	loadimage(&back_selected, _T("IMAGE"), _T("back_selected"));
	loadimage(&back_unselected, _T("IMAGE"), _T("back_unselected"));
	putimage(208, 188, &pause_a, SRCAND);
	putimage(208, 188, &pause_b, SRCPAINT);
	putimage(290, 290, &btomenu_unselected);
	putimage(430, 290, &restartgame_unselected);
	putimage(382, 426, &back_a, SRCAND);
	putimage(382, 426, &back_unselected, SRCPAINT);
	//---------------------------------字体设置
	LOGFONT pausefont;
	gettextstyle(&pausefont);                     // 获取当前字体设置
	pausefont.lfHeight = 14;                      // 设置字体高度为 14
	_tcscpy_s(pausefont.lfFaceName, _T("黑体"));//设置字体
	pausefont.lfQuality = ANTIALIASED_QUALITY;    // 设置输出效果为抗锯齿  
	pausefont.lfWeight = FW_MEDIUM;
	settextstyle(&pausefont);
	settextcolor(BLACK);
	//---------------------------------
	TCHAR btomenu[] = _T("进度会被保存");
	TCHAR restart[] = _T("不会保存进度");
	MOUSEMSG mouse;
	bool continueselected = 1;
	bool btomenuselected = 1;
	bool backselected = 1;
	double btomo;
	double conmo;
	double backmo;
	FlushBatchDraw();
	FlushMouseMsgBuffer();
	Sleep(100);
	while (1) {
		if (MouseHit()) {
			mouse = GetMouseMsg();
			btomo = ((mouse.x - 340.0)*(mouse.x - 340.0) + (mouse.y - 340.0)*(mouse.y - 340.0)) / 2209.0;
			conmo = ((mouse.x - 480.0)*(mouse.x - 480.0) + (mouse.y - 340.0)*(mouse.y - 340.0)) / 2209.0;
			backmo = ((mouse.x - 407.0)*(mouse.x - 407.0) + (mouse.y - 451.0)*(mouse.y - 451.0)) / 625.0;
			if (btomo <= 1 && btomenuselected == 1) {
				putimage(290, 290, &btomenu_selected);
				outtextxy(295, 390, btomenu);
				FlushBatchDraw();
				PlaySound((LPCSTR)"botton", NULL, SND_RESOURCE | SND_ASYNC);
				btomenuselected = 0;
			}
			else if (btomo > 1 && btomenuselected == 0) {
				putimage(290, 290, &btomenu_unselected);
				FlushBatchDraw();
				PlaySound((LPCSTR)"botton", NULL, SND_RESOURCE | SND_ASYNC);
				btomenuselected = 1;
			}
			else if (btomo <= 1 && mouse.uMsg == WM_LBUTTONUP) {
				PlaySound((LPCSTR)"bottondown", NULL, SND_RESOURCE | SND_ASYNC);
				EndBatchDraw();
				return BACKTOMENU;
			}
			else if (conmo <= 1 && continueselected == 1) {
				putimage(430, 290, &restartgame_selected);
				outtextxy(440, 390, restart);
				FlushBatchDraw();
				PlaySound((LPCSTR)"botton", NULL, SND_RESOURCE | SND_ASYNC);
				continueselected = 0;
			}
			else if (conmo > 1 && continueselected == 0) {
				putimage(430, 290, &restartgame_unselected);
				FlushBatchDraw();
				PlaySound((LPCSTR)"botton", NULL, SND_RESOURCE | SND_ASYNC);
				continueselected = 1;
			}
			else if (conmo <= 1 && mouse.uMsg == WM_LBUTTONUP) {
				PlaySound((LPCSTR)"bottondown", NULL, SND_RESOURCE | SND_ASYNC);
				EndBatchDraw();
				Sleep(1000);
				return RESTARTGAME;
			}
			else if (backmo <= 1 && backselected == 1) {
				putimage(382, 426, &back_a, SRCAND);
				putimage(382, 426, &back_selected, SRCPAINT);
				FlushBatchDraw();
				PlaySound((LPCSTR)"botton", NULL, SND_RESOURCE | SND_ASYNC);
				backselected = 0;
			}
			else if (backmo > 1 && backselected == 0) {
				putimage(382, 426, &back_a, SRCAND);
				putimage(382, 426, &back_unselected, SRCPAINT);
				FlushBatchDraw();
				PlaySound((LPCSTR)"botton", NULL, SND_RESOURCE | SND_ASYNC);
				backselected = 1;
			}
			else if (backmo <= 1 && mouse.uMsg == WM_LBUTTONUP) {
				PlaySound((LPCSTR)"bottondown", NULL, SND_RESOURCE | SND_SYNC);
				EndBatchDraw();
				return CONTINUEGAME;
			}
		}
	}
}
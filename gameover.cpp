#include "snake.h"

int gameover() {
	BeginBatchDraw();
	IMAGE pause_a;
	IMAGE gameover;
	IMAGE btomenu_selected;
	IMAGE btomenu_unselected;
	IMAGE restartgame_selected;
	IMAGE restartgame_unselected;
	loadimage(&pause_a, _T("IMAGE"), _T("pause_a"));
	loadimage(&gameover, _T("IMAGE"), _T("gameover"));
	loadimage(&btomenu_selected, _T("IMAGE"), _T("btomenu_selected"));
	loadimage(&btomenu_unselected, _T("IMAGE"), _T("btomenu_unselected"));
	loadimage(&restartgame_selected, _T("IMAGE"), _T("restartgame_selected"));
	loadimage(&restartgame_unselected, _T("IMAGE"), _T("restartgame_unselected"));
	putimage(208, 188, &pause_a, SRCAND);
	putimage(208, 188, &gameover, SRCPAINT);
	putimage(290, 290, &btomenu_unselected);
	putimage(430, 290, &restartgame_unselected);
	MOUSEMSG mouse;
	bool btomenuselected = 1;
	bool restartgameselected = 1;
	double btomenumo;
	double restartgamemo;
	FlushBatchDraw();
	FlushMouseMsgBuffer();
	while (1) {
		if (MouseHit()) {
			mouse = GetMouseMsg();
			btomenumo = ((mouse.x - 340.0)*(mouse.x - 340.0) + (mouse.y - 340.0)*(mouse.y - 340.0)) / 2209.0;
			restartgamemo = ((mouse.x - 480.0)*(mouse.x - 480.0) + (mouse.y - 340.0)*(mouse.y - 340.0)) / 2209.0;
			if (btomenumo <= 1 && btomenuselected == 1) {
				putimage(290, 290, &btomenu_selected);
				FlushBatchDraw();
				PlaySound((LPCSTR)"botton", NULL, SND_RESOURCE | SND_ASYNC);
				btomenuselected = 0;
			}
			else if (btomenumo > 1 && btomenuselected == 0) {
				putimage(290, 290, &btomenu_unselected);
				FlushBatchDraw();
				PlaySound((LPCSTR)"botton", NULL, SND_RESOURCE | SND_ASYNC);
				btomenuselected = 1;
			}
			else if (btomenumo <= 1 && mouse.uMsg == WM_LBUTTONUP) {
				PlaySound((LPCSTR)"bottondown", NULL, SND_RESOURCE | SND_ASYNC);
				EndBatchDraw();
				return BACKTOMENU;
			}
			else if (restartgamemo <= 1 && restartgameselected == 1) {
				putimage(430, 290, &restartgame_selected);
				FlushBatchDraw();
				PlaySound((LPCSTR)"botton", NULL, SND_RESOURCE | SND_ASYNC);
				restartgameselected = 0;
			}
			else if (restartgamemo > 1 && restartgameselected == 0) {
				putimage(430, 290, &restartgame_unselected);
				FlushBatchDraw();
				PlaySound((LPCSTR)"botton", NULL, SND_RESOURCE | SND_ASYNC);
				restartgameselected = 1;
			}
			else if (restartgamemo <= 1 && mouse.uMsg == WM_LBUTTONUP) {
				PlaySound((LPCSTR)"bottondown", NULL, SND_RESOURCE | SND_ASYNC);
				EndBatchDraw();
				return RESTARTGAME;
			}
		}
	}
}
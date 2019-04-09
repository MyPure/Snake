#include "snake.h"
int chooselevel(int currentlevel,int passlevel,bool tg) {
	BeginBatchDraw();
	IMAGE back_a;
	IMAGE back_selected;
	IMAGE back_unselected;
	IMAGE continue_selected;
	IMAGE continue_unselected;
	IMAGE continue_cantselected;
	IMAGE level1_selected;
	IMAGE level1_unselected;
	IMAGE level2_selected;
	IMAGE level2_unselected; 
	IMAGE level2_cantselected;
	IMAGE level3_selected;
	IMAGE level3_unselected;
	IMAGE level3_cantselected;
	IMAGE level4_selected;
	IMAGE level4_unselected;
	IMAGE level4_cantselected;
	loadimage(&back_a, _T("IMAGE"), _T("back_a"));
	loadimage(&back_selected, _T("IMAGE"), _T("back_selected"));
	loadimage(&back_unselected, _T("IMAGE"), _T("back_unselected"));
	loadimage(&continue_selected, _T("IMAGE"), _T("continue_selected"));
	loadimage(&continue_unselected, _T("IMAGE"), _T("continue_unselected"));
	loadimage(&continue_cantselected, _T("IMAGE"), _T("continue_cantselected"));
	loadimage(&level1_selected, _T("IMAGE"), _T("level1_selected"));
	loadimage(&level1_unselected, _T("IMAGE"), _T("level1_unselected"));
	loadimage(&level2_selected, _T("IMAGE"), _T("level2_selected"));
	loadimage(&level2_unselected, _T("IMAGE"), _T("level2_unselected"));
	loadimage(&level2_cantselected, _T("IMAGE"), _T("level2_cantselected"));
	loadimage(&level3_selected, _T("IMAGE"), _T("level3_selected"));
	loadimage(&level3_unselected, _T("IMAGE"), _T("level3_unselected"));
	loadimage(&level3_cantselected, _T("IMAGE"), _T("level3_cantselected"));
	loadimage(&level4_selected, _T("IMAGE"), _T("level4_selected"));
	loadimage(&level4_unselected, _T("IMAGE"), _T("level4_unselected"));
	loadimage(&level4_cantselected, _T("IMAGE"), _T("level4_cantselected"));
	if (tg == 1) {
		loadimage(NULL, _T("IMAGE"), _T("chooselevelbg"));
	}
	else {
		loadimage(NULL, _T("IMAGE"), _T("chooselevelbg_h"));
	}
	putimage(702, 48, &back_a, SRCAND);
	putimage(702, 48, &back_unselected, SRCPAINT);
	putimage(48, 508, &back_a, SRCAND);
	if (currentlevel == 0) {
		putimage(48, 508, &continue_cantselected, SRCPAINT);
	}
	else {
		putimage(48, 508, &continue_unselected, SRCPAINT);
	}
	if (passlevel >= 1) {
		putimage(247, 156, &level2_unselected);
	}
	else {
		putimage(247, 156, &level2_cantselected);
	}
	if (passlevel >= 2) {
		putimage(424, 34, &level3_unselected);
	}
	else {
		putimage(424, 34, &level3_cantselected);
	}
	if (passlevel >= 3) {
		putimage(601, 156, &level4_unselected);
	}
	else {
		putimage(601, 156, &level4_cantselected);
	}
	putimage(70, 34, &level1_unselected);
	MOUSEMSG mouse;
	bool backselected = 1;
	bool level1selected = 1;
	bool level2selected = 1;
	bool level3selected = 1;
	bool level4selected = 1;
	bool continueselected = 1;
	double backmo;
	double continuemo;
	FlushBatchDraw();
	FlushMouseMsgBuffer();
	Sleep(300);
	while (1) {
		if (MouseHit()) {
			mouse = GetMouseMsg();
			backmo = ((mouse.x - 727.0)*(mouse.x - 727.0) + (mouse.y - 73.0)*(mouse.y - 73.0)) / 625.0;
			continuemo = ((mouse.x - 73.0)*(mouse.x - 73.0) + (mouse.y - 533.0)*(mouse.y - 533.0)) / 625.0;
			if (backmo <= 1 && backselected == 1) {
				putimage(702, 48, &back_a, SRCAND);
				putimage(702, 48, &back_selected, SRCPAINT);
				FlushBatchDraw();
				PlaySound((LPCSTR)"botton", NULL, SND_RESOURCE | SND_ASYNC);
				backselected = 0;
			}
			else if (backmo > 1 && backselected == 0) {
				putimage(702, 48, &back_a, SRCAND);
				putimage(702, 48, &back_unselected, SRCPAINT);
				FlushBatchDraw();
				PlaySound((LPCSTR)"botton", NULL, SND_RESOURCE | SND_ASYNC);
				backselected = 1;
			}
			else if (backmo <= 1 && mouse.uMsg == WM_LBUTTONUP) {
				PlaySound((LPCSTR)"bottondown", NULL, SND_RESOURCE | SND_ASYNC);
				EndBatchDraw();
				return 0;
			}
			else if (mouse.x >= 70 && mouse.x <= 201 && mouse.y >= 34 && mouse.y <= 436 && level1selected == 1) {
				putimage(70, 34, &level1_selected);
				FlushBatchDraw();
				PlaySound((LPCSTR)"botton", NULL, SND_RESOURCE | SND_ASYNC);
				level1selected = 0;
			}
			else if (!(mouse.x >= 70 && mouse.x <= 201 && mouse.y >= 34 && mouse.y <= 436) && level1selected == 0) {
				putimage(70, 34, &level1_unselected);
				FlushBatchDraw();
				PlaySound((LPCSTR)"botton", NULL, SND_RESOURCE | SND_ASYNC);
				level1selected = 1;
			}
			else if (mouse.x >= 70 && mouse.x <= 201 && mouse.y >= 34 && mouse.y <= 436 && mouse.uMsg == WM_LBUTTONUP) {
				PlaySound((LPCSTR)"bottondown", NULL, SND_RESOURCE | SND_SYNC);
				EndBatchDraw();
				return 1;
			}
			if (passlevel >= 1) {
				if (mouse.x >= 247 && mouse.x <= 378 && mouse.y >= 156 && mouse.y <= 558 && level2selected == 1) {
					putimage(247, 156, &level2_selected);
					FlushBatchDraw();
					PlaySound((LPCSTR)"botton", NULL, SND_RESOURCE | SND_ASYNC);
					level2selected = 0;
				}
				else if (!(mouse.x >= 247 && mouse.x <= 378 && mouse.y >= 156 && mouse.y <= 558) && level2selected == 0) {
					putimage(247, 156, &level2_unselected);
					FlushBatchDraw();
					PlaySound((LPCSTR)"botton", NULL, SND_RESOURCE | SND_ASYNC);
					level2selected = 1;
				}
				else if (mouse.x >= 247 && mouse.x <= 378 && mouse.y >= 156 && mouse.y <= 558 && mouse.uMsg == WM_LBUTTONUP) {
					PlaySound((LPCSTR)"bottondown", NULL, SND_RESOURCE | SND_SYNC);
					EndBatchDraw();
					return 2;
				}
			}
			if (passlevel >= 2) {
				if (mouse.x >= 424 && mouse.x <= 555 && mouse.y >= 34 && mouse.y <= 436 && level3selected == 1) {
					putimage(424, 34, &level3_selected);
					FlushBatchDraw();
					PlaySound((LPCSTR)"botton", NULL, SND_RESOURCE | SND_ASYNC);
					level3selected = 0;
				}
				else if (!(mouse.x >= 424 && mouse.x <= 555 && mouse.y >= 34 && mouse.y <= 436) && level3selected == 0) {
					putimage(424, 34, &level3_unselected);
					FlushBatchDraw();
					PlaySound((LPCSTR)"botton", NULL, SND_RESOURCE | SND_ASYNC);
					level3selected = 1;
				}
				else if (mouse.x >= 424 && mouse.x <= 555 && mouse.y >= 34 && mouse.y <= 436 && mouse.uMsg == WM_LBUTTONUP) {
					PlaySound((LPCSTR)"bottondown", NULL, SND_RESOURCE | SND_SYNC);
					EndBatchDraw();
					return 3;
				}
			}
			if (passlevel >= 3) {
				if (mouse.x >= 601 && mouse.x <= 734 && mouse.y >= 156 && mouse.y <= 558 && level4selected == 1) {
					putimage(601, 156, &level4_selected);
					FlushBatchDraw();
					PlaySound((LPCSTR)"botton", NULL, SND_RESOURCE | SND_ASYNC);
					level4selected = 0;
				}
				else if (!(mouse.x >= 601 && mouse.x <= 734 && mouse.y >= 156 && mouse.y <= 558) && level4selected == 0) {
					putimage(601, 156, &level4_unselected);
					FlushBatchDraw();
					PlaySound((LPCSTR)"botton", NULL, SND_RESOURCE | SND_ASYNC);
					level4selected = 1;
				}
				else if (mouse.x >= 601 && mouse.x <= 734 && mouse.y >= 156 && mouse.y <= 558 && mouse.uMsg == WM_LBUTTONUP) {
					PlaySound((LPCSTR)"bottondown", NULL, SND_RESOURCE | SND_SYNC);
					EndBatchDraw();
					return 4;
				}
			}
			if (currentlevel != 0) {
				if (continuemo <= 1 && continueselected == 1) {
					putimage(48, 508, &back_a, SRCAND);
					putimage(48, 508, &continue_selected, SRCPAINT);
					FlushBatchDraw();
					PlaySound((LPCSTR)"botton", NULL, SND_RESOURCE | SND_ASYNC);
					continueselected = 0;
				}
				else if (continuemo > 1 && continueselected == 0) {
					putimage(48, 508, &back_a, SRCAND);
					putimage(48, 508, &continue_unselected, SRCPAINT);
					FlushBatchDraw();
					PlaySound((LPCSTR)"botton", NULL, SND_RESOURCE | SND_ASYNC);
					continueselected = 1;
				}
				else if (continuemo <= 1 && mouse.uMsg == WM_LBUTTONUP) {
					PlaySound((LPCSTR)"bottondown", NULL, SND_RESOURCE | SND_ASYNC);
					EndBatchDraw();
					return 6;
				}
			}
		}
	}
}
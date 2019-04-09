#include "snake.h"
int mainmenuinterface(bool tg,int passlevel) {
	BeginBatchDraw();
	cleardevice();
	if (tg == 1) {
		loadimage(NULL, _T("IMAGE"), _T("menubg_u"));
	}
	else {
		if (passlevel >= 4) {
			loadimage(NULL, _T("IMAGE"), _T("menubg_h_u"));
		}
		else {
			loadimage(NULL, _T("IMAGE"), _T("menubg_h"));
		}
	}
	IMAGE start_selected_a;
	IMAGE start_selected_b;
	IMAGE start_unselected_a;
	IMAGE start_unselected_b;
	IMAGE rank_a;
	IMAGE rank_selected;
	IMAGE rank_unselected;
	IMAGE achievements_selected;
	IMAGE achievements_unselected;
	IMAGE exit_selected;
	IMAGE exit_unselected;
	IMAGE option_selected;
	IMAGE option_unselected;
	loadimage(&start_selected_a, _T("IMAGE"), _T("start_selected_a"));
	loadimage(&start_selected_b, _T("IMAGE"), _T("start_selected_b"));
	loadimage(&start_unselected_a, _T("IMAGE"), _T("start_unselected_a"));
	loadimage(&start_unselected_b, _T("IMAGE"), _T("start_unselected_b"));
	loadimage(&rank_a, _T("IMAGE"), _T("rank_a"));
	loadimage(&rank_selected, _T("IMAGE"), _T("rank_selected"));
	loadimage(&rank_unselected, _T("IMAGE"), _T("rank_unselected"));
	loadimage(&achievements_selected, _T("IMAGE"), _T("achievements_selected"));
	loadimage(&achievements_unselected, _T("IMAGE"), _T("achievements_unselected"));
	loadimage(&exit_selected, _T("IMAGE"), _T("exit_selected"));
	loadimage(&exit_unselected, _T("IMAGE"), _T("exit_unselected"));
	loadimage(&option_selected, _T("IMAGE"), _T("OPTION_SELECTED"));
	loadimage(&option_unselected, _T("IMAGE"), _T("OPTION_UNSELECTED"));
	putimage(318, 400, &start_unselected_a, SRCAND);
	putimage(318, 400, &start_unselected_b, SRCPAINT);
	putimage(240, 417, &rank_a, SRCAND);
	putimage(240, 417, &rank_unselected, SRCPAINT);
	putimage(509, 417, &rank_a, SRCAND);
	putimage(509, 417, &achievements_unselected, SRCPAINT);
	putimage(715, 34, &rank_a, SRCAND);
	putimage(715, 34, &exit_unselected, SRCPAINT);
	putimage(35, 34, &rank_a, SRCAND);
	putimage(35, 34, &option_unselected, SRCPAINT);
	MOUSEMSG mouse;
	double startmo;//鼠标的坐标是否在start椭圆内
	double rankmo;
	double achievementsmo;
	double exitmo;
	double optionmo;
	double bossmo;
	bool startselected = 1;
	bool rankselected = 1;
	bool achievementsselected = 1;
	bool exitselected = 1;
	bool optionselected=1;
	bool bossselected = 1;
	int choose;
	FlushBatchDraw();
	FlushMouseMsgBuffer();
	while (1) {
		if (MouseHit()) {
			mouse = GetMouseMsg();
			startmo = ((mouse.x - 398.0)*(mouse.x - 398.0)) / 6400.0 + ((mouse.y - 443.0)*(mouse.y - 443.0)) / 676.0;
			rankmo = ((mouse.x - 265.0)*(mouse.x - 265.0) + (mouse.y - 442.0)*(mouse.y - 442.0)) / 625.0;
			achievementsmo = ((mouse.x - 534.0)*(mouse.x - 534.0) + (mouse.y - 442.0)*(mouse.y - 442.0)) / 625.0;
			exitmo = ((mouse.x - 740.0)*(mouse.x - 740.0) + (mouse.y - 59.0)*(mouse.y - 59.0)) / 625.0;
			optionmo = ((mouse.x - 60.0)*(mouse.x - 60.0) + (mouse.y - 59.0)*(mouse.y - 59.0)) / 625.0;
			bossmo = ((mouse.x - 396.0)*(mouse.x - 396.0) + (mouse.y - 252.0)*(mouse.y - 252.0)) / 7921.0;
			if (startmo <= 1 && startselected == 1) {
				putimage(318, 400, &start_selected_a, SRCAND);
				putimage(318, 400, &start_selected_b, SRCPAINT);
				FlushBatchDraw();
				PlaySound((LPCSTR)"botton", NULL, SND_RESOURCE | SND_ASYNC);
				startselected = 0;
			}
			else if (startmo > 1 && startselected == 0) {
				putimage(318, 400, &start_unselected_a, SRCAND);
				putimage(318, 400, &start_unselected_b, SRCPAINT);
				FlushBatchDraw();
				PlaySound((LPCSTR)"botton", NULL, SND_RESOURCE | SND_ASYNC);
				startselected = 1;
			}
			else if (startmo <= 1 && mouse.uMsg == WM_LBUTTONUP) {
				PlaySound((LPCSTR)"bottondown", NULL, SND_RESOURCE | SND_ASYNC);
				choose = START;
				break;
			}
			else if (rankmo <= 1 && rankselected == 1) {
				putimage(240, 417, &rank_a, SRCAND);
				putimage(240, 417, &rank_selected, SRCPAINT);
				FlushBatchDraw();
				PlaySound((LPCSTR)"botton", NULL, SND_RESOURCE | SND_ASYNC);
				rankselected = 0;
			}
			else if (rankmo > 1 && rankselected == 0) {
				putimage(240, 417, &rank_a, SRCAND);
				putimage(240, 417, &rank_unselected, SRCPAINT);
				FlushBatchDraw();
				PlaySound((LPCSTR)"botton", NULL, SND_RESOURCE | SND_ASYNC);
				rankselected = 1;
			}
			else if (rankmo <= 1 && mouse.uMsg == WM_LBUTTONUP) {
				PlaySound((LPCSTR)"bottondown", NULL, SND_RESOURCE | SND_ASYNC);
				choose = RANK;
				break;
			}
			else if (achievementsmo <= 1 && achievementsselected == 1) {
				putimage(509, 417, &rank_a, SRCAND);
				putimage(509, 417, &achievements_selected, SRCPAINT);
				FlushBatchDraw();
				PlaySound((LPCSTR)"botton", NULL, SND_RESOURCE | SND_ASYNC);
				achievementsselected = 0;
			}
			else if (achievementsmo > 1 && achievementsselected == 0) {
				putimage(509, 417, &rank_a, SRCAND);
				putimage(509, 417, &achievements_unselected, SRCPAINT);
				FlushBatchDraw();
				PlaySound((LPCSTR)"botton", NULL, SND_RESOURCE | SND_ASYNC);
				achievementsselected = 1;
			}
			else if (achievementsmo <= 1 && mouse.uMsg == WM_LBUTTONUP) {
				PlaySound((LPCSTR)"bottondown", NULL, SND_RESOURCE | SND_ASYNC);
				choose = ACHIEVEMENTS;
				break;
			}
			else if (exitmo <= 1 && exitselected == 1) {
				putimage(715, 34, &rank_a, SRCAND);
				putimage(715, 34, &exit_selected, SRCPAINT);
				FlushBatchDraw();
				PlaySound((LPCSTR)"botton", NULL, SND_RESOURCE | SND_ASYNC);
				exitselected = 0;
			}
			else if (exitmo > 1 && exitselected == 0) {
				putimage(715, 34, &rank_a, SRCAND);
				putimage(715, 34, &exit_unselected, SRCPAINT);
				FlushBatchDraw();
				PlaySound((LPCSTR)"botton", NULL, SND_RESOURCE | SND_ASYNC);
				exitselected = 1;
			}
			else if (exitmo <= 1 && mouse.uMsg == WM_LBUTTONUP) {
				PlaySound((LPCSTR)"bottondown", NULL, SND_RESOURCE | SND_ASYNC);
				choose = EXIT;
				break;
			}
			else if (optionmo <= 1 && optionselected == 1) {
				putimage(35, 34, &rank_a, SRCAND);
				putimage(35, 34, &option_selected, SRCPAINT);
				FlushBatchDraw();
				PlaySound((LPCSTR)"botton", NULL, SND_RESOURCE | SND_ASYNC);
				optionselected = 0;
			}
			else if (optionmo > 1 && optionselected == 0) {
				putimage(35, 34, &rank_a, SRCAND);
				putimage(35, 34, &option_unselected, SRCPAINT);
				FlushBatchDraw();
				PlaySound((LPCSTR)"botton", NULL, SND_RESOURCE | SND_ASYNC);
				optionselected = 1;
			}
			else if (optionmo <= 1 && mouse.uMsg == WM_LBUTTONUP) {
				PlaySound((LPCSTR)"bottondown", NULL, SND_RESOURCE | SND_ASYNC);
				choose = OPTION;
				break;
			}
			if (passlevel >= 4) {
				if (bossmo <= 1 && bossselected == 1) {
					if (tg == 0) {
						loadimage(NULL, _T("IMAGE"), _T("menubg_h_s"));
					}
					else {
						loadimage(NULL, _T("IMAGE"), _T("menubg_s"));
					}
					putimage(318, 400, &start_unselected_a, SRCAND);
					putimage(318, 400, &start_unselected_b, SRCPAINT);
					putimage(240, 417, &rank_a, SRCAND);
					putimage(240, 417, &rank_unselected, SRCPAINT);
					putimage(509, 417, &rank_a, SRCAND);
					putimage(509, 417, &achievements_unselected, SRCPAINT);
					putimage(715, 34, &rank_a, SRCAND);
					putimage(715, 34, &exit_unselected, SRCPAINT);
					putimage(35, 34, &rank_a, SRCAND);
					putimage(35, 34, &option_unselected, SRCPAINT);
					FlushBatchDraw();
					PlaySound((LPCSTR)"botton", NULL, SND_RESOURCE | SND_ASYNC);
					bossselected = 0;
				}
				else if (bossmo > 1 && bossselected == 0) {
					if (tg == 0) {
						loadimage(NULL, _T("IMAGE"), _T("menubg_h_u"));
					}
					else {
						loadimage(NULL, _T("IMAGE"), _T("menubg_u"));
					}
					putimage(318, 400, &start_unselected_a, SRCAND);
					putimage(318, 400, &start_unselected_b, SRCPAINT);
					putimage(240, 417, &rank_a, SRCAND);
					putimage(240, 417, &rank_unselected, SRCPAINT);
					putimage(509, 417, &rank_a, SRCAND);
					putimage(509, 417, &achievements_unselected, SRCPAINT);
					putimage(715, 34, &rank_a, SRCAND);
					putimage(715, 34, &exit_unselected, SRCPAINT);
					putimage(35, 34, &rank_a, SRCAND);
					putimage(35, 34, &option_unselected, SRCPAINT);
					FlushBatchDraw();
					PlaySound((LPCSTR)"botton", NULL, SND_RESOURCE | SND_ASYNC);
					bossselected = 1;
				}
				else if (bossmo <= 1 && mouse.uMsg == WM_LBUTTONUP) {
					PlaySound((LPCSTR)"bottondown", NULL, SND_RESOURCE | SND_ASYNC);
					choose = BOSS;
					break;
				}
			}
		}
	}
	EndBatchDraw();
	return choose;
}
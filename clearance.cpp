#include "snake.h"
struct rank1 {
	char name[10][NAMELIMIT + 1];
	int score[10][5];
	int totalscore[10];
	int id[10];
}rank1;
void sortrank();
void clearance(int score[],int totalscore,char name[],int id,int currentscore) {
	BeginBatchDraw();
	IMAGE pause_a;
	IMAGE clearance;
	IMAGE back_a;
	IMAGE next_selected;
	IMAGE next_unselected;
	loadimage(&pause_a, _T("IMAGE"), _T("pause_a"));
	loadimage(&clearance, _T("IMAGE"), _T("clearance"));
	loadimage(&back_a, _T("IMAGE"), _T("back_a"));
	loadimage(&next_selected, _T("IMAGE"), _T("continue_selected"));
	loadimage(&next_unselected, _T("IMAGE"), _T("continue_unselected"));
	putimage(208, 188, &pause_a, SRCAND);
	putimage(208, 188, &clearance, SRCPAINT);
	putimage(382, 426, &back_a, SRCAND);
	putimage(382, 426, &next_unselected, SRCPAINT);
	//-------字体设置-------//
	LOGFONT clearancefont;
	gettextstyle(&clearancefont);                     // 获取当前字体设置
	clearancefont.lfHeight = 35;
	_tcscpy_s(clearancefont.lfFaceName, _T("微软雅黑"));//设置字体
	clearancefont.lfQuality = ANTIALIASED_QUALITY;    // 设置输出效果为抗锯齿  
	clearancefont.lfWeight = FW_BOLD;
	settextstyle(&clearancefont);
	settextcolor(BLACK);
	//----------------------//
	char s[15];
	char ts[15];
	sprintf(s, "%d", currentscore);
	sprintf(ts, "%d", totalscore);
	outtextxy(405, 265, s);
	settextstyle(25, 0, "微软雅黑");
	outtextxy(346,324,ts);
	//-------上榜判定-------//
	FILE *frank1;
	int i;
	int crank=-1;
	frank1 = fopen("rank.dat", "rb");
	fread(&rank1, sizeof(struct rank1), 1, frank1);
	fclose(frank1);
	int idd[10];
	for (i = 0; i < 9; i++) {
		idd[i] = rank1.id[i];
		if (rank1.id[i] == id) {
			crank = i;
			break;
		}
	}
	if (crank == -1) {
		if (totalscore <= rank1.totalscore[8]) {
			outtextxy(410 - textwidth("你没有上榜，但你还可以继续挑战!") / 2, 356, "你没有上榜，但你还可以继续挑战!");
		}
		else {
			outtextxy(410 - textwidth("恭喜你成功进入排行榜!") / 2, 356, "恭喜你成功进入排行榜!");
			rank1.totalscore[8] = totalscore;
			for (i = 0; i < i; i++) {
				rank1.score[8][i] = score[i];
			}
			strcpy(rank1.name[8], name);
			rank1.id[8] = id;
		}
	}
	else {
		if (totalscore <= rank1.totalscore[crank]) {
			outtextxy(410 - textwidth("你没有打破你的记录!") / 2, 356, "你没有打破你的记录!");
		}
		else {
			outtextxy(410 - textwidth("你创造了新记录!") / 2, 356, "你创造了新记录!");
			rank1.totalscore[crank] = totalscore;
			for (i = 0; i < i; i++) {
				rank1.score[crank][i] = score[i];
			}
			strcpy(rank1.name[crank], name);
		}
	}
	frank1 = fopen("rank.dat", "wb");
	fwrite(&rank1, sizeof(struct rank1), 1, frank1);
	fclose(frank1);
	sortrank();
	//-------上榜判定-------//
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
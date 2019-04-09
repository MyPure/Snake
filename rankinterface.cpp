#include "snake.h"
struct rank0 {
	char name[10][NAMELIMIT + 1];
	int score[10][5];
	int totalscore[10];
	int id[10];
}rank0;
void sortrank();
void rankinterface() {
	sortrank();
	BeginBatchDraw();
	loadimage(NULL, _T("IMAGE"), _T("rankbg"));
	IMAGE back_a;
	IMAGE back_selected;
	IMAGE back_unselected;
	loadimage(&back_a, _T("IMAGE"), _T("back_a"));
	loadimage(&back_selected, _T("IMAGE"), _T("back_selected"));
	loadimage(&back_unselected, _T("IMAGE"), _T("back_unselected"));
	putimage(718, 518, &back_a, SRCAND);
	putimage(718, 518, &back_unselected, SRCPAINT);
	MOUSEMSG mouse;
	bool backselected = 1;
	double backmo;//鼠标的坐标是否在back圆内
	int i;
	//------ -字体设置-------//
	LOGFONT startfont;
	gettextstyle(&startfont);                     // 获取当前字体设置
	startfont.lfHeight = 24;                      // 设置字体高度为 48
	_tcscpy_s(startfont.lfFaceName, _T("微软雅黑"));//设置字体
	startfont.lfQuality = ANTIALIASED_QUALITY;    // 设置输出效果为抗锯齿  
	startfont.lfWeight = FW_MEDIUM;
	settextstyle(&startfont);
	settextcolor(BLACK);
	//-----------------------//
	//--------读取文件-------//
	FILE *frank0;
	frank0 = fopen("rank.dat", "rb");
	fread(&rank0, sizeof(struct rank0), 1, frank0);
	fclose(frank0);
	//-----------------------//
	//--------显示文本--------//
	char c[30];
	for (i = 0; i < 9; i++) {
		if (rank0.name[i][0] == 0) {
			outtextxy(294 - textwidth("虚位以待") / 2, 186 + 35 * i, "虚位以待");
			outtextxy(548 - textwidth("????????") / 2, 186 + 35 * i, "????????");
		}
		else {
			outtextxy(294 - textwidth(rank0.name[i]) / 2, 186 + 35 * i, rank0.name[i]);
			sprintf(c, "%d", rank0.totalscore[i]);
			outtextxy(548 - textwidth(c) / 2, 186 + 35 * i, c);
		}
	}
	outtextxy(441 - textwidth(rank0.name[9]) / 2, 77, rank0.name[9]);
	if (rank0.totalscore[9] == 0) {
		settextstyle(20,0,"微软雅黑");
		outtextxy(425 - textwidth("快去开始你的旅途吧!") / 2, 108, "快去开始你的旅途吧!");
	}
	else {
		sprintf(c, "%d", rank0.totalscore[9]);
		outtextxy(425 - textwidth(c) / 2, 123, c);
		settextstyle(22, 0, "微软雅黑");
		outtextxy(425 - textwidth("你目前的总分是") / 2, 102, "你目前的总分是");
	}
	//-----------------------//
	FlushBatchDraw();
	FlushMouseMsgBuffer();
	while (1) {
		if (MouseHit()) {
			mouse = GetMouseMsg();
			backmo = ((mouse.x - 743.0)*(mouse.x - 743.0) + (mouse.y - 543.0)*(mouse.y - 543.0)) / 625.0;
			if (backmo <= 1 && backselected == 1) {
				putimage(718, 518, &back_a, SRCAND);
				putimage(718, 518, &back_selected, SRCPAINT);
				FlushBatchDraw();
				PlaySound((LPCSTR)"botton", NULL, SND_RESOURCE | SND_ASYNC);
				backselected = 0;
			}
			else if (backmo > 1 && backselected == 0) {
				putimage(718, 518, &back_a, SRCAND);
				putimage(718, 518, &back_unselected, SRCPAINT);
				FlushBatchDraw();
				PlaySound((LPCSTR)"botton", NULL, SND_RESOURCE | SND_ASYNC);
				backselected = 1;
			}
			else if (backmo <= 1 && mouse.uMsg== WM_LBUTTONUP) {
				PlaySound((LPCSTR)"bottondown", NULL, SND_RESOURCE | SND_ASYNC);
				break;
			}
		}
	}
	EndBatchDraw();
}

void sortrank() {
	FILE *frank0;
	frank0 = fopen("rank.dat", "rb");
	fread(&rank0, sizeof(struct rank0), 1, frank0);
	fclose(frank0);
	char tempname[NAMELIMIT + 1];
	int tempscore[5];
	int temptotal;
	int tempid;
	int i, j,k;
	for (i = 0; i < 8; i++) {
		for (j = 0; j < 8 - i; j++) {
			if (rank0.totalscore[j] < rank0.totalscore[j + 1]) {
				temptotal = rank0.totalscore[j];
				rank0.totalscore[j] = rank0.totalscore[j + 1];
				rank0.totalscore[j + 1] = temptotal;
				tempid = rank0.id[j];
				rank0.id[j] = rank0.id[j + 1];
				rank0.id[j + 1] = tempid;
				for (k = 0; k < 5; k++) {
					tempscore[k] = rank0.score[j][k];
					rank0.score[j][k]= rank0.score[j+1][k];
					rank0.score[j + 1][k] = tempscore[k];
				}
				strcpy(tempname, rank0.name[j]);
				strcpy(rank0.name[j], rank0.name[j+1]);
				strcpy(rank0.name[j+1],tempname);
			}
		}
	}
	frank0 = fopen("rank.dat", "wb");
	fwrite(&rank0, sizeof(struct rank0), 1, frank0);
	fclose(frank0);
}
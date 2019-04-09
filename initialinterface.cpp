#include "snake.h"
void flashtext(TCHAR s[], int x, int y, IMAGE* bg);//闪烁文字
void initialinterface() {
	TCHAR s[] = _T("Press Any Key to Continue");
	IMAGE bg;
	initgraph(WIDTH, HEIGHT);
	//initgraph(WIDTH, HEIGHT, SHOWCONSOLE);
	setbkcolor(CYAN);
	setbkmode(TRANSPARENT);//设置图案填充和文字输出时的背景模式为透明
	loadimage(NULL, _T("IMAGE"), _T("startbackground"));
	getimage(&bg, 0, 0, WIDTH, HEIGHT);
	//---------------------------------字体设置
	LOGFONT startfont;
	gettextstyle(&startfont);                     // 获取当前字体设置
	startfont.lfHeight = 30;                      // 设置字体高度为 48
	_tcscpy_s(startfont.lfFaceName, _T("Segoe Print"));//设置字体
	startfont.lfQuality = ANTIALIASED_QUALITY;    // 设置输出效果为抗锯齿  
	startfont.lfWeight = FW_MEDIUM;
	settextstyle(&startfont);
	settextcolor(BGR(0xE86976));
	//---------------------------------
	outtextxy(280, 530, s);
	system("pause");
	PlaySound((LPCSTR)"continue", NULL, SND_RESOURCE | SND_ASYNC);
	flashtext(s, 280, 530, &bg);
}
void flashtext(TCHAR s[], int x, int y, IMAGE* bg) {
	for (int i = 0; i < 8; i++) {
		putimage(0, 0, bg);
		Sleep(80);
		outtextxy(x, y, s);
		Sleep(80);
	}
}
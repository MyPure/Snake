#include "snake.h"
void flashtext(TCHAR s[], int x, int y, IMAGE* bg);//��˸����
void initialinterface() {
	TCHAR s[] = _T("Press Any Key to Continue");
	IMAGE bg;
	initgraph(WIDTH, HEIGHT);
	//initgraph(WIDTH, HEIGHT, SHOWCONSOLE);
	setbkcolor(CYAN);
	setbkmode(TRANSPARENT);//����ͼ�������������ʱ�ı���ģʽΪ͸��
	loadimage(NULL, _T("IMAGE"), _T("startbackground"));
	getimage(&bg, 0, 0, WIDTH, HEIGHT);
	//---------------------------------��������
	LOGFONT startfont;
	gettextstyle(&startfont);                     // ��ȡ��ǰ��������
	startfont.lfHeight = 30;                      // ��������߶�Ϊ 48
	_tcscpy_s(startfont.lfFaceName, _T("Segoe Print"));//��������
	startfont.lfQuality = ANTIALIASED_QUALITY;    // �������Ч��Ϊ�����  
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
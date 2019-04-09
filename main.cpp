#include "snake.h"
#define HEADA 30
#define BALLR 2
#define FELLBALLR 5
#define SPFOODR 15
#define NORMALBALLR 10
#define LEVEL1WINNEEDLENGTH 100
#define LEVEL2WINNEEDLENGTH 80.0
#define LEVEL2TIMELIMIT 10000.0
#define RTIME 200
#define GTIME 200
#define BTIME 200
#define CHAOSTIME 200
#define FREEZETIME 200
#define REPELTIME 200
#define INVISIBLETIME 100
#define PY 10//掉落食物的偏移量
struct snake {
	int x;
	int y;
	int state;
	COLORREF color;
	struct snake *previous;
	struct snake *next;
};
struct food {
	int x;
	int y;
	int size;
	COLORREF color;
	struct food *previous;
	struct food *next;
};
struct spfood {
	int x;
	int y;
	int type;
	int time;
	struct spfood *previous;
	struct spfood *next;
};
struct bufftime {
	int rtime;
	int gtime;
	int btime;
};
struct debufftime {
	int chaostime;
	int repelfoodtime;
	int freezetime;
	int invisibletime;
};
struct gamestate {
	bool movie;//是否看过开场动画
	bool clearance;//是否通关
	int passlevel;//已经通过了几关
	int score[5];//各个关卡得分
	int id;
	char playername[NAMELIMIT+1];
	//以上是全局状态，以下是暂时状态
	int currentlevel;//上次保存了第几关（0为未保存）
	int currentscore;//上次保存的分数
	int currenttotaleatfoods;//上次保存的吃了多少小能量
	int currentgametime;
	double currentrad;//上次保存的蛇头旋转角度
	struct bufftime currentbufftime;//上次保存的buff状态
	struct debufftime currentdebufftime;
};
struct rank {
	char name[10][NAMELIMIT + 1];
	int score[10][5];
	int totalscore[10];
	int id[10];
};
//----------全局变量-----------//
struct snake *head=NULL;
struct snake *tail = NULL;
struct food *foodhead = NULL;
struct food *foodtail = NULL;
struct spfood *spfoodhead=NULL;
struct spfood *spfoodtail = NULL;
double headfoodmo = (HEADA / 2 + BALLR)*(HEADA / 2 + BALLR);//吃到食物的判定值
int snakebuff;
struct gamestate gamestate;//游戏状态
struct rank rank;
int snakelength;
double speed;
double rotationspeed;
double attractrange;//最大吸附距离
double attractdistancemax = 10;//单次吸附距离最大值
int settlescore;
//----------全局变量-----------//
//----------关卡预设-----------//
int maxfoodspreset[5] = {200,150,100,100,100};//食物数量最大值
int growneedfoodspreset[5] = { 30,15,30,30,30 };
int growneedfoods;//变长一节需要的食物
//----------关卡预设-----------//
//--------运行主体--------//
void gamestart(int);//游戏运行主函数
void movesnake(char,int,int);//移动蛇，返回吃到的食物
void updatefood(struct food *,int);//更新食物
void attractfood(struct food *);//吸引食物
int judgesnake(int);//判断蛇
void death(int);
void updatesnakelength();
void updatespfood(int);
bool judgecreate(int, int, int);
//--------运行主体--------//
//--------画面显示--------//
void printworld(int);//刷新画面
void printbg(int);//打印背景
void printsnake(int);//打印蛇
void printfood();//打印食物
void printspfood();
//--------画面显示--------//
//-----内存分配----//
void creatsnake();//生成蛇
void creatfood(int);//生成食物
void creatspfood();
void freesnake();//free
void freefood();
void freespfood();
void addspfood(int, int, int, int);
void deletespfood(struct spfood *);
void snakeboom();
//-----内存分配----//
//-------存档------//
void loadsnake();//读取蛇
void loadfood();//读取食物
void loadspfood();
void savesnake();//保存蛇
void savefood();//保存食物
void savespfood();
void loadgamestate();//读取存档
void savecurrentgamestate(int);//存当前档
void clearcurrentgamesave();//清空当前存档
void resetgame();//重置游戏
void saveglobalgame();//保存全局存档
void saverank();
void clearrank();
//-------存档------//
//-------UI--------//
int gamepause();//暂停
void initialinterface();//初始界面
int mainmenuinterface(bool,int);//主菜单界面
void rankinterface();//排行榜界面
int chooselevel(int,int,bool);//选关界面
int sureexit();//确认是否退出界面
int option();//设置和重置界面
int surereset();//确定重置界面
int gameover();//死亡
void win(int);//胜利
void flashtext(TCHAR s[], int x, int y, IMAGE* bg);//闪烁文字
void printmainUI(int);
void clearance(int [], int,char [],int id,int);
//-------UI--------//
int main() {
	int choose;
	int level;
	srand((unsigned)time(NULL));
	loadgamestate();
	//输入名字
	if (gamestate.playername[0] == 0) {
		int namen = 0;
		bool namepass = 0;
		char c;
		printf("敢问阁下尊姓大名？\n请输入你的名字（%d字符以内）：",NAMELIMIT);
		while (namepass==0) {
			fflush(stdin);
			while ((c = getchar()) != '\n') {
				gamestate.playername[namen] = c;
				namen++;
				if (namen >= NAMELIMIT+1) {
					printf("输入不合法！请重新输入：");
					namepass = 0;
					namen = 0;
					for (int i = 0; i < NAMELIMIT+1; i++) {
						gamestate.playername[i] = 0;
					}
					break;
				}
				else {
					namepass = 1;
				}
			}
		}
		gamestate.playername[NAMELIMIT] = 0;
		gamestate.id = rand();
		saveglobalgame();
	}
	//输入名字
	saverank();
	initialinterface();
	while (1) {
		choose = mainmenuinterface(gamestate.clearance,gamestate.passlevel);
		IMAGE bg;
		getimage(&bg, 0, 0, WIDTH, HEIGHT);
		if (choose == START) {
			while ((level = chooselevel(gamestate.currentlevel, gamestate.passlevel, gamestate.clearance)) != 0) {
				if (level == 6) {//存档的关卡
					gamestart(gamestate.currentlevel);
				}
				else {
					clearcurrentgamesave();
					gamestart(level);
				};
			}
		}
		else if(choose == RANK) {
			rankinterface();
		}
		else if (choose == EXIT) {
			if (sureexit() == SUREEXIT) {
				break;
			}
		}
		else if (choose == OPTION) {
			while ((choose = option())!=BACK) {
				if (choose = SURERESET) {
					putimage(0, 0, &bg);
					choose = surereset();
					if (choose == RESETGAME) {
						resetgame();
					}
				}
			}
		}
		else if (choose == BOSS) {
			clearcurrentgamesave();
			gamestart(5);
		}
	}
	closegraph();
	return 0;
}
void gamestart(int level){
	int keyl;
	int keyr;
	int keyup;
	int keyesc;
	int pausechoose;
	int gameoverchoose;
	int cscore;//本次移动若吃到食物应当加的分
	double up;
	int buffr, buffg, buffb,bufffreeze;
	bool pause;
	int judge;
	int clearsumscore;
	struct food *eatfoodp=NULL;
	pausechoose = CONTINUEGAME;
	while (pausechoose != BACKTOMENU) {
		pause = 0;
		//loadgamestate();
		if (gamestate.currentlevel != 0) {
			loadsnake();
			loadfood();
			loadspfood();
			printworld(level);
			//暂停一下
			pausechoose = gamepause();
			if (pausechoose == BACKTOMENU) {
				savesnake();
				savefood();
				savespfood();
				savecurrentgamestate(level);
				freesnake();
				freefood();
				freespfood();
				return;
			}
			else if (pausechoose == RESTARTGAME) {
				freesnake();
				freefood();
				freespfood();
				clearcurrentgamesave();
				creatsnake();
				creatfood(level);
				creatspfood();
			}
			//暂停一下
		}
		else {
			creatsnake();
			creatfood(level);
			creatspfood();
		}
		if (level == 1) {
			PlaySound((LPCSTR)"springbgm", NULL, SND_RESOURCE | SND_ASYNC | SND_LOOP);
		}
		else if (level == 2) {
			PlaySound((LPCSTR)"summerbgm", NULL, SND_RESOURCE | SND_ASYNC | SND_LOOP);
		}
		while (1) {
			keyesc = GetKeyState(VK_ESCAPE);
			keyup = GetKeyState(VK_UP);
			keyl = GetKeyState(VK_LEFT);
			keyr = GetKeyState(VK_RIGHT);
			if (keyesc < 0) {
				pausechoose = gamepause();
				if (pausechoose == BACKTOMENU) {
					savesnake();
					savefood();
					savespfood();
					savecurrentgamestate(level);
					freesnake();
					freefood();
					freespfood();
					return;
				}
				else if (pausechoose == RESTARTGAME) {
					freesnake();
					freefood();
					freespfood();
					clearcurrentgamesave();
					break;
				}
				pause = 1;
			}
			if (pause == 1) {
				if (level == 1) {
					PlaySound((LPCSTR)"springbgm", NULL, SND_RESOURCE | SND_ASYNC | SND_LOOP);
				}
				else if (level == 2) {
					PlaySound((LPCSTR)"summerbgm", NULL, SND_RESOURCE | SND_ASYNC | SND_LOOP);
				}
				pause = 0;
			}
			//---------buff---------//
			if (keyup < 0) {
				up = 1;
			}
			else {
				up = 0;
			}
			if (snakebuff == R) {
				buffr = 1;
				buffg = buffb = 0;
			}
			else if (snakebuff == G) {
				buffg = 1;
				buffr = buffb = 0;
			}
			else if (snakebuff == B) {
				buffb = 1;
				buffr = buffg = 0;
			}
			else if (snakebuff == RandG) {
				buffr = 1;
				buffg = 1;
				buffb = 0;
			}
			else if (snakebuff == RandB) {
				buffr = 1;
				buffb = 1;
				buffg = 0;
			}
			else if (snakebuff == GandB) {
				buffb = 1;
				buffg = 1;
				buffr = 0;
			}
			else if (snakebuff == RandGandB) {
				buffr = 1;
				buffg = 1;
				buffb = 1;
			}
			else {
				buffr = buffg = buffb = 0;
			}
			if (gamestate.currentdebufftime.freezetime > 0) {
				bufffreeze = 1;
			}
			else {
				bufffreeze = 0;
			}
			growneedfoods = growneedfoodspreset[level-1] - growneedfoodspreset[level - 1] * 2 / 3 * buffb;
			speed = 6 + 2 * buffg + 2 * up * !(bufffreeze)-2 * bufffreeze;
			attractrange = 50 + 50 * buffr + 10 * up;
			attractdistancemax = 15 + 9 * buffr + 3 * up;
			rotationspeed = PI / (15 + 3 * buffg + 2 * up);
			//分数
			if (level == 1) {
				cscore = (6 + 30 * buffb) * (buffr + 1)*(buffg + 1)*(buffb + 1);
			}
			else if (level == 2) {
				cscore = (2 + 5 * buffb) * (buffr + 1)*(buffg + 1)*(buffb + 1);
			}
			else {
				cscore = 0;
			}
			if (keyl < 0 || keyr < 0) {
				if (keyl < 0 && keyr >= 0) {
					if (gamestate.currentdebufftime.chaostime > 0) {
						movesnake('R', cscore,level);
					}
					else {
						movesnake('L', cscore,level);
					}
				}
				else if (keyr < 0 && keyl >= 0) {
					if (gamestate.currentdebufftime.chaostime > 0) {
						movesnake('L', cscore,level);
					}
					else {
						movesnake('R', cscore,level);
					}
				}
				else {
					movesnake('S', cscore, level);
				}
			}
			else {
				movesnake('S', cscore, level);
			}
			updatesnakelength();
			updatespfood(level);
			printworld(level);
			judge = judgesnake(level);
			gamestate.currentgametime++;
			if (judge == DEATH || judge == BOOMDEATH) {
				PlaySound((LPCSTR)"gameover", NULL, SND_RESOURCE | SND_ASYNC);
				Sleep(600);
				if (judge == DEATH) {
					death(level);
				}
				clearcurrentgamesave();
				freesnake();
				freefood();
				freespfood();
				gameoverchoose = gameover();
				if (gameoverchoose == RESTARTGAME) {
					break;
				}
				else if (gameoverchoose == BACKTOMENU) {
					return;
				}
			}
			else if (judge == WIN||judge==CLEARANCE) {
				PlaySound((LPCSTR)"win", NULL, SND_RESOURCE | SND_ASYNC);
				if (gamestate.score[level - 1] < gamestate.currentscore) {
					gamestate.score[level - 1] = gamestate.currentscore;
					saverank();
				}
				if (gamestate.passlevel < level) {
					gamestate.passlevel = level;
				}
				if (judge == WIN) {
					win(gamestate.currentscore);
				}
				else {
					clearsumscore = 0;
					for (int i = 0; i < 5; i++) {
						if (i != level - 1) {
							clearsumscore += gamestate.score[i];
						}
					}
					clearsumscore += gamestate.currentscore;
					clearance(gamestate.score,clearsumscore,gamestate.playername,gamestate.id, gamestate.currentscore);
					gamestate.clearance = 1;
				}
				clearcurrentgamesave();
				saveglobalgame();
				freesnake();
				freefood();
				freespfood();
				return;
			}
			Sleep(25);
		}
	}
}
void creatsnake() {
	head = (struct snake *)malloc(sizeof(struct snake));
	head->x = 400;
	head->y = 50;
	head->state = NORMAL;
	head->next = NULL;
	head->previous = NULL;
	head->color = BLACK;
	struct snake *p1, *p2;
	p1 = p2 = head;
	for (int i = 0; i < 2; i++) {
		p1 = (struct snake *)malloc(sizeof(struct snake));
		p2->next = p1;
		p1->previous = p2;
		p1->state = NORMAL;
		p1->x = p2->x;
		p1->y = p2->y - (int)speed;
		p1->color = BLACK;
		p2 = p1;
	}
	tail = p2;
	tail->next = NULL;
}
void loadsnake() {
	FILE *fsnake;
	int n, i;
	int r, g, b;
	fsnake = fopen("snake.dat", "r");
	fscanf(fsnake, "%d", &n);
	struct snake *p, *p1, *p2;
	head = (struct snake *)malloc(sizeof(struct snake));
	fscanf(fsnake, "%d,%d,%d,%d,%d,%d", &head->x, &head->y, &head->state,&r, &g, &b);
	head->color = RGB(r, g, b);
	head->previous = NULL;
	head->next = NULL;
	p = p1 = p2 = head;
	for (i = 0; i < n-1; i++) {
		p1 = (struct snake *)malloc(sizeof(struct snake));
		fscanf(fsnake, "%d,%d,%d,%d,%d,%d", &p1->x, &p1->y,&p1->state, &r, &g, &b);
		p1->color = RGB(r, g, b);
		p1->previous = p2;
		p1->next = NULL;
		p2->next = p1;
		p2 = p1;
	}
	tail = p1;
	tail->next = NULL;
	fclose(fsnake);
}
void freesnake() {
	struct snake *p,*pf;
	p= head->next;
	pf = head;
	while (p != NULL) {
		free(pf);
		pf = p;
		p = p->next;
	}
	free(pf);
	head = NULL;
	tail = NULL;
}
void movesnake(char c,int cscore,int level) {
	struct snake *p1, *p2,*p;
	struct food *pf = foodhead, *attractfp = foodhead;
	double xm = speed * sin(gamestate.currentrad);
	double ym = speed * cos(gamestate.currentrad);
	while (pf != NULL) {
		if (((head->x - pf->x)*(head->x - pf->x) + (head->y - pf->y)*(head->y - pf->y)) <= headfoodmo) {
			if (pf->size == 1) {
				gamestate.currenttotaleatfoods += 1;
				updatefood(pf,level);
				gamestate.currentscore += cscore;
			}
			else if (pf->size == 2) {
				gamestate.currenttotaleatfoods += (int)(growneedfoods/2);
				pf = pf->previous;
				updatefood(pf->next,level);
			}
		}
		if (pf->x > WIDTH - BALLR || pf->x<BALLR || pf->y>HEIGHT - BALLR || pf->y < BALLR||judgecreate(pf->x,pf->y,level)) {
			if (pf->size == 1) {
				updatefood(pf,level);
			}
			else if (pf->size == 2) {
				pf = pf->previous;
				updatefood(pf->next, level);
			}
		}
		pf = pf->next;
	}
	if (c=='L') {
		gamestate.currentrad += rotationspeed;
	}
	else if (c == 'R') {
		gamestate.currentrad -=  rotationspeed;
	}
	while (attractfp != NULL) {
		if (((head->x - attractfp->x)*(head->x - attractfp->x) + (head->y - attractfp->y)*(head->y - attractfp->y)) <= attractrange * attractrange) {
			attractfood(attractfp);
		}
		attractfp = attractfp->next;
	}
	p1 = (struct snake *)malloc(sizeof(struct snake));
	p1->x = head->x + (int)xm;
	p1->y = head->y + (int)ym;
	p1->state = NORMAL;
	p1->next = head;
	head->previous = p1;
	p1->previous = NULL;
	head = p1;
	if (gamestate.currenttotaleatfoods >= growneedfoods) {
		p = head;
		while (p->next != NULL) {
			p->color = p->next->color;
			p = p->next;
		}
		tail->color = RGB(rand() % 255, rand() % 255, rand() % 255);
		gamestate.currenttotaleatfoods -= growneedfoods;
	}
	else {
		p2 = tail->previous;
		p = head;
		while (p->next != NULL) {
			p->color = p->next->color;
			p = p->next;
		}
		p2->next = NULL;
		free(tail);
		tail = p2;
	}
}
void snakeboom() {
	struct snake *ps1, *ps2;
	struct food *pf1, *pf2;
	int n = (int)((snakelength - 1)/2);
	int i;
	ps1 = ps2 = tail;
	pf1 = pf2 = foodtail;
	for (i = 0; i < n; i++) {
		pf1 = (struct food*)malloc(sizeof(struct food));
		pf1->x = ps1->x += rand() % PY * 2 - PY;
		pf1->y = ps1->y += rand() % PY * 2 - PY;
		pf1->size = 2;
		pf1->color = ps1->color;
		pf1->previous = pf2;
		pf1->next = NULL;
		pf2->next = pf1;
		pf2 = pf1;
		ps2 = ps2->previous;
		free(ps1);
		ps1 = ps2;
		ps1->next = NULL;
	}
	tail = ps1;
	foodtail = pf1;
}
void printworld(int level) {
	BeginBatchDraw();
	printbg(level);
	printfood();
	printspfood();
	printsnake(level);
	printmainUI(level);
	FlushBatchDraw();
	EndBatchDraw();
}

void creatfood(int level) {
	int totalfoods = 0;
	int x, y;
	struct food *p, *p1, *p2;
	foodhead = (struct food *)malloc(sizeof(struct food));
	do {
		x = rand() % (WIDTH - BALLR * 2) + BALLR;
		y = rand() % (HEIGHT - BALLR * 2) + BALLR;
	} while (judgecreate(x, y, level));
	foodhead->x = x;
	foodhead->y = y;
	foodhead->size = 1;
	foodhead->color = RGB(rand() % 255, rand() % 255, rand() % 255);
	foodhead->previous = NULL;
	foodhead->next = NULL;
	p = p1 = p2 = foodhead;
	while (totalfoods < maxfoodspreset[level-1]-1) {
		p1 = (struct food *)malloc(sizeof(struct food));
		do {
			x = rand() % (WIDTH - BALLR * 2) + BALLR;
			y = rand() % (HEIGHT - BALLR * 2) + BALLR;
		} while (judgecreate(x, y, level));
		p1->x = x;
		p1->y = y;
		p1->size = 1;
		p1->color = RGB(rand() % 255, rand() % 255, rand() % 255);
		p1->previous = p2;
		p1->next = NULL;
		p2->next = p1;
		p2 = p1;
		totalfoods += 1;
	}
	foodtail = p1;
	foodtail->next = NULL;
}
void loadfood() {
	FILE *ffood;
	int n,i;
	int r, g, b;
	ffood = fopen("food.dat", "r");
	fscanf(ffood,"%d", &n);
	struct food *p, *p1, *p2;
	foodhead = (struct food *)malloc(sizeof(struct food));
	fscanf(ffood, "%d,%d,%d,%d,%d,%d", &foodhead->x, &foodhead->y,&foodhead->size, &r, &g, &b);
	foodhead->color = RGB(r, g, b);
	foodhead->previous = NULL;
	foodhead->next = NULL;
	p = p1 = p2 = foodhead;
	for (i = 0; i < n-1;i++) {
		p1 = (struct food *)malloc(sizeof(struct food));
		fscanf(ffood, "%d,%d,%d,%d,%d,%d", &p1->x, &p1->y,&p1->size ,&r, &g, &b);
		p1->color = RGB(r, g, b);
		p1->previous = p2;
		p1->next = NULL;
		p2->next = p1;
		p2 = p1;
	}
	foodtail = p1;
	foodtail->next = NULL;
	fclose(ffood);
}
void freefood() {
	struct food *p, *pf;
	p = foodhead->next;
	pf = foodhead;
	while (p != NULL) {
		free(pf);
		pf = p;
		p = p->next;
	}
	free(pf);
	foodhead = NULL;
	foodtail = NULL;
}
void updatefood(struct food *eatfoodp,int level) {
	int x, y;
	if (eatfoodp != NULL) {
		if (eatfoodp->size == 1) {
			do {
				x = rand() % (WIDTH - BALLR * 2) + BALLR;
				y = rand() % (HEIGHT - BALLR * 2) + BALLR;
			} while (judgecreate(x, y, level));
			eatfoodp->x = x;
			eatfoodp->y = y;
			eatfoodp->color = RGB(rand() % 255, rand() % 255, rand() % 255);
		}
		else if (eatfoodp->size == 2) {
			if (eatfoodp != foodtail) {
				eatfoodp->previous->next = eatfoodp->next;
				eatfoodp->next->previous = eatfoodp->previous;
				free(eatfoodp);
			}
			else {
				foodtail = eatfoodp->previous;
				eatfoodp->previous->next = NULL;
				free(eatfoodp);
			}
		}
	}
}
void creatspfood() {
	spfoodhead = (struct spfood *)malloc(sizeof(struct spfood));
	spfoodhead->x = spfoodhead->y = spfoodhead->time = spfoodhead->type = 0;
	spfoodhead->next = spfoodhead->previous = NULL;
	spfoodtail = spfoodhead;
}

void updatespfood(int level) {
	struct spfood *p;
	int x, y;
	int r;//数量
	int g;//数量
	int b;//数量
	int poison;//数量
	int bomb;//数量
	int eattype;//吃了啥
	r = 0;
	g = 0;
	b = 0;
	bomb = 0;
	poison = 0;
	p = spfoodhead->next;
	gamestate.currentbufftime.rtime -= gamestate.currentbufftime.rtime > 0 ;
	gamestate.currentbufftime.gtime -= gamestate.currentbufftime.gtime > 0;
	gamestate.currentbufftime.btime -= gamestate.currentbufftime.btime > 0 ;
	gamestate.currentdebufftime.chaostime -= gamestate.currentdebufftime.chaostime > 0;
	gamestate.currentdebufftime.repelfoodtime -= gamestate.currentdebufftime.repelfoodtime > 0;
	gamestate.currentdebufftime.freezetime -= gamestate.currentdebufftime.freezetime > 0;
	gamestate.currentdebufftime.invisibletime -= gamestate.currentdebufftime.invisibletime > 0;
	while (p!=NULL) {//计数
		if (p->type == 1) {
			r++;
		}
		else if (p->type == 2) {
			g++;
		}
		else if (p->type == 3) {
			b++;
		}
		else if (p->type == 4) {
			poison++;
		}
		else if (p->type == 5) {
			bomb++;
		}
		p = p->next;
	}
		//-------生成--------//
	if (gamestate.currentbufftime.rtime == 0 && r < 1 && rand() % 100000 <= 62) {
		do {
			x = rand() % (WIDTH - SPFOODR * 2) + SPFOODR;
			y = rand() % (HEIGHT - SPFOODR * 2) + SPFOODR;
		} while (judgecreate(x, y, level));
		addspfood(x, y, 1, 200);
	}
	if (gamestate.currentbufftime.gtime == 0 && g < 1 && rand() % 100000 <= 124) {
		do {
			x = rand() % (WIDTH - SPFOODR * 2) + SPFOODR;
			y = rand() % (HEIGHT - SPFOODR * 2) + SPFOODR;
		} while (judgecreate(x, y, level));
		addspfood(x, y, 2, 200);
	}
	if (gamestate.currentbufftime.btime == 0 && b < 1 && rand() % 100000 <= 42) {
		do {
			x = rand() % (WIDTH - SPFOODR * 2) + SPFOODR;
			y = rand() % (HEIGHT - SPFOODR * 2) + SPFOODR;
		} while (judgecreate(x, y, level));
		addspfood(x, y, 3, 200);
	}
	if (poison < 5 && rand() % 100000 <= 1249) {
		do {
			x = rand() % (WIDTH - SPFOODR * 2) + SPFOODR;
			y = rand() % (HEIGHT - SPFOODR * 2) + SPFOODR;
		} while (judgecreate(x, y, level) || (head->x - x)*(head->x - x) + (head->y - y)*(head->y - y) <= 10000);
		addspfood(x, y, 4, 200);
	}
	if (bomb < 1 && rand() % 100000 <= 165) {
		do {
			x = rand() % (WIDTH - SPFOODR * 2) + SPFOODR;
			y = rand() % (HEIGHT - SPFOODR * 2) + SPFOODR;
		} while (judgecreate(x, y, level) || (head->x - x)*(head->x - x) + (head->y - y)*(head->y - y) <= 10000);
		addspfood(x, y, 5, 200);
	}
		//-------删除--------//
	p = spfoodhead->next;
	while (p != NULL) {
		if (p->type == 4 && --p->time <= 0) {
			deletespfood(p);
			break;
		}
		p = p->next;
	}
		//-------判定--------//
	p = spfoodhead->next;
	eattype = NULL;
	while (p != NULL) {
		if ((head->x - p->x)*(head->x - p->x) + (head->y - p->y)*(head->y - p->y) <= 529) {
			if (p->type == 1) {
				eattype = R;
			}
			else if (p->type == 2) {
				eattype = G;
			}
			else if (p->type == 3) {
				eattype = B;
			}
			else if (p->type == 4) {
				eattype = POISON;
			}
			else if (p->type == 5) {
				eattype = BOMB;
			}
			deletespfood(p);
			break;
		}
		p = p->next;
	}
		//-------结算--------//
	if (eattype == R) {
		gamestate.currentbufftime.rtime = RTIME;
	}
	else if (eattype == G) {
		gamestate.currentbufftime.gtime = GTIME;
	}
	else if (eattype == B) {
		gamestate.currentbufftime.btime = BTIME;
	}
	else if (eattype == POISON) {
		int randnumber = rand() % 4;
		if (randnumber == 0) {
			gamestate.currentdebufftime.chaostime = CHAOSTIME;
		}
		else if (randnumber == 1) {
			gamestate.currentdebufftime.freezetime = FREEZETIME;
		}
		else if (randnumber == 2) {
			gamestate.currentdebufftime.repelfoodtime = REPELTIME;
		}
		else if (randnumber == 3) {
			gamestate.currentdebufftime.invisibletime = INVISIBLETIME;
		}
		gamestate.currentscore = gamestate.currentscore * 3 / 4;
	}
	else if (eattype == BOMB) {
		snakeboom();
		gamestate.currentscore /= 2;
	}
	snakebuff = 0;
	if (gamestate.currentbufftime.rtime > 0) {
		snakebuff += R;
	}
	if (gamestate.currentbufftime.gtime > 0) {
		snakebuff += G;
	}
	if (gamestate.currentbufftime.btime > 0) {
		snakebuff += B;
	}
}
void addspfood(int x,int y,int type,int time) {
	struct spfood *p;
	p = (struct spfood *)malloc(sizeof(struct spfood));
	p->next = NULL;
	p->previous = spfoodtail;
	p->type = type;
	p->time = time;
	p->x = x;
	p->y = y;
	spfoodtail->next = p;
	spfoodtail = p;
}
void deletespfood(struct spfood *p) {
	if (p != spfoodtail) {
		p->previous->next = p->next;
		p->next->previous = p->previous;
		free(p);
	}
	else {
		spfoodtail = p->previous;
		p->previous->next = NULL;
		free(p);
	}
}
void savespfood() {
	struct spfood *p;
	int n = 0, i;
	FILE *fspfood;
	p = spfoodhead;
	while (p != NULL) {
		n++;
		p = p->next;
	}
	fspfood = fopen("spfood.dat", "w");
	fprintf(fspfood, "%d\n", n);
	p = spfoodhead;
	for (i = 0; i < n; i++) {
		fprintf(fspfood, "%d,%d,%d,%d\n", p->x, p->y, p->type,p->time);
		p = p->next;
	}
	fclose(fspfood);
}
void loadspfood() {
	FILE *fspfood;
	int n, i;
	fspfood = fopen("spfood.dat", "r");
	fscanf(fspfood, "%d", &n);
	struct spfood *p, *p1, *p2;
	spfoodhead = (struct spfood *)malloc(sizeof(struct spfood));
	fscanf(fspfood, "%d,%d,%d,%d", &spfoodhead->x, &spfoodhead->y, &spfoodhead->type, &spfoodhead->time);
	spfoodhead->previous = NULL;
	spfoodhead->next = NULL;
	p = p1 = p2 = spfoodhead;
	for (i = 0; i < n - 1; i++) {
		p1 = (struct spfood *)malloc(sizeof(struct spfood));
		fscanf(fspfood, "%d,%d,%d,%d", &p1->x, &p1->y, &p1->type, &p1->time);
		p1->previous = p2;
		p1->next = NULL;
		p2->next = p1;
		p2 = p1;
	}
	spfoodtail = p1;
	spfoodtail->next = NULL;
	fclose(fspfood);
}
void freespfood() {
	struct spfood *p, *pf;
	p = spfoodhead->next;
	pf = spfoodhead;
	while (p != NULL) {
		free(pf);
		pf = p;
		p = p->next;
	}
	free(pf);
	spfoodhead = NULL;
	spfoodtail = NULL;
}
void printfood() {
	struct food *pf=foodhead->next;
	while (pf != NULL) {
		setfillcolor(pf->color);
		if (pf->size == 1) {
			if (gamestate.currentbufftime.btime>0) {
				solidcircle(pf->x, pf->y, BALLR + 1);
			}
			else {
				solidcircle(pf->x, pf->y, BALLR);
			}
		}
		else if (pf->size == 2) {
			solidcircle(pf->x, pf->y, FELLBALLR);
		}
		pf = pf->next;
	}
}
void printspfood() {
	IMAGE red[2];
	IMAGE green[2];
	IMAGE blue[2];
	IMAGE bomb[2];
	IMAGE poison[2];
	loadimage(&red[0], _T("IMAGE"), _T("red_a"));
	loadimage(&red[1], _T("IMAGE"), _T("red_b"));
	loadimage(&green[0], _T("IMAGE"), _T("green_a"));
	loadimage(&green[1], _T("IMAGE"), _T("green_b"));
	loadimage(&blue[0], _T("IMAGE"), _T("blue_a"));
	loadimage(&blue[1], _T("IMAGE"), _T("blue_b"));
	loadimage(&bomb[0], _T("IMAGE"), _T("bomb_a"));
	loadimage(&bomb[1], _T("IMAGE"), _T("bomb_b"));
	loadimage(&poison[0], _T("IMAGE"), _T("poison_a"));
	loadimage(&poison[1], _T("IMAGE"), _T("poison_b"));
	struct spfood *pf = spfoodhead->next;
	while (pf != NULL) {
		if (pf->type == 1) {
			putimage(pf->x- SPFOODR, pf->y- SPFOODR, &red[0], SRCAND);
			putimage(pf->x- SPFOODR, pf->y- SPFOODR, &red[1], SRCPAINT);
		}
		else if (pf->type == 2) {
			putimage(pf->x - SPFOODR, pf->y - SPFOODR, &green[0], SRCAND);
			putimage(pf->x - SPFOODR, pf->y - SPFOODR, &green[1], SRCPAINT);
		}
		else if (pf->type == 3) {
			putimage(pf->x - SPFOODR, pf->y - SPFOODR, &blue[0], SRCAND);
			putimage(pf->x - SPFOODR, pf->y - SPFOODR, &blue[1], SRCPAINT);
		}
		else if (pf->type == 4) {
			if (pf->time >= 40 ||pf->time / 5 % 2 == 0) {
				putimage(pf->x - SPFOODR, pf->y - SPFOODR, &poison[0], SRCAND);
				putimage(pf->x - SPFOODR, pf->y - SPFOODR, &poison[1], SRCPAINT);
			}
		}
		else if (pf->type == 5) {
			putimage(pf->x - SPFOODR, pf->y - SPFOODR, &bomb[0], SRCAND);
			putimage(pf->x - SPFOODR, pf->y - SPFOODR, &bomb[1], SRCPAINT);
		}
		pf = pf->next;
	}
}
void printbg(int level) {
	if (level == 1) {
		if (gamestate.clearance == 1) {
			loadimage(NULL, _T("IMAGE"), _T("level1bg"));
		}
		else {
			loadimage(NULL, _T("IMAGE"), _T("level1bg_h"));
		}
	}
	else if (level == 2) {
		if (gamestate.clearance == 1) {
			if (judgecreate(head->x, head->y, level)) {
				loadimage(NULL, _T("IMAGE"), _T("level2bg_s"));
			}
			else {
				loadimage(NULL, _T("IMAGE"), _T("level2bg"));
			}
		}
		else {
			if (judgecreate(head->x, head->y, level)) {
				loadimage(NULL, _T("IMAGE"), _T("level2bg_h_s"));
			}
			else {
				loadimage(NULL, _T("IMAGE"), _T("level2bg_h"));
			}
		}
	}
}

void printsnake(int level) {
	if (gamestate.currentdebufftime.invisibletime == 0) {
		struct snake *p;
		IMAGE snakehead_a;
		IMAGE snakehead_b;
		IMAGE snakeheadrotate_a;
		IMAGE snakeheadrotate_b;
		p = tail;
		while (p != head) {
			setfillcolor(p->color);
			if (p->state == DEATH) {
				solidcircle(p->x, p->y, FELLBALLR);
			}
			else if (p->state == NORMAL) {
				solidcircle(p->x, p->y, NORMALBALLR);
			}
			p = p->previous;
		}
		loadimage(&snakehead_a, _T("IMAGE"), _T("head_a"));
		loadimage(&snakehead_b, _T("IMAGE"), _T("head_b"));
		rotateimage(&snakeheadrotate_a, &snakehead_a, PI + gamestate.currentrad, WHITE);
		rotateimage(&snakeheadrotate_b, &snakehead_b, PI + gamestate.currentrad, BLACK);
		putimage(head->x - HEADA / 2, head->y - HEADA / 2, &snakeheadrotate_a, SRCAND);
		putimage(head->x - HEADA / 2, head->y - HEADA / 2, &snakeheadrotate_b, SRCPAINT);
	}
}

void attractfood(struct food *pf) {
	double attractdistance;//当次吸附距离
	double  attractforcemin = 0.2;//最小吸附力度,0~1
	int x1, x2, y1, y2;
	double fhrad;//蛇头和食物的角度
	x1 = pf->x;
	x2 = head->x;
	y1 = pf->y;
	y2 = head->y;
	attractdistance = (1 - (sqrt((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2)) / attractrange))*(attractdistancemax*(1 - attractforcemin)) + attractdistancemax * attractforcemin;
	fhrad = atan((y1 - y2) / ((x1 - x2) == 0 ? 0.00001 : (x1 - x2)));
	if (head->x > pf->x) {
		if (gamestate.currentdebufftime.repelfoodtime > 0) {
			pf->x -= (int)(attractdistance / pf->size * cos(fhrad));
			pf->y -= (int)(attractdistance / pf->size * sin(fhrad));
		}
		else {
			pf->x += (int)(attractdistance / pf->size * cos(fhrad));
			pf->y += (int)(attractdistance / pf->size * sin(fhrad));
		}
	}
	else {
		if (gamestate.currentdebufftime.repelfoodtime > 0) {
			pf->x += (int)(attractdistance / pf->size * cos(fhrad));
			pf->y += (int)(attractdistance / pf->size * sin(fhrad));
		}
		else {
			pf->x -= (int)(attractdistance / pf->size * cos(fhrad));
			pf->y -= (int)(attractdistance / pf->size * sin(fhrad));
		}
	}
}

void loadgamestate() {
	int i;
	FILE *fgamestate;
	fgamestate = fopen("gamestate.dat", "rb");
	if (fgamestate == NULL) {
		fgamestate = fopen("gamestate.dat", "wb");
		gamestate.movie = 0;
		gamestate.clearance = 0;
		gamestate.passlevel = 0;
		gamestate.id = 0;
		for (i = 0; i < 5; i++) {
			gamestate.score[i] = 0;
		}
		for (i = 0; i < NAMELIMIT + 1; i++) {
			gamestate.playername[i] = 0;
		}
		clearcurrentgamesave();
		fwrite(&gamestate, sizeof(struct gamestate), 1, fgamestate);
		fclose(fgamestate);
	}
	else {
		fread(&gamestate, sizeof(struct gamestate), 1, fgamestate);
		fclose(fgamestate);
		/*printf("player:%s,id:%d\n", gamestate.playername,gamestate.id);
		printf("global:\n通关：%d,开场：%d,过关：%d,\n得分：\n%d,%d,%d,%d,%d\n", gamestate.clearance, gamestate.movie, gamestate.passlevel, gamestate.score[0], gamestate.score[1], gamestate.score[2], gamestate.score[3], gamestate.score[4]);
		printf("current:\n当前关：%d,当前分：%d,当前角度：%f,当前食物：%d，当前时间：%d\n", gamestate.currentlevel, gamestate.currentscore, gamestate.currentrad, gamestate.currenttotaleatfoods, gamestate.currentgametime);
		printf("currentbuff:R:%d,G:%d,B:%d\n", gamestate.currentbufftime.rtime, gamestate.currentbufftime.gtime, gamestate.currentbufftime.btime);
		printf("currentdebuff:chaos:%d,repel:%d,freeze:%d,invisiable:%d\n", gamestate.currentdebufftime.chaostime, gamestate.currentdebufftime.repelfoodtime, gamestate.currentdebufftime.freezetime, gamestate.currentdebufftime.invisibletime);*/
	}
}

void saveglobalgame() {
	FILE *fgamestate;
	fgamestate = fopen("gamestate.dat", "wb");
	fwrite(&gamestate, sizeof(struct gamestate), 1, fgamestate);
	fclose(fgamestate);
}

void savesnake() {
	struct snake *p;
	int n=0,i;
	FILE *fsnake;
	p = head;
	while (p != NULL) {
		n++;
		p = p->next;
	}
	fsnake=fopen("snake.dat", "w");
	fprintf(fsnake, "%d\n", n);
	p = head;
	for (i = 0; i < n; i++) {
		fprintf(fsnake, "%d,%d,%d,%d,%d,%d\n", p->x,p->y,p->state, (int)GetRValue(p->color), (int)GetGValue(p->color), (int)GetBValue(p->color));
		p = p->next;
	}
	fclose(fsnake);
}

void savefood() {
	struct food *p;
	int n = 0, i;
	FILE *ffood;
	p = foodhead;
	while (p != NULL) {
		n++;
		p = p->next;
	}
	ffood=fopen("food.dat", "w");
	fprintf(ffood, "%d\n", n);
	p = foodhead;
	for (i = 0; i < n; i++) {
		fprintf(ffood, "%d,%d,%d,%d,%d,%d\n", p->x, p->y,p->size, (int)GetRValue(p->color), (int)GetGValue(p->color), (int)GetBValue(p->color));
		p = p->next;
	}
	fclose(ffood);
}

void savecurrentgamestate(int level) {
	FILE *fgamestate;
	fgamestate = fopen("gamestate.dat", "wb");
	gamestate.currentlevel = level;
	fwrite(&gamestate, sizeof(struct gamestate), 1, fgamestate);
	fclose(fgamestate);
}

void clearcurrentgamesave() {
	FILE *fgamestate;
	fgamestate = fopen("gamestate.dat", "wb");
	gamestate.currentlevel = 0;
	gamestate.currentscore = 0;
	gamestate.currentrad = 0;
	gamestate.currenttotaleatfoods = 0;
	gamestate.currentgametime = 0;
	gamestate.currentbufftime.rtime = gamestate.currentbufftime.gtime = gamestate.currentbufftime.btime = gamestate.currentdebufftime.chaostime = 0;
	gamestate.currentdebufftime.chaostime = gamestate.currentdebufftime.freezetime = gamestate.currentdebufftime.repelfoodtime =gamestate.currentdebufftime.invisibletime= 0;
	fwrite(&gamestate, sizeof(struct gamestate), 1, fgamestate);
	fclose(fgamestate);
}
void resetgame() {
	FILE *fgamestate;
	int i;
	fgamestate = fopen("gamestate.dat", "wb");
	gamestate.movie = 0;
	gamestate.clearance = 0;
	gamestate.id = 0;
	gamestate.passlevel = 0;
	for (i = 0; i < 5; i++) {
		gamestate.score[i] = 0;
	}
	for (i = 0; i < NAMELIMIT+1; i++) {
		gamestate.playername[i] = 0;
	}
	clearcurrentgamesave();
	fwrite(&gamestate, sizeof(struct gamestate), 1, fgamestate);
	fclose(fgamestate);
	exit(0);
}
int judgesnake(int level) {
	int x, y;
	x = head->x;
	y = head->y;
	if (x >= WIDTH || x <= 0 || y >= HEIGHT || y <= 0) {
		return DEATH;
	}
	if (snakelength < 3) {
		return BOOMDEATH;
	}
	if (level == 1) {
		if (x >= 153 - 10 && x <= 179 + 10 && y >= 286 - 10 && y <= 312 + 10 || x >= 628 - 10 && x <= 654 + 10&& y >= 286 - 10 && y <= 312 + 10) {
			return DEATH;
		}
		if (snakelength == LEVEL1WINNEEDLENGTH|| GetKeyState('M') < 0) {
			if (gamestate.clearance == 0) {
				return WIN;
			}
			else {
				return CLEARANCE;
			}
		}
	}
	else if (level == 2) {
		struct snake *p = head;
		int n=0;
		double mo;
		double absmo;
		settlescore = 0;
		while (p != NULL) {
			mo = (p->x - 400)*(p->x - 400) + (p->y - 300)*(p->y - 300);
			if ( mo<= 58564 && mo>= 50716) {
				n++;
				absmo = mo - 54730.0;
				if (absmo  < 0) {
					absmo *= -1;
				}
				settlescore += (int)(1000 * (1 - absmo/ 4194));
			}
			p = p->next;
		}
		if (n == LEVEL2WINNEEDLENGTH|| GetKeyState('M') < 0) {
			gamestate.currentscore += settlescore;
			settlescore = 0;
			if (gamestate.clearance == 0) {
				return WIN;
			}
			else {
				return CLEARANCE;
			}
		}
		if (gamestate.currentgametime >= LEVEL2TIMELIMIT) {
			return BOOMDEATH;
		}
	}
	else if (level == 3) {
		if (gamestate.clearance == 0) {
			return WIN;
		}
		else {
			return CLEARANCE;
		}
	}
	else if (level == 4) {
		if (gamestate.clearance == 0) {
			return WIN;
		}
		else {
			return CLEARANCE;
		}
	}
	else if (level == 5) {
		return CLEARANCE;
	}
	return NORMAL;
}
void death(int level) {
	BeginBatchDraw();
	struct snake *p = tail;
	while (p->previous->previous->previous != NULL) {
		p->x += rand() % PY*2 - PY;
		p->y += rand() % PY * 2 - PY;
		p->state = DEATH;
		printworld(level);
		FlushBatchDraw();
		Sleep(12);
		p = p->previous;
	}
	EndBatchDraw();
}
void printmainUI(int level) {
	//-------------字体设置------------//
	LOGFONT winfont;
	gettextstyle(&winfont);                     // 获取当前字体设置
	winfont.lfHeight = 20;                      // 设置字体高度
	_tcscpy_s(winfont.lfFaceName, _T("方正舒体"));//设置字体
	winfont.lfQuality = ANTIALIASED_QUALITY;    // 设置输出效果为抗锯齿  
	winfont.lfWeight = FW_BOLD;
	settextstyle(&winfont);
	settextcolor(BLACK);
	//---------------------------------//
	char s[15];
	if (level == 1) {
		sprintf(s, "%d", gamestate.currentscore);
	}
	else if (level == 2) {
		if (settlescore != 0) {
			sprintf(s, "%d + %d", gamestate.currentscore, settlescore);
		}
		else {
			sprintf(s, "%d", gamestate.currentscore);
		}
	}
	else {
		sprintf(s, "%d", gamestate.currentscore);
	}
	IMAGE progress_a;
	IMAGE progress_b;
	IMAGE uihead_a;
	IMAGE uihead_b;
	IMAGE lotus_a;
	IMAGE lotus_b;
	IMAGE red[3];
	IMAGE green[3];
	IMAGE blue[3];
	IMAGE debuff[4][2];
	loadimage(&red[0], _T("IMAGE"), _T("red_a"),25,25);
	loadimage(&red[1], _T("IMAGE"), _T("red_b"), 25, 25);
	loadimage(&red[2], _T("IMAGE"), _T("red_d"), 25, 25);
	loadimage(&green[0], _T("IMAGE"), _T("green_a"), 25, 25);
	loadimage(&green[1], _T("IMAGE"), _T("green_b"), 25, 25);
	loadimage(&green[2], _T("IMAGE"), _T("green_d"), 25, 25);
	loadimage(&blue[0], _T("IMAGE"), _T("blue_a"), 25, 25);
	loadimage(&blue[1], _T("IMAGE"), _T("blue_b"), 25, 25);
	loadimage(&blue[2], _T("IMAGE"), _T("blue_d"), 25, 25);
	loadimage(&debuff[0][0], _T("IMAGE"), _T("chaosui_a"));
	loadimage(&debuff[0][1], _T("IMAGE"), _T("chaosui_b"));
	loadimage(&debuff[1][0], _T("IMAGE"), _T("freezeui_a"));
	loadimage(&debuff[1][1], _T("IMAGE"), _T("freezeui_b"));
	loadimage(&debuff[2][0], _T("IMAGE"), _T("repelui_a"));
	loadimage(&debuff[2][1], _T("IMAGE"), _T("repelui_b"));
	loadimage(&debuff[3][0], _T("IMAGE"), _T("invisibleui_a"));
	loadimage(&debuff[3][1], _T("IMAGE"), _T("invisibleui_b"));
	loadimage(&progress_a, _T("IMAGE"), _T("progress_a"));
	loadimage(&progress_b, _T("IMAGE"), _T("progress_b"));
	loadimage(&uihead_a, _T("IMAGE"), _T("uihead_a"));
	loadimage(&uihead_b, _T("IMAGE"), _T("uihead_b"));
	loadimage(&lotus_a, _T("IMAGE"), _T("lotus_a"));
	loadimage(&lotus_b, _T("IMAGE"), _T("lotus_b"));
	putimage(40, 36, &progress_a, SRCAND);
	putimage(40, 36, &progress_b, SRCPAINT);
	int rx=40, gx=75, bx=110, y=52;
	putimage(rx, y, &red[0], SRCAND);
	putimage(gx, y, &green[0], SRCAND);
	putimage(bx, y, &blue[0], SRCAND);
	outtextxy(210, 28, s);
	int progressframelenth = 154;
	int n = 0;
	double progress;
	if (level == 1) {
		progress =(( snakelength - 3 + (double)gamestate.currenttotaleatfoods / (double)growneedfoods ))/ (double)(LEVEL1WINNEEDLENGTH-3);
	}
	else if (level == 2) {
		struct snake *p = head;
		while (p != NULL) {
			if ((p->x - 400)*(p->x - 400) + (p->y - 300)*(p->y - 300) <= 58564 && (p->x - 400)*(p->x - 400) + (p->y - 300)*(p->y - 300) >= 50716) {
				n++;
			}
			p = p->next;
		}
		progress = (n / LEVEL2WINNEEDLENGTH);
	}
	else {
		progress = 1;
	}
	putimage(34 + (int)(progressframelenth* progress), 28, &uihead_a, SRCAND);
	putimage(34 + (int)(progressframelenth* progress), 28, &uihead_b, SRCPAINT);
	if (level == 2) {
		if (gamestate.currentgametime <= LEVEL2TIMELIMIT / 5.0) {
			putimage(145, 55, &lotus_a, SRCAND);
			putimage(145, 55, &lotus_b, SRCPAINT);
		}
		else if (gamestate.currentgametime > LEVEL2TIMELIMIT / 5.0 && gamestate.currentgametime <= LEVEL2TIMELIMIT / 5.0*2) {
			if (gamestate.currentgametime / 20 % 2 != 0) {
				putimage(145, 55, &lotus_a, SRCAND);
				putimage(145, 55, &lotus_b, SRCPAINT);
			}
		}
		else if (gamestate.currentgametime > LEVEL2TIMELIMIT / 5.0 * 2 && gamestate.currentgametime <= LEVEL2TIMELIMIT / 5.0*3) {
			if (gamestate.currentgametime / 10 % 2 != 0) {
				putimage(145, 55, &lotus_a, SRCAND);
				putimage(145, 55, &lotus_b, SRCPAINT);
			}
		}
		else if (gamestate.currentgametime > LEVEL2TIMELIMIT / 5.0 * 3 && gamestate.currentgametime <= LEVEL2TIMELIMIT/5.0*4) {
			if (gamestate.currentgametime / 5 % 2 != 0) {
				putimage(145, 55, &lotus_a, SRCAND);
				putimage(145, 55, &lotus_b, SRCPAINT);
			}
		}
		else if(gamestate.currentgametime > LEVEL2TIMELIMIT / 5.0 * 4 && gamestate.currentgametime <= LEVEL2TIMELIMIT) {
			if (gamestate.currentgametime / 2 % 2 != 0) {
				putimage(145, 55, &lotus_a, SRCAND);
				putimage(145, 55, &lotus_b, SRCPAINT);
			}
		}
	}
	//----------buff-----------//
	if (gamestate.currentbufftime.rtime > 0) {
		if (gamestate.currentbufftime.rtime >= RTIME / 5 || gamestate.currentbufftime.rtime / 5 % 2 == 0) {
			putimage(rx, y, &red[1], SRCPAINT);
		}
		else {
			putimage(rx, y, &red[2], SRCPAINT);
		}
	}
	else {
		putimage(rx, y, &red[2], SRCPAINT);
	}
	if (gamestate.currentbufftime.gtime > 0) {
		if (gamestate.currentbufftime.gtime >= GTIME / 5 || gamestate.currentbufftime.gtime / 5 % 2 == 0) {
			putimage(gx, y, &green[1], SRCPAINT);
		}
		else {
			putimage(gx, y, &green[2], SRCPAINT);
		}
	}
	else {
		putimage(gx, y, &green[2], SRCPAINT);
	}
	if (gamestate.currentbufftime.btime > 0) {
		if (gamestate.currentbufftime.btime >= BTIME / 5 || gamestate.currentbufftime.btime / 5 % 2 == 0) {
			putimage(bx, y, &blue[1], SRCPAINT);
		}
		else {
			putimage(bx, y, &blue[2], SRCPAINT);
		}
	}
	else {
		putimage(bx, y, &blue[2], SRCPAINT);
	}
	//-------debuff--------//
	struct debuffui {
		short type;
		short time;
	}*pdebuff,temp;
	const int debuffn = 4;
	int i,j;
	int debufftime[debuffn];
	debufftime[0] = gamestate.currentdebufftime.chaostime;
	debufftime[1] = gamestate.currentdebufftime.freezetime;
	debufftime[2] = gamestate.currentdebufftime.repelfoodtime;
	debufftime[3] = gamestate.currentdebufftime.invisibletime;
	pdebuff = (struct debuffui *)malloc(debuffn * sizeof(struct debuffui));
	for (i = 0; i < debuffn; i++) {
		(pdebuff + i)->type = i;
		(pdebuff + i)->time = debufftime[i];
	}
	//------排序------//
	for (i = 0; i < debuffn - 1; i++) {
		for (j = 0; j < debuffn - 1; j++) {
			if ((pdebuff + j)->time > (pdebuff + j + 1)->time) {
				temp = *(pdebuff + j + 1);
				*(pdebuff + j + 1) = *(pdebuff + j);
				*(pdebuff + j) = temp;
			}
		}
	}
	//------排序------//
	//------显示------//
	int dx[4] = {40,65,90,115},dy = 82;
	int putn=0;
	for (i = 0; i < debuffn; i++) {
		if ((pdebuff + i)->time != 0) {
			if ((pdebuff + i)->time >= CHAOSTIME / 5 || ((pdebuff + i)->time) / 5 % 2 == 0) {
				putimage(dx[putn], dy, &debuff[(pdebuff + i)->type][0], SRCAND);
				putimage(dx[putn], dy, &debuff[(pdebuff + i)->type][1], SRCPAINT);
			}
			putn++;
		}
	}
}

void updatesnakelength() {
	struct snake *p = head;
	snakelength = 0;
	while (p != NULL) {
		snakelength++;
		p = p->next;
	}
}

bool judgecreate(int x,int y,int level) {
	if (level == 1) {
		if (x >= 153 - 10 && x <= 179 + 10 && y >= 286 - 10 && y <= 312 + 10 || x >= 628 - 10 && x <= 654 + 10 && y >= 286 - 10 && y <= 312 + 10) {
			return 1;
		}
		else {
			return 0;
		}
	}
	else if (level == 2) {
		if ((x - 400)*(x - 400) + (y - 300)*(y - 300) <= 22801) {
			return 1;
		}
		else {
			return 0;
		}
	}
	else {
		return 0;
	}
}

void saverank() {
	FILE *frank;
	frank = fopen("rank.dat", "rb");
	if (frank == NULL) {
		clearrank();
		frank = fopen("rank.dat", "wb");
		fclose(frank);
	}
	else {
		fread(&rank, sizeof(struct rank), 1, frank);
		fclose(frank);
	}
	int sum = 0, i;
	for (i = 0; i < 5; i++) {
		rank.score[9][i] = gamestate.score[i];
		sum += gamestate.score[i];
	}
	rank.totalscore[9] = sum;
	strcpy(rank.name[9], gamestate.playername);
	rank.id[9] = gamestate.id;
	frank = fopen("rank.dat", "wb");
	fwrite(&rank, sizeof(struct rank), 1, frank);
	fclose(frank);
}
void clearrank() {
	int i,j;
	for (i = 0; i < 10; i++) {
		for (j = 0; j < NAMELIMIT + 1; j++) {
			rank.name[i][j] = 0;
		}
		for (j = 0; j < 5; j++) {
			rank.score[i][j] = 0;
		}
		rank.totalscore[i] = 0;
		rank.id[i] = 0;
	}
	FILE *frank;
	frank = fopen("rank.dat", "wb");
	fwrite(&rank, sizeof(struct rank), 1, frank);
	fclose(frank);
}
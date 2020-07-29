#include<graphics.h>
#include<iostream>
#include<ctime>
#include<conio.h>
#include<mmsystem.h>
#pragma comment(lib,"winmm.lib")
#define width 400
#define height 300
using namespace std;
int direct = 3;

struct Food
{
	int x;
	int y;
};
Food food;
bool IsFood = true;//是否产生食物
bool Isover = false;//游戏是否结束
bool Ischange = true;//方向是否可以改变
struct Body
{
	int x;
	int y;
};
Body snack[1200];
int len;//蛇长度

void addfood();
void move();
void paint();
DWORD WINAPI ThreadProc(LPVOID lParam);
DWORD WINAPI playmusic(LPVOID lpvoid);

int main()
{
	mciSendString("open 平静的弹性环.mp3", 0, 0, 0);
	mciSendString("play 平静的弹性环.mp3 repeat", 0, 0, 0);
	srand((unsigned int)time(NULL));
	initgraph(width, height);
	snack[0].x = 0;
	snack[0].y = 0;
	len = 1;
	addfood();
	CreateThread(0, 0, ThreadProc, 0, 0, 0);
	while (!Isover)
	{
		paint();
		move();
		Sleep(100);
	}
	closegraph();
	return 0;
}

void addfood()
{
	if (!IsFood)
		return;
	IsFood = false;
	bool b = false;
	do
	{
		food.x = rand() % (width / 10);
		food.y = rand() % (height / 10);
		for (int i = 0; i < len; i++)
		{
			if (food.x == snack[i].x && food.y == snack[i].y)
			{
				b = true;
				break;
			}
			else
				b = false;
		}
	} while (b);
}


DWORD WINAPI playmusic(LPVOID lpvoid)
{
	mciSendString("open 伐木.mp3 alias music", 0, 0, 0);
	mciSendString("play music wait", 0, 0, 0);
	mciSendString("close music", 0, 0, 0);
	return 0;
}

void move()
{
	int px = 0;
	int py = 0;
	switch (direct)
	{
	case 0:
		py -= 1;
		break;
	case 1:
		py += 1;
		break;
	case 2:
		px -= 1;
		break;
	case 3:
		px += 1;
		break;
	}
	Body head;
	head.x = snack[0].x + px;
	head.y = snack[0].y + py;
	for (int i = len - 1; i > 0; i--)
	{
		snack[i] = snack[i - 1];
		if (head.x == snack[i].x && head.y == snack[i].y)
			Isover = true;
	}
	
	snack[0].x += px;
	snack[0].y += py;
	if (snack[0].x < 0)
		snack[0].x = 39;
	if (snack[0].x > 39)
		snack[0].x = 0;
	if (snack[0].y < 0)
		snack[0].y = 29;
	if (snack[0].y > 29)
		snack[0].y = 0; 
	if (snack[0].x == food.x && snack[0].y == food.y)
	{
		CreateThread(0, 0, playmusic, 0, 0, 0);
		snack[len].x = -1;
		snack[len].y = -1;
		len++;
		IsFood = true;
		//food.x = -1;
		//food.y = -1;
	}
	addfood();
	Ischange = true; 
}

void paint()
{
	IMAGE img(400, 300);
	loadimage(&img, "1.jpg");
	SetWorkingImage(&img);
	//setfillcolor(RGB(255, 255, 255));
	//solidrectangle(0, 0, 400, 300);
	setfillcolor(RGB(0,255,0));
	for (int i = 0; i < len; i++)
	{
		fillrectangle(snack[i].x * 10, snack[i].y * 10, snack[i].x * 10 + 10, snack[i].y * 10 + 10);
	}

	setfillcolor(RGB(255, 0, 0));
	fillrectangle(food.x * 10, food.y * 10, food.x * 10 + 10, food.y * 10 + 10);
	SetWorkingImage();
	putimage(0, 0, &img);  
}


DWORD WINAPI ThreadProc(LPVOID lParam)
{
	while (!Isover)
	{
		char ch = _getch();
		if (!Ischange)
			continue;
		switch(ch)
		{
		case 'W':
		case 'w':
			if (direct == 1)
				break;
			direct = 0;
			Ischange = false;
			break;
		case'S':
		case's':
			if (direct == 0)
				break;
			direct = 1;
			Ischange = false;
			break;
		case'A':
		case'a':
			if (direct == 3)
				break;
			direct = 2;
			Ischange = false;
			break;
		case'D':
		case'd':
			if (direct == 2)
				break;
			direct = 3;
			Ischange = false;
			break;
		}
	}
	return 0;
}
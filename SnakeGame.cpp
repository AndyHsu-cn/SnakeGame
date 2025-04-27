#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <easyx.h>
#include <conio.h>
typedef struct
{
	int x;
	int y;
}Node;
enum dir {
	UP, DOWN, LEFT, RIGHT
};
void paintGrid(int width, int height);
void paintSnake(Node snake[], int cur_len);
Node snakeMove(Node snake[], int cur_len, int dir);
Node createFood(Node snake[], int cur_len);
void paintFood(Node food);
void changeDir(int* pdir);
bool isGameOver(Node snake[], int cur_len);
void paintScore(int cur_score, int max_score);
int loadMaxScore();
void saveMaxScore(int score);
const int WIDTH = 800;
const int HEIGHT = 600;
const int GRID_WIDTH = 40;
const int MAX_LEN = 100;
int main(void)
{
	Node snake[MAX_LEN] = { {5,7}, {4,7}, {3,7} , {2,7} , {1,7}};
	Node food = { 12, 7 };
	int cur_len = 5;
	Node cur_tail = snake[cur_len - 1];
	int cur_dir = RIGHT;
	int cur_score = 0;
	int max_score = 0;
	
	max_score = loadMaxScore();
	initgraph(WIDTH + 200, HEIGHT);
	setbkcolor(0xFFFF55);
	srand(time(NULL));

	while (1)
	{
		cleardevice();
		paintGrid(WIDTH, HEIGHT);
		paintSnake(snake, cur_len);
		paintFood(food);
		paintScore(cur_score, max_score);
		changeDir(&cur_dir);
		cur_tail = snakeMove(snake, cur_len, cur_dir);
		if (snake[0].x == food.x && snake[0].y == food.y)
		{
			if (cur_len < 100)
			{
				snake[cur_len] = cur_tail;
				cur_len++;
				cur_score += 100;
			}
			food = createFood(snake, cur_len);
		}
		if (isGameOver(snake, cur_len))
		{
			settextstyle(48, 0, _T("宋体"));
			outtextxy(300, 250, _T("Game Over!"));
			if (cur_score > max_score)
				saveMaxScore(cur_score);
			_getch();
			closegraph();
			return 0;
		}
		Sleep(500);
	}
	closegraph();

	return 0;
}

void paintGrid(int width, int height)
{
	for (int y = 0; y <= HEIGHT; y += GRID_WIDTH)
		line(0, y, 800, y);
	for (int x = 0; x <= WIDTH; x += GRID_WIDTH)
		line(x, 0, x, 600);
}

void paintSnake(Node snake[], int cur_len)
{
	for (int i = 0; i < cur_len; i++)
	{
		int left = snake[i].x * GRID_WIDTH;
		int top = snake[i].y * GRID_WIDTH;
		int right = (snake[i].x + 1) * GRID_WIDTH;
		int bottom = (snake[i].y + 1) * GRID_WIDTH;
		solidrectangle(left, top, right, bottom);
	}
}

Node snakeMove(Node snake[], int cur_len, int dir)
{
	Node tail = snake[cur_len - 1];

	for (int i = cur_len - 1; i > 0; i--)
		snake[i] = snake[i - 1];
	switch (dir)
	{
	case UP:
		snake[0].y--;
		break;
	case DOWN:
		snake[0].y++;
		break;
	case LEFT:
		snake[0].x--;
		break;
	case RIGHT:
		snake[0].x++;
		break;
	
	}
	
	return tail;
}

void changeDir(int* pdir)
{
	if (_kbhit() != 0)
	{
		char ch = _getch();
		switch (ch)
		{
		case 'w':
			if (*pdir != DOWN)
				*pdir = UP;
			break;
		case 's':
			if (*pdir != UP)
				*pdir = DOWN;
			break;
		case 'a':
			if (*pdir != RIGHT)
				*pdir = LEFT;
			break;
		case 'd':
			if (*pdir != LEFT)
				*pdir = RIGHT;
			break;
		}
	}
}

Node createFood(Node snake[], int cur_len)
{
	// 要求1：产生位置不越界
	// 要求2：产生位置不能在蛇身上
	Node food;

	while (1)
	{
		int i = 0;
		food.x = rand() % (WIDTH / GRID_WIDTH);
		food.y = rand() % (HEIGHT / GRID_WIDTH);
		for ( ; i < cur_len; i++)
		{
			if (snake[i].x == food.x && snake[i].y == food.y)
				break;
		}
		if (i < cur_len)
			continue;
		else
			break;
	}

	return food;
}

void paintFood(Node food)
{
	int left = food.x * GRID_WIDTH;
	int top = food.y * GRID_WIDTH;
	int right = (food.x + 1) * GRID_WIDTH;
	int bottom = (food.y + 1) * GRID_WIDTH;

	setfillcolor(YELLOW);
	solidrectangle(left, top, right, bottom);
	setfillcolor(WHITE);
}

bool isGameOver(Node snake[], int cur_len)
{
	if (snake[0].x < 0 || snake[0].x > 19 || snake[0].y < 0 || snake[0].y > 14)
		return true;
	for (int i = 1; i < cur_len; i++)
		if (snake[0].x == snake[i].x && snake[0].y == snake[i].y)
			return true;
	return false;
}

void paintScore(int cur_score, int max_score)
{
	const char* pScore		= "Score: ";
	const char* pMaxScore = "Max  : ";
	char scoreStr[20];						// 存储格式化后的字符串
	char maxScoreStr[20];

	// 将分数转换为字符串
	sprintf_s(scoreStr, "%s %d", pScore, cur_score);
	sprintf_s(maxScoreStr, "%s %d", pMaxScore, max_score);

	settextcolor(BLACK);
	settextstyle(24, 0, _T("宋体"));			// 高度36，宽度自适应，字体为宋体
	outtextxy(820, 50, _T(scoreStr));		// 使用 _T() 宏兼容 Unicode
	outtextxy(820, 100, _T(maxScoreStr));
}

int loadMaxScore() {
	FILE* fp;
	int max_score = 0;

	fopen_s(&fp, "score.txt", "r");
	if (fp != NULL) {
		fscanf_s(fp, "%d", &max_score);
		fclose(fp);
	}
	return max_score;
}

void saveMaxScore(int score) {
	FILE* fp;

	fopen_s(&fp, "score.txt", "w");
	if (fp != NULL) {
		fprintf_s(fp, "%d", score);
		fclose(fp);
	}
}
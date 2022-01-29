#include <iostream>
#include <Windows.h>
#include <random>
#include <stdlib.h>
#include <chrono>


using namespace std;


enum TCell : char {
	CROSS = 'X',
	ZERO = '0',
	EMPTY = '_'
};

enum TProgress {
	IN_PROGRESS,
	WON_HUMAN,
	WON_AI,
	DRAW
};


struct TCoord {
	size_t x{ 0U };
	size_t y{ 0U };
};

struct TGame {
	TCell** ppField = nullptr;
	const size_t SIZE{ 5 };
	TCell human;
	TCell ai;
	TProgress progress{ IN_PROGRESS };
	size_t turn{ 0U };
};

inline void clrscr()
{
	system("CLS");
}

int32_t __fastcall getRandomNum(int32_t min, int32_t max)
{
	const static auto seed = chrono::system_clock::now().time_since_epoch().count();
	static mt19937_64 generator(seed);
	uniform_int_distribution<int32_t> dis(min, max);
	return dis(generator);
}

void __fastcall initGame(TGame& g)
{
	g.ppField = new TCell * [g.SIZE];
	for (size_t i = 0; i < g.SIZE; i++)
	{
		g.ppField[i] = new TCell[g.SIZE];
	}

	for (size_t x = 0; x < g.SIZE; x++)
	{
		for (size_t y = 0; y < g.SIZE; y++)
		{
			g.ppField[x][y] = EMPTY;
		}
	}

	if (getRandomNum(0, 1000) > 500)
	{
		g.human = CROSS;
		g.ai = ZERO;
		g.turn = 0;
	}
	else
	{
		g.human = ZERO;
		g.ai = CROSS;
		g.turn = 1;
	}
}

void __fastcall deinitGame(TGame& g)
{
	for (size_t i = 0; i < g.SIZE; i++)
	{
		delete[] g.ppField[i];
	}
	delete[] g.ppField;
	g.ppField = nullptr;
}

void __fastcall drawGame(const TGame& g)
{
	cout << "     ";
	for (size_t y = 0; y < g.SIZE; y++)
		cout << y + 1 << "   ";
	cout << endl;
	for (size_t x = 0; x < g.SIZE; x++)
	{
		cout << " " << x + 1 << " | ";
		for (size_t y = 0; y < g.SIZE; y++)
		{
			cout << g.ppField[x][y] << " | ";
		}
		cout << endl;
	}
	cout << endl;

	cout << " Human: " << g.human << endl << " Computer: " << g.ai << endl << endl;
}

void __fastcall congrats(const TGame& g)
{
	if (g.progress == WON_HUMAN)
		cout << "Human won! =)" << endl;
	else if (g.progress == WON_AI)
		cout << "Computer won! =/" << endl;
	else if (g.progress == DRAW)
		cout << "Draw! =(" << endl;
}

TProgress getWon(const TGame& g)
{
	for (size_t x = 0; x < g.SIZE; x++)
	{
		if (g.ppField[x][0] == g.ppField[x][1] && g.ppField[x][0] == g.ppField[x][2] 
			&& g.ppField[x][0] == g.ppField[x][3] && g.ppField[x][0] == g.ppField[x][4])
		{
			if (g.ppField[x][0] == g.human)
				return WON_HUMAN;

			if (g.ppField[x][0] == g.ai)
				return WON_AI;
		}
	}

	for (size_t y = 0; y < g.SIZE; y++)
	{
		if (g.ppField[0][y] == g.ppField[1][y] && g.ppField[0][y] == g.ppField[2][y] 
			&& g.ppField[0][y] == g.ppField[3][y] && g.ppField[0][y] == g.ppField[4][y])
		{
			if (g.ppField[0][y] == g.human)
				return WON_HUMAN;

			if (g.ppField[0][y] == g.ai)
				return WON_AI;
		}
	}

	if (g.ppField[0][0] == g.ppField[1][1] && g.ppField[0][0] == g.ppField[2][2] 
		&& g.ppField[0][0] == g.ppField[3][3] && g.ppField[0][0] == g.ppField[4][4])
	{
		if (g.ppField[1][1] == g.human)
			return WON_HUMAN;

		if (g.ppField[1][1] == g.ai)
			return WON_AI;
	}

	if (g.ppField[4][0] == g.ppField[2][2] && g.ppField[2][2] == g.ppField[0][4] 
		&& g.ppField[3][1] == g.ppField[2][2] && g.ppField[2][2] == g.ppField[1][3])
	{
		if (g.ppField[2][2] == g.human)
			return WON_HUMAN;

		if (g.ppField[2][2] == g.ai)
			return WON_AI;
	}

	bool draw = true;
	for (size_t x = 0; x < g.SIZE; x++)
	{
		for (size_t y = 0; y < g.SIZE; y++)
		{
			if (g.ppField[x][y] == EMPTY)
			{
				draw = false;
				break;
			}
		}
		if (!draw)
			break;
	}
	if (draw)
		return DRAW;

	return IN_PROGRESS;
}

TCoord getHumanCoord(const TGame& g)
{
	TCoord c;
	do {
		cout << "Enter X (1..3): ";
		cin >> c.x;
		cout << "Enter Y (1..3): ";
		cin >> c.y;

		c.x--;
		c.y--;
	} while (c.x > 4 || c.y > 4 || g.ppField[c.x][c.y] != EMPTY);

	return c;
}

TCoord getAICoord(TGame& g)
{
	for (size_t x = 0; x < g.SIZE; x++)
	{
		for (size_t y = 0; y < g.SIZE; y++)
		{
			if (g.ppField[x][y] == EMPTY)
			{
				g.ppField[x][y] = g.ai;
				if (getWon(g) == WON_AI)
				{
					g.ppField[x][y] = EMPTY;
					return { x, y };
				}
				g.ppField[x][y] = EMPTY;
			}
		}
	}

	for (size_t x = 0; x < g.SIZE; x++)
	{
		for (size_t y = 0; y < g.SIZE; y++)
		{
			if (g.ppField[x][y] == EMPTY)
			{
				g.ppField[x][y] = g.human;
				if (getWon(g) == WON_HUMAN)
				{
					g.ppField[x][y] = EMPTY;
					return { x, y };
				}
				g.ppField[x][y] = EMPTY;
			}
		}
	}

	if (g.ppField[2][2] == EMPTY)
		return { 2, 2 };

	TCoord buf[18];
	size_t num{ 0 };
	if (g.ppField[0][0] == EMPTY)
	{
		buf[num++] = { 0, 0 };
	}
	if (g.ppField[4][4] == EMPTY)
	{
		buf[num++] = { 4, 4 };
	}
	if (g.ppField[0][4] == EMPTY)
	{
		buf[num++] = { 0, 4 };
	}
	if (g.ppField[4][0] == EMPTY)
	{
		buf[num++] = { 4, 0 };
	}
	if (num > 0)
	{
		const size_t index = getRandomNum(0, 1000) % num;
		return buf[index];
	}

	num = 0;
	if (g.ppField[0][1] == EMPTY)
	{
		buf[num++] = { 0, 1 };
	}
	if (g.ppField[0][2] == EMPTY)
	{
		buf[num++] = { 0, 2 };
	}
	if (g.ppField[0][3] == EMPTY)
	{
		buf[num++] = { 0, 3 };
	}
	if (g.ppField[1][0] == EMPTY)
	{
		buf[num++] = { 1, 0 };
	}
	if (g.ppField[1][1] == EMPTY)
	{
		buf[num++] = { 1, 1 };
	}
	if (g.ppField[1][2] == EMPTY)
	{
		buf[num++] = { 1, 2 };
	}
	if (g.ppField[1][3] == EMPTY)
	{
		buf[num++] = { 1, 3 };
	}
	if (g.ppField[1][4] == EMPTY)
	{
		buf[num++] = { 1, 4 };
	}
	if (g.ppField[2][0] == EMPTY)
	{
		buf[num++] = { 2, 0 };
	}
	if (g.ppField[2][1] == EMPTY)
	{
		buf[num++] = { 2, 1 };
	}
	if (g.ppField[2][3] == EMPTY)
	{
		buf[num++] = { 2, 3 };
	}
	if (g.ppField[2][4] == EMPTY)
	{
		buf[num++] = { 2, 4 };
	}
	if (g.ppField[4][1] == EMPTY)
	{
		buf[num++] = { 4, 1 };
	}
	if (g.ppField[4][2] == EMPTY)
	{
		buf[num++] = { 4, 2 };
	}
	if (g.ppField[4][3] == EMPTY)
	{
		buf[num++] = { 4, 3 };
	}
	if (g.ppField[3][0] == EMPTY)
	{
		buf[num++] = { 3, 0 };
	}
	if (g.ppField[3][1] == EMPTY)
	{
		buf[num++] = { 3, 1 };
	}
	if (g.ppField[3][2] == EMPTY)
	{
		buf[num++] = { 3, 2 };
	}
	if (g.ppField[3][3] == EMPTY)
	{
		buf[num++] = { 3, 3 };
	}
	if (g.ppField[3][4] == EMPTY)
	{
		buf[num++] = { 3, 4 };
	}
	if (num > 0)
	{
		const size_t index = getRandomNum(0, 1000) % num;
		return buf[index];
	}
}

int main()
{

	setlocale(LC_ALL, "Russian");

	TGame g;
	initGame(g);
	clrscr();
	drawGame(g);

	do {

		if (g.turn % 2 == 0)
		{
			TCoord c = getHumanCoord(g);
			g.ppField[c.x][c.y] = g.human;
		}
		else
		{
			TCoord c = getAICoord(g);
			g.ppField[c.x][c.y] = g.ai;
		}

		g.turn++;
		clrscr();
		drawGame(g);
		g.progress = getWon(g);

	} while (g.progress == IN_PROGRESS);

	congrats(g);
	deinitGame(g);
	return 0;

}


#include "stdafx.h"
#include <iostream>
#include <string>
#include <stdlib.h>

char *StartingBoard[14] = {
	"++++++++++++++",
	"+............+",
	"+.*r_.r*.....+",
	"+...bbr......+",
	"+............+",
	"+............+",
	"+............+",
	"+............+",
	"+............+",
	"+............+",
	"+............+",
	"+............+",
	"+............+",
	"++++++++++++++"
};

class PointList {
public:
	PointList(int x, int y);
	PointList *Append(PointList *next);
	int x;
	int y;
	PointList *next;
};

PointList::PointList(int x, int y) {
	this->x = x;
	this->y = y;
	this->next = nullptr;
}

PointList *PointList::Append(PointList *newPt) {
	newPt->next = this;
	return newPt;
}

class GameHistory {
public:
	char *board[14];
	PointList *EL;
	PointList *sunEL;
	PointList *shaEL;
	GameHistory *next;
	GameHistory();
	GameHistory *Push(GameHistory *gh);
	GameHistory *Pop();
	void ShowBoard();
};

GameHistory::GameHistory() {
	this->board[0] = StartingBoard[0];
	this->board[1] = StartingBoard[1];
	this->board[2] = StartingBoard[2];
	this->board[3] = StartingBoard[3];
	this->board[4] = StartingBoard[4];
	this->board[5] = StartingBoard[5];
	this->board[6] = StartingBoard[6];
	this->board[7] = StartingBoard[7];
	this->board[8] = StartingBoard[8];
	this->board[9] = StartingBoard[9];
	this->board[10] = StartingBoard[10];
	this->board[11] = StartingBoard[11];
	this->board[12] = StartingBoard[12];
	this->board[13] = StartingBoard[13];
	this->EL = nullptr;
	this->sunEL = nullptr;
	this->shaEL = nullptr;
	this->next = nullptr;
}

GameHistory *GameHistory::Push(GameHistory *gh) {
	GameHistory *ghnew = new GameHistory;

	for (int i = 0; i < 14; i++) {
		char *row = (char *)malloc(15 * sizeof(char));
		strcpy_s(row, 15, gh->board[i]);
		ghnew->board[i] = row;
	}
	ghnew->next = gh;

	return ghnew;
}

GameHistory *GameHistory::Pop() {
	GameHistory *next = this->next;

	return next;
}

void GameHistory::ShowBoard() {
	printf("%s\n", this->board[0]);
	printf("%s\n", this->board[1]);
	printf("%s\n", this->board[2]);
	printf("%s\n", this->board[3]);
	printf("%s\n", this->board[4]);
	printf("%s\n", this->board[5]);
	printf("%s\n", this->board[6]);
	printf("%s\n", this->board[7]);
	printf("%s\n", this->board[8]);
	printf("%s\n", this->board[9]);
	printf("%s\n", this->board[10]);
	printf("%s\n", this->board[11]);
	printf("%s\n", this->board[12]);
	printf("%s\n", this->board[13]);
}
class Cursor {
public:
	Cursor();
	int x;
	int y;
	char dir;
};

Cursor::Cursor() {
	this->x = 0;
	this->y = 0;
	this->dir = 'n';
}

class Present {
public:
	Present();
	bool red;
	bool blu;
};

Present::Present() {
	this->red = false;
	this->blu = false;
}

class Points {
public:
	Points();
	int red;
	int blu;
};

Points::Points() {
	this->red = 0;
	this->blu = 0;
}

class Move {
public:
	Move();
	int scoredelta;
	char p1;
	int el1;
	char p2;
	int el2;
	char p3;
	int el3;
};

Move::Move() {
	this->scoredelta = 0;
	this->p1 = 'r';
	this->el1 = 0;
	this->p2 = '*';
	this->el2 = 0;
	this->p3 = '_';
	this->el3 = 0;
}

Cursor *cursor = new Cursor;

Points *thispiece = new Points;
Points *thismove = new Points;

char at(GameHistory *g) {
	return g->board[cursor->y][cursor->x];
}

void step() {
	if (cursor->dir == 'n') {
		cursor->y--;
	}
	else if (cursor->dir == 'e') {
		cursor->x++;
	}
	else if (cursor->dir == 'w') {
		cursor->x--;
	}
	else if (cursor->dir == 's') {
		cursor->y++;
	}
}

void reverse() {
	if (cursor->dir == 'n') {
		cursor->dir = 's';
	}
	else if (cursor->dir == 'e') {
		cursor->dir = 'w';
	}
	else if (cursor->dir == 'w') {
		cursor->dir = 'e';
	}
	else if (cursor->dir == 's') {
		cursor->dir = 'n';
	}
}

void bonuspts(Present *p) {
	if (p->red) {
		thispiece->red += 2;
	}
	else if (p->blu) {
		thispiece->blu += 2;
	}
}
void SkipRoofs(GameHistory *g, Present *p) {
	while ((at(g) == 'r') || (at(g) == 'b')) {
		if (at(g) == 'r') {
			p->red = true;
		}
		else if (at(g) == 'b') {
			p->blu = true;
		}
		step();
	}
}

void AddPointsFrom(GameHistory *g, int PointCt, Present *p, Points *pts) {
	while (at(g) == '_') {
		PointCt++;
		step();
	}
	if (p->red) {
		pts->red += PointCt;
	}
	if (p->blu) {
		pts->blu += PointCt;
	}
}

bool ShaFindSiesta(GameHistory *g, char dir, int x, int y, Points *p) {
	Present pres;

	cursor->dir = dir;
	cursor->x = x;
	cursor->y = y;

	do {
		step();
	} while (at(g) == '_');

	if ((at(g) != 'r') && (at(g) != 'b')) {
		return false;
	}

	SkipRoofs(g, &pres);

	if (at(g) == '*') {
		cursor->x = x;
		cursor->y = y;
		reverse();
		step();
		AddPointsFrom(g, 1, &pres, p);
		return true;
	}
	return false;
}

bool FindShaDouble(GameHistory *g, char dir, int x, int y, Present *pres) {
	cursor->dir = dir;
	cursor->x = x;
	cursor->y = y;

	do {
		step();
	} while (at(g) == '_');

	if ((at(g) != 'r') && (at(g) != 'b')) {
		return false;
	}

	SkipRoofs(g, pres);

	if (((pres->red) && (pres->blu)) || at(g) != '*') {
		return false;
	}

	cursor->x = x;
	cursor->y = y;
	reverse();

	do {
		step();
	} while (at(g) == '_');

	if ((at(g) != 'r') && (at(g) != 'b')) {
		return false;
	}

	SkipRoofs(g, pres);

	if (((pres->red) && (pres->blu)) || at(g) != '*') {
		return false;
	}

	return true;
}

void FindShaPoints(GameHistory *g, int x, int y, Points *p) {
	Present pres;

	ShaFindSiesta(g, 'n', x, y, p);
	ShaFindSiesta(g, 'e', x, y, p);
	ShaFindSiesta(g, 'w', x, y, p);
	ShaFindSiesta(g, 's', x, y, p);

	pres.red = false;
	pres.blu = false;
	if (FindShaDouble(g, 'n', x, y, &pres)) {
		bonuspts(&pres);
	}
	pres.red = false;
	pres.blu = false;
	if (FindShaDouble(g, 'e', x, y, &pres)) {
		bonuspts(&pres);
	}
}

void SunFindSiesta(GameHistory *g, char dir, int x, int y, Points *p) {
	Present pres;

	cursor->dir = dir;
	cursor->x = x;
	cursor->y = y;

	step();
	SkipRoofs(g, &pres);

	if (at(g) != '_') {
		return;
	}

	AddPointsFrom(g, 0, &pres, p);
}

bool FindSunDouble(GameHistory *g, char dir, int x, int y, Present *pres) {
	Points p;

	cursor->dir = dir;
	cursor->x = x;
	cursor->y = y;

	step();
	SkipRoofs(g, pres);

	if (((pres->red) && (pres->blu)) || (at(g) != '_')) {
		return false;
	}

	AddPointsFrom(g, 0, pres, &p);

	SkipRoofs(g, pres);

	if (((pres->red) && (pres->blu)) || (at(g) != '*')) {
		return false;
	}

	return true;
}

void FindSunPoints(GameHistory *g, int x, int y, Points *p) {
	Present pres;

	SunFindSiesta(g, 'n', x, y, p);
	SunFindSiesta(g, 'e', x, y, p);
	SunFindSiesta(g, 'w', x, y, p);
	SunFindSiesta(g, 's', x, y, p);

	pres.red = false;
	pres.blu = false;
	if (FindSunDouble(g, 'n', x, y, &pres)) {
		bonuspts(&pres);
	}
	pres.red = false;
	pres.blu = false;
	if (FindSunDouble(g, 'e', x, y, &pres)) {
		bonuspts(&pres);
	}
	pres.red = false;
	pres.blu = false;
	if (FindSunDouble(g, 'w', x, y, &pres)) {
		bonuspts(&pres);
	}
	pres.red = false;
	pres.blu = false;
	if (FindSunDouble(g, 's', x, y, &pres)) {
		bonuspts(&pres);
	}
}

void RoofFindSiesta(GameHistory *g, char dir, int x, int y, Present *pres, Points *p) {
	Present tpres;

	cursor->dir = dir;
	cursor->x = x;
	cursor->y = y;
	tpres.red = pres->red;
	tpres.blu = pres->blu;
	
	step();
	SkipRoofs(g, &tpres);

	if (at(g) != '*') {
		return;
	}

	cursor->x = x;
	cursor->y = y;
	reverse();

	step();
	SkipRoofs(g, &tpres);

	if (at(g) != '_') {
		return;
	}

	AddPointsFrom(g, 0, &tpres, p);
}

bool FindRoofDouble(GameHistory *g, char dir, int x, int y, Present *pres) {
	Points p;

	cursor->dir = dir;
	cursor->x = x;
	cursor->y = y;

	step();
	SkipRoofs(g, pres);

	if ((pres->red && pres->blu) || (at(g) != '*')) {
		return false;
	}

	cursor->x = x;
	cursor->y = y;
	reverse();

	step();
	SkipRoofs(g, pres);

	if ((pres->red && pres->blu) || (at(g) != '_')) {
		return false;
	}

	AddPointsFrom(g, 0, pres, &p);

	SkipRoofs(g, pres);

	if ((pres->red && pres->blu) || (at(g) != '*')) {
		return false;
	}

	return true;
}

void FindRoofPoints(GameHistory *g, int x, int y, Points *p, Present *pres) {
	Present tpres;
	
	RoofFindSiesta(g, 'n', x, y, pres, p);
	RoofFindSiesta(g, 'e', x, y, pres, p);
	RoofFindSiesta(g, 'w', x, y, pres, p);
	RoofFindSiesta(g, 's', x, y, pres, p);

	tpres.red = pres->red;
	tpres.blu = pres->blu;
	if (FindRoofDouble(g, 'n', x, y, &tpres)) {
		bonuspts(&tpres);
	}
	tpres.red = pres->red;
	tpres.blu = pres->blu;
	if (FindRoofDouble(g, 'e', x, y, &tpres)) {
		bonuspts(&tpres);
	}
	tpres.red = pres->red;
	tpres.blu = pres->blu;
	if (FindRoofDouble(g, 'w', x, y, &tpres)) {
		bonuspts(&tpres);
	}
	tpres.red = pres->red;
	tpres.blu = pres->blu;
	if (FindRoofDouble(g, 's', x, y, &tpres)) {
		bonuspts(&tpres);
	}
}

bool HasPieceAdj(GameHistory *g, int x, int y) {
	if ((x < 1) || (x > 12) || (y < 1) || (y > 12)) {
		return false;
	}
	char n = g->board[y - 1][x];
	char e = g->board[y][x + 1];
	char w = g->board[y][x - 1];
	char s = g->board[y + 1][x];

	return (
		((n != '.') && (n != '+')) ||
		((e != '.') && (e != '+')) ||
		((w != '.') && (w != '+')) ||
		((s != '.') && (s != '+'))
		);
}

bool HasNoAdj(GameHistory *g, char type, int x, int y) {
	return (
		(g->board[y - 1][x] != type) &&
		(g->board[y][x + 1] != type) &&
		(g->board[y][x - 1] != type) &&
		(g->board[y + 1][x] != type)
		);
}

void UpdateEdgeLists(GameHistory *g) {
	Points pts;
	PointList *iter = nullptr;
	PointList *EdgeList = new PointList(0, 0);
	PointList *SunEdgeList = new PointList(0, 0);
	PointList *ShaEdgeList = new PointList(0, 0);

	for (int y = 1; y < 13; y++) {
		for (int x = 1; x < 13; x++) {
			if ((g->board[y][x] == '.') && HasPieceAdj(g, x, y)) {
				EdgeList = EdgeList->Append(new PointList(x, y));
			}
		}
	}
	iter = EdgeList;
	while (iter->next) {
		int x = iter->x;
		int y = iter->y;
		if (HasNoAdj(g, '_', x, y)) {
			SunEdgeList = SunEdgeList->Append(new PointList(x, y));
		}
		iter = iter->next;
	}
	iter = EdgeList;
	while (iter->next) {
		int x = iter->x;
		int y = iter->y;
		if (HasNoAdj(g, '*', x, y)) {
			if (
				ShaFindSiesta(g, 'n', x, y, &pts) ||
				ShaFindSiesta(g, 'e', x, y, &pts) ||
				ShaFindSiesta(g, 'w', x, y, &pts) ||
				ShaFindSiesta(g, 's', x, y, &pts)
				) {
				ShaEdgeList = ShaEdgeList->Append(new PointList(x, y));
			}
		}
		iter = iter->next;
	}

	g->EL = EdgeList;
	g->sunEL = SunEdgeList;
	g->shaEL = ShaEdgeList;
}

void DoCompMove(GameHistory *g) {
	GameHistory *tree = g;
	Move CandidateMove;
	Move NewCandidateMove;
	Points thismove, thispiece;

	thismove.red = 0;
	thismove.blu = 0;
	int i = 0;
	PointList *iiter = nullptr;
	int j = 0;
	PointList *jiter = nullptr;
	int k = 0;
	PointList *kiter = nullptr;

	for (i = 0, iiter = tree->EL; iiter->next; i++, iiter = iiter->next) {
		int x = iiter->x;
		int y = iiter->y;
		Points p;
		Present pres;
		pres.red = true;

		FindRoofPoints(tree, x, y, &p, &pres);
		thispiece.red = p.red;
		thispiece.blu = p.blu;
		NewCandidateMove.p1 = 'r';
		NewCandidateMove.el1 = i;
		printf("%d r (%d, %d): r %d, b %d\n", i, x, y, thispiece.red, thispiece.blu);

		tree = tree->Push(tree);
		tree->board[y][x] = 'r';
		UpdateEdgeLists(tree);

		for (j = 0, jiter = tree->sunEL; jiter->next; j++, jiter = jiter->next) {
			int x = jiter->x;
			int y = jiter->y;
			Points p;

			FindSunPoints(tree, x, y, &p);
			thispiece.red = p.red;
			thispiece.blu = p.blu;
			NewCandidateMove.p2 = '*';
			NewCandidateMove.el2 = j;
			printf("%d %d * (%d, %d): r %d, b %d\n", i, j, x, y, thispiece.red, thispiece.blu);

			tree = tree->Push(tree);
			tree->board[y][x] = '*';
			tree->ShowBoard();
			UpdateEdgeLists(tree);

			for (k = 0, kiter = tree->shaEL; kiter->next; k++, kiter = kiter->next) {
				int x = kiter->x;
				int y = kiter->y;
				Points p;

				FindShaPoints(tree, x, y, &p);
				thispiece.red = p.red;
				thispiece.blu = p.blu;
				NewCandidateMove.p3 = '_';
				NewCandidateMove.el3 = k;
				printf("%d %d %d _ (%d, %d): r %d, b %d\n", i, j, k, x, y, thispiece.red, thispiece.blu);
			}

			tree = tree->Pop();
		}

		tree = tree->Pop();
	}
}

void main()
{
	GameHistory *g = new GameHistory;
	Present p;
	p.blu = true;

	UpdateEdgeLists(g);
	DoCompMove(g);
}


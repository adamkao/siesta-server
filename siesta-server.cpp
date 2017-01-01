#include "stdafx.h"
#include <iostream>
#include <string>
#include <stdlib.h>

char *StartingBoard[14] = {
	"++++++++++++++",
	"+...*........+",
	"+.r.*........+",
	"+.****r*.....+",
	"+.bbrb.......+",
	"+..rb_.......+",
	"+...r........+",
	"+...*r.......+",
	"+...r*.......+",
	"+..._r.......+",
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
	int x1;
	int y1;
	char p2;
	int el2;
	int x2;
	int y2;
	char p3;
	int el3;
	int x3;
	int y3;
};

Move::Move() {
	this->scoredelta = 0;
	this->p1 = 'r';
	this->el1 = 0;
	this->x1 = 0;
	this->y1 = 0;
	this->p2 = '*';
	this->el2 = 0;
	this->x2 = 0;
	this->y2 = 0;
	this->p3 = '_';
	this->el3 = 0;
	this->x3 = 0;
	this->y3 = 0;
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
		if (pres.red) {
			p->red += 2;
		}
		if (pres.blu) {
			p->blu += 2;
		}
	}
	pres.red = false;
	pres.blu = false;
	if (FindShaDouble(g, 'e', x, y, &pres)) {
		if (pres.red) {
			p->red += 2;
		}
		if (pres.blu) {
			p->blu += 2;
		}
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
		if (pres.red) {
			p->red += 2;
		}
		if (pres.blu) {
			p->blu += 2;
		}
	}
	pres.red = false;
	pres.blu = false;
	if (FindSunDouble(g, 'e', x, y, &pres)) {
		if (pres.red) {
			p->red += 2;
		}
		if (pres.blu) {
			p->blu += 2;
		}
	}
	pres.red = false;
	pres.blu = false;
	if (FindSunDouble(g, 'w', x, y, &pres)) {
		if (pres.red) {
			p->red += 2;
		}
		if (pres.blu) {
			p->blu += 2;
		}
	}
	pres.red = false;
	pres.blu = false;
	if (FindSunDouble(g, 's', x, y, &pres)) {
		if (pres.red) {
			p->red += 2;
		}
		if (pres.blu) {
			p->blu += 2;
		}
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
		if (tpres.red) {
			p->red += 2;
		}
		if (tpres.blu) {
			p->blu += 2;
		}
	}
	tpres.red = pres->red;
	tpres.blu = pres->blu;
	if (FindRoofDouble(g, 'e', x, y, &tpres)) {
		if (tpres.red) {
			p->red += 2;
		}
		if (tpres.blu) {
			p->blu += 2;
		}
	}
	tpres.red = pres->red;
	tpres.blu = pres->blu;
	if (FindRoofDouble(g, 'w', x, y, &tpres)) {
		if (tpres.red) {
			p->red += 2;
		}
		if (tpres.blu) {
			p->blu += 2;
		}
	}
	tpres.red = pres->red;
	tpres.blu = pres->blu;
	if (FindRoofDouble(g, 's', x, y, &tpres)) {
		if (tpres.red) {
			p->red += 2;
		}
		if (tpres.blu) {
			p->blu += 2;
		}
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

void CheckP3Roofs(GameHistory *tree, Move *CandidateMove, Move *NewCandidateMove, Points *p1pts, Points *p2pts) {
	int k = 0;
	PointList *kiter = nullptr;
	Points p3pts;

	for (k = 0, kiter = tree->EL; kiter->next; k++, kiter = kiter->next) {
		int x = kiter->x;
		int y = kiter->y;
		Present pres;
		pres.blu = false;
		pres.red = true;

		p3pts.red = 0;
		p3pts.blu = 0;
		FindRoofPoints(tree, x, y, &p3pts, &pres);

		int scoredelta = (p1pts->red + p2pts->red + p3pts.red) - (p1pts->blu + p2pts->blu + p3pts.blu);

		if (scoredelta > CandidateMove->scoredelta) {
			CandidateMove->p1 = NewCandidateMove->p1;
			CandidateMove->el1 = NewCandidateMove->el1;
			CandidateMove->x1 = NewCandidateMove->x1;
			CandidateMove->y1 = NewCandidateMove->y1;
			CandidateMove->p2 = NewCandidateMove->p2;
			CandidateMove->el2 = NewCandidateMove->el2;
			CandidateMove->x2 = NewCandidateMove->x2;
			CandidateMove->y2 = NewCandidateMove->y2;
			CandidateMove->p3 = 'r';
			CandidateMove->el3 = k;
			CandidateMove->x3 = x;
			CandidateMove->y3 = y;
			CandidateMove->scoredelta = scoredelta;
		}
	}
}

void CheckP3Suns(GameHistory *tree, Move *CandidateMove, Move *NewCandidateMove, Points *p1pts, Points *p2pts) {
	int k = 0;
	PointList *kiter = nullptr;
	Points p3pts;

	for (k = 0, kiter = tree->sunEL; kiter->next; k++, kiter = kiter->next) {
		int x = kiter->x;
		int y = kiter->y;

		p3pts.red = 0;
		p3pts.blu = 0;
		FindSunPoints(tree, x, y, &p3pts);

		int scoredelta = (p1pts->red + p2pts->red + p3pts.red) - (p1pts->blu + p2pts->blu + p3pts.blu);

		if (scoredelta > CandidateMove->scoredelta) {
			CandidateMove->p1 = NewCandidateMove->p1;
			CandidateMove->el1 = NewCandidateMove->el1;
			CandidateMove->x1 = NewCandidateMove->x1;
			CandidateMove->y1 = NewCandidateMove->y1;
			CandidateMove->p2 = NewCandidateMove->p2;
			CandidateMove->el2 = NewCandidateMove->el2;
			CandidateMove->x2 = NewCandidateMove->x2;
			CandidateMove->y2 = NewCandidateMove->y2;
			CandidateMove->p3 = '*';
			CandidateMove->el3 = k;
			CandidateMove->x3 = x;
			CandidateMove->y3 = y;
			CandidateMove->scoredelta = scoredelta;
		}
	}
}

void CheckP3Shas(GameHistory *tree, Move *CandidateMove, Move *NewCandidateMove, Points *p1pts, Points *p2pts) {
	int k = 0;
	PointList *kiter = nullptr;
	Points p3pts;

	for (k = 0, kiter = tree->shaEL; kiter->next; k++, kiter = kiter->next) {
		int x = kiter->x;
		int y = kiter->y;

		p3pts.red = 0;
		p3pts.blu = 0;
		FindShaPoints(tree, x, y, &p3pts);

		int scoredelta = (p1pts->red + p2pts->red + p3pts.red) - (p1pts->blu + p2pts->blu + p3pts.blu);

		if (scoredelta > CandidateMove->scoredelta) {
			CandidateMove->p1 = NewCandidateMove->p1;
			CandidateMove->el1 = NewCandidateMove->el1;
			CandidateMove->x1 = NewCandidateMove->x1;
			CandidateMove->y1 = NewCandidateMove->y1;
			CandidateMove->p2 = NewCandidateMove->p2;
			CandidateMove->el2 = NewCandidateMove->el2;
			CandidateMove->x2 = NewCandidateMove->x2;
			CandidateMove->y2 = NewCandidateMove->y2;
			CandidateMove->p3 = '_';
			CandidateMove->el3 = k;
			CandidateMove->x3 = x;
			CandidateMove->y3 = y;
			CandidateMove->scoredelta = scoredelta;
		}
	}
}


void CheckP2Roofs(GameHistory *tree, Move *CandidateMove, Move *NewCandidateMove, Points *p1pts) {
	int j = 0;
	PointList *jiter = nullptr;
	Points p2pts;

	NewCandidateMove->p2 = 'r';
	for (j = 0, jiter = tree->EL; jiter->next; j++, jiter = jiter->next) {
		int x = jiter->x;
		int y = jiter->y;
		Present pres;
		pres.blu = false;
		pres.red = true;

		NewCandidateMove->el2 = j;
		NewCandidateMove->x2 = x;
		NewCandidateMove->y2 = y;

		p2pts.red = 0;
		p2pts.blu = 0;
		FindRoofPoints(tree, x, y, &p2pts, &pres);

		tree = tree->Push(tree);
		tree->board[y][x] = 'r';
		UpdateEdgeLists(tree);

		CheckP3Roofs(tree, CandidateMove, NewCandidateMove, p1pts, &p2pts);
		CheckP3Suns(tree, CandidateMove, NewCandidateMove, p1pts, &p2pts);
		CheckP3Shas(tree, CandidateMove, NewCandidateMove, p1pts, &p2pts);

		tree = tree->Pop();
	}
}

void CheckP2Suns(GameHistory *tree, Move *CandidateMove, Move *NewCandidateMove, Points *p1pts) {
	int j = 0;
	PointList *jiter = nullptr;
	Points p2pts;

	NewCandidateMove->p2 = '*';
	for (j = 0, jiter = tree->sunEL; jiter->next; j++, jiter = jiter->next) {
		int x = jiter->x;
		int y = jiter->y;

		NewCandidateMove->el2 = j;
		NewCandidateMove->x2 = x;
		NewCandidateMove->y2 = y;

		p2pts.red = 0;
		p2pts.blu = 0;
		FindSunPoints(tree, x, y, &p2pts);

		tree = tree->Push(tree);
		tree->board[y][x] = '*';
		UpdateEdgeLists(tree);

		CheckP3Roofs(tree, CandidateMove, NewCandidateMove, p1pts, &p2pts);
		CheckP3Suns(tree, CandidateMove, NewCandidateMove, p1pts, &p2pts);
		CheckP3Shas(tree, CandidateMove, NewCandidateMove, p1pts, &p2pts);

		tree = tree->Pop();
	}
}

void CheckP2Shas(GameHistory *tree, Move *CandidateMove, Move *NewCandidateMove, Points *p1pts) {
	int j = 0;
	PointList *jiter = nullptr;
	Points p2pts;

	NewCandidateMove->p2 = '_';
	for (j = 0, jiter = tree->shaEL; jiter->next; j++, jiter = jiter->next) {
		int x = jiter->x;
		int y = jiter->y;

		NewCandidateMove->el2 = j;
		NewCandidateMove->x2 = x;
		NewCandidateMove->y2 = y;

		p2pts.red = 0;
		p2pts.blu = 0;
		FindShaPoints(tree, x, y, &p2pts);

		tree = tree->Push(tree);
		tree->board[y][x] = '_';
		UpdateEdgeLists(tree);

		CheckP3Roofs(tree, CandidateMove, NewCandidateMove, p1pts, &p2pts);
		CheckP3Suns(tree, CandidateMove, NewCandidateMove, p1pts, &p2pts);
		CheckP3Shas(tree, CandidateMove, NewCandidateMove, p1pts, &p2pts);

		tree = tree->Pop();
	}
}

void DoCompMove2(GameHistory *g) {
	GameHistory *tree = g;
	int i = 0;
	PointList *iiter = nullptr;
	Points p1pts;

	Move CandidateMove;
	Move NewCandidateMove;

	NewCandidateMove.p1 = 'r';
	for (i = 0, iiter = tree->EL; iiter->next; i++, iiter = iiter->next) {
		int x = iiter->x;
		int y = iiter->y;
		Present pres;
		pres.blu = false;
		pres.red = true;

		NewCandidateMove.el1 = i;
		NewCandidateMove.x1 = x;
		NewCandidateMove.y1 = y;

		p1pts.red = 0;
		p1pts.blu = 0;
		FindRoofPoints(tree, x, y, &p1pts, &pres);

		tree = tree->Push(tree);
		tree->board[y][x] = 'r';
		UpdateEdgeLists(tree);

		CheckP2Roofs(tree, &CandidateMove, &NewCandidateMove, &p1pts);
		CheckP2Suns(tree, &CandidateMove, &NewCandidateMove, &p1pts);
		CheckP2Shas(tree, &CandidateMove, &NewCandidateMove, &p1pts);

		tree = tree->Pop();
	}
	NewCandidateMove.p1 = '*';
	for (i = 0, iiter = tree->sunEL; iiter->next; i++, iiter = iiter->next) {
		int x = iiter->x;
		int y = iiter->y;

		NewCandidateMove.el1 = i;
		NewCandidateMove.x1 = x;
		NewCandidateMove.y1 = y;

		p1pts.red = 0;
		p1pts.blu = 0;
		FindSunPoints(tree, x, y, &p1pts);

		tree = tree->Push(tree);
		tree->board[y][x] = '*';
		UpdateEdgeLists(tree);

		CheckP2Roofs(tree, &CandidateMove, &NewCandidateMove, &p1pts);
		CheckP2Suns(tree, &CandidateMove, &NewCandidateMove, &p1pts);
		CheckP2Shas(tree, &CandidateMove, &NewCandidateMove, &p1pts);

		tree = tree->Pop();
	}
	NewCandidateMove.p1 = '_';
	for (i = 0, iiter = tree->shaEL; iiter->next; i++, iiter = iiter->next) {
		int x = iiter->x;
		int y = iiter->y;

		NewCandidateMove.el1 = i;
		NewCandidateMove.x1 = x;
		NewCandidateMove.y1 = y;

		p1pts.red = 0;
		p1pts.blu = 0;
		FindShaPoints(tree, x, y, &p1pts);

		tree = tree->Push(tree);
		tree->board[y][x] = '_';
		UpdateEdgeLists(tree);

		CheckP2Roofs(tree, &CandidateMove, &NewCandidateMove, &p1pts);
		CheckP2Suns(tree, &CandidateMove, &NewCandidateMove, &p1pts);
		CheckP2Shas(tree, &CandidateMove, &NewCandidateMove, &p1pts);

		tree = tree->Pop();
	}
}

void main()
{
	GameHistory *g = new GameHistory;
	Present p;
	p.blu = true;

	UpdateEdgeLists(g);
	DoCompMove2(g);
}


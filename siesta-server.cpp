#include "stdafx.h"
#include <iostream>
#include <string>

char *board[14] = {
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
	int x;
	int y;
	PointList *next;
	PointList(int x, int y);
	PointList *Append(PointList *next);
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

PointList *EdgeList = nullptr;
PointList *SunEdgeList = nullptr;
PointList *ShaEdgeList = nullptr;

Cursor *cursor = new Cursor;

Points *thispiece = new Points;
Points *thismove = new Points;

char at() {
	return board[cursor->y][cursor->x];
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
void SkipRoofs(Present *p) {
	while ((at() == 'r') || (at() == 'b')) {
		if (at() == 'r') {
			p->red = true;
		}
		else if (at() == 'b') {
			p->blu = true;
		}
		step();
	}
}

void AddPointsFrom(int PointCt, Present *p, Points *pts) {
	while (at() == '_') {
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

bool ShaFindSiesta(char dir, int x, int y, Points *p) {
	Present pres;

	cursor->dir = dir;
	cursor->x = x;
	cursor->y = y;

	do {
		step();
	} while (at() == '_');

	if ((at() != 'r') && (at() != 'b')) {
		return false;
	}

	SkipRoofs(&pres);

	if (at() == '*') {
		cursor->x = x;
		cursor->y = y;
		reverse();
		step();
		AddPointsFrom(1, &pres, p);
		return true;
	}
	return false;
}

bool FindShaDouble(char dir, int x, int y, Present *pres) {
	cursor->dir = dir;
	cursor->x = x;
	cursor->y = y;

	do {
		step();
	} while (at() == '_');

	if ((at() != 'r') && (at() != 'b')) {
		return false;
	}

	SkipRoofs(pres);

	if (((pres->red) && (pres->blu)) || at() != '*') {
		return false;
	}

	cursor->x = x;
	cursor->y = y;
	reverse();

	do {
		step();
	} while (at() == '_');

	if ((at() != 'r') && (at() != 'b')) {
		return false;
	}

	SkipRoofs(pres);

	if (((pres->red) && (pres->blu)) || at() != '*') {
		return false;
	}

	return true;
}

void FindShaPoints(int x, int y, Points *p) {
	Present pres;

	ShaFindSiesta('n', x, y, p);
	ShaFindSiesta('e', x, y, p);
	ShaFindSiesta('w', x, y, p);
	ShaFindSiesta('s', x, y, p);

	pres.red = false;
	pres.blu = false;
	if (FindShaDouble('n', x, y, &pres)) {
		bonuspts(&pres);
	}
	pres.red = false;
	pres.blu = false;
	if (FindShaDouble('e', x, y, &pres)) {
		bonuspts(&pres);
	}
}

void SunFindSiesta(char dir, int x, int y, Points *p) {
	Present pres;

	cursor->dir = dir;
	cursor->x = x;
	cursor->y = y;

	step();
	SkipRoofs(&pres);

	if (at() != '_') {
		return;
	}

	AddPointsFrom(0, &pres, p);
}

bool FindSunDouble(char dir, int x, int y, Present *pres) {
	Points p;

	cursor->dir = dir;
	cursor->x = x;
	cursor->y = y;

	step();
	SkipRoofs(pres);

	if (((pres->red) && (pres->blu)) || (at() != '_')) {
		return false;
	}

	AddPointsFrom(0, pres, &p);

	SkipRoofs(pres);

	if (((pres->red) && (pres->blu)) || (at() != '*')) {
		return false;
	}

	return true;
}

void FindSunPoints(int x, int y, Points *p) {
	Present pres;

	SunFindSiesta('n', x, y, p);
	SunFindSiesta('e', x, y, p);
	SunFindSiesta('w', x, y, p);
	SunFindSiesta('s', x, y, p);

	pres.red = false;
	pres.blu = false;
	if (FindSunDouble('n', x, y, &pres)) {
		bonuspts(&pres);
	}
	pres.red = false;
	pres.blu = false;
	if (FindSunDouble('e', x, y, &pres)) {
		bonuspts(&pres);
	}
	pres.red = false;
	pres.blu = false;
	if (FindSunDouble('w', x, y, &pres)) {
		bonuspts(&pres);
	}
	pres.red = false;
	pres.blu = false;
	if (FindSunDouble('s', x, y, &pres)) {
		bonuspts(&pres);
	}
}

void RoofFindSiesta(char dir, int x, int y, Present *pres, Points *p) {
	Present tpres;

	cursor->dir = dir;
	cursor->x = x;
	cursor->y = y;
	tpres.red = pres->red;
	tpres.blu = pres->blu;
	
	step();
	SkipRoofs(&tpres);

	if (at() != '*') {
		return;
	}

	cursor->x = x;
	cursor->y = y;
	reverse();

	step();
	SkipRoofs(&tpres);

	if (at() != '_') {
		return;
	}

	AddPointsFrom(0, &tpres, p);
}

bool FindRoofDouble(char dir, int x, int y, Present *pres) {
	Points p;

	cursor->dir = dir;
	cursor->x = x;
	cursor->y = y;

	step();
	SkipRoofs(pres);

	if ((pres->red && pres->blu) || (at() != '*')) {
		return false;
	}

	cursor->x = x;
	cursor->y = y;
	reverse();

	step();
	SkipRoofs(pres);

	if ((pres->red && pres->blu) || (at() != '_')) {
		return false;
	}

	AddPointsFrom(0, pres, &p);

	SkipRoofs(pres);

	if ((pres->red && pres->blu) || (at() != '*')) {
		return false;
	}

	return true;
}

void FindRoofPoints(int x, int y, Points *p, Present *pres) {
	Present tpres;
	
	RoofFindSiesta('n', x, y, pres, p);
	RoofFindSiesta('e', x, y, pres, p);
	RoofFindSiesta('w', x, y, pres, p);
	RoofFindSiesta('s', x, y, pres, p);

	tpres.red = pres->red;
	tpres.blu = pres->blu;
	if (FindRoofDouble('n', x, y, &tpres)) {
		bonuspts(&tpres);
	}
	tpres.red = pres->red;
	tpres.blu = pres->blu;
	if (FindRoofDouble('e', x, y, &tpres)) {
		bonuspts(&tpres);
	}
	tpres.red = pres->red;
	tpres.blu = pres->blu;
	if (FindRoofDouble('w', x, y, &tpres)) {
		bonuspts(&tpres);
	}
	tpres.red = pres->red;
	tpres.blu = pres->blu;
	if (FindRoofDouble('s', x, y, &tpres)) {
		bonuspts(&tpres);
	}
}

bool HasPieceAdj(int x, int y) {
	if ((x < 1) || (x > 12) || (y < 1) || (y > 12)) {
		return false;
	}
	char n = board[y - 1][x];
	char e = board[y][x + 1];
	char w = board[y][x - 1];
	char s = board[y + 1][x];

	return (
		((n != '.') && (n != '+')) ||
		((e != '.') && (e != '+')) ||
		((w != '.') && (w != '+')) ||
		((s != '.') && (s != '+'))
		);
}

bool HasNoAdj(char type, int x, int y) {
	return (
		(board[y - 1][x] != type) &&
		(board[y][x + 1] != type) &&
		(board[y][x - 1] != type) &&
		(board[y + 1][x] != type)
		);
}

void UpdateEdgeLists() {
	EdgeList = new PointList(0, 0);
	SunEdgeList = new PointList(0, 0);
	ShaEdgeList = new PointList(0, 0);

	for (int y = 1; y < 13; y++) {
		for (int x = 1; x < 13; x++) {
			if ((board[y][x] == '.') && HasPieceAdj(x, y)) {
				EdgeList = EdgeList->Append(new PointList(x, y));
			}
		}
	}
	PointList *iter = EdgeList;
	while (iter->next) {
		int x = iter->x;
		int y = iter->y;
		if (HasNoAdj('_', x, y)) {
			SunEdgeList = SunEdgeList->Append(new PointList(x, y));
		}
		iter = iter->next;
	}
}


void main()
{
	Present p;
	p.blu = true;

	UpdateEdgeLists();
	thispiece->red = 0;
	thispiece->blu = 0;
	FindRoofPoints(5, 2, thispiece, &p);
}


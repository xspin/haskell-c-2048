// -*- coding: utf-8 -*-
// Copyright © 2014 Spin <code@e-spin.org>
//
#ifndef __HEAD_H
#define __HEAD_H

//********C++ head file 
//#include <python3.3/Python.h>
//#include <iostream>
//#include <cstdlib>
//#include <cstring>
//#include <cstdio>
//#include <fstream>
//#include <cmath>
//using namespace std;
//*********************

//*****C head file
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <time.h>
#define max(a, b) ((a) > (b) ? (a) : (b))
#define min(a, b) ((a) < (b) ? (a) : (b))
//*****************
#define LEN 4
#define Directions "URDL"
#define Convert(i, x, k) ((i) + (2*(i)-(k))*((x)-1)/2)

typedef int* Grid[4];
#include "ffi.h"

const int dir[][4] = {{1,0},{0,-1},{-1,0},{0,1}}; // U R D L

struct Tuple{
	public:
		int x;
		int y;
};

float log2(float n);
float log2(int n);
int index(char key);
void zero(int **ar);
void copy(int **ar, int **br);
void memGet(Grid g);
void memFree(Grid g);

bool shift(Grid grid, char key, int* score=NULL, bool move=true);
bool canShift( Grid grid, char key);
//int randChoice(int* ch, int len);
bool randPut(Grid grid);
int maxTile( Grid grid, Tuple *pos = NULL);

bool over(Grid grid);
bool win(Grid grid);
void init(Grid grid);

#endif

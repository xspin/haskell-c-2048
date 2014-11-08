#ifndef __RUN_H
#define __RUN_H

#define WIN 1
#define FAIL -1
#define NONE 0

#include "head.h"
#include "ai.h"

int run(Grid grid, char ch, int* score, bool ai);
void disp(Grid grid);
void putScore(int bestScore);
int getScore();

Grid GRID;
int SCORE = 0; 
int bestScore = 0;
//#include "ffi.h"
#endif

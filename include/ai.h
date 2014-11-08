#ifndef __AI_H
#define __AI_H

#include "head.h"

#define Base 0.9
#define Proportion(x, d) ((x) * pow(Base, maxDepth - (d)))

int empty(Grid grid);
float w_smooth(Grid grid);
float w_monotonicity(Grid grid);
float w_small(Grid grid);
float weight(Grid grid);

float evaluateMax(float alpha, float beta, int depth, Grid grid);
float evaluateMin(float alpha, float beta, int depth, Grid grid);

char aI(Grid grid);

#endif

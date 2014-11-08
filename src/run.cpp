// -*- coding: utf-8 -*-
// Copyright © 2014 Spin <code@e-spin.org>

#include "run.h"

int run(Grid grid, char ch, int* score, bool ai = false)
{
	if(!over(grid))
	{
		if (ai)
		{
			ch = aI(grid);
		}
		if (shift(grid, ch, score))
		{
			randPut(grid);
            if(win(grid))
            {
                return WIN;
            }
	    }
        else
        {
            return NONE;
        }
    }
    else
        return FAIL;
}

void disp(Grid grid)
{
	/* 显示grid */
	printf("--------------------\n");
	for (int i=0; i<LEN; i++)
	{
		for (int j=0; j<LEN; j++)
		{
			if(grid[i][j] == 0)
				printf("    |");
			else
				printf("%4d|", grid[i][j]);
		}
        printf("\n");
	}
	printf("--------------------\n");
}

int getScore()
{
	FILE *fp = fopen("log", "r");
    int best = 0;
	if (fp == NULL)
	{
		printf("Can not open 'log' file!\n");
        printf("Make 'log' file.\n");
        fp = fopen("log", "wr");
        if(fp==NULL)
        {
            //cout << "Make fail!" << endl;
            exit(0);
        }
        fprintf(fp, "BEST:0");
	}
	fscanf(fp, "BEST:%d", &best);
	fclose(fp);
    return best;
}
void putScore(int bestScore)
{
	FILE* fp = fopen("log", "w");
	fprintf(fp, "BEST:%d", bestScore);
	fclose(fp);
}


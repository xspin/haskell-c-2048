#include "ai.h"

float Prop(int depth)
{
	float r = 0.5;
	return pow(r, depth);
}
float w_smooth(Grid grid)
{
	/* 计算光滑的权重 */
	float mg = 0;
	for (int k=0; k<LEN; k++)
	{
		for (int i=k, j=1; j<LEN; j++)
		{
			float a = grid[i][j-1];
			float b = grid[i][j];
			a = (a == 0)? 0 : log2(a);
			b = (b == 0)? 0 : log2(b);
			if (a == b)
            {
				mg += a;
                //j++;
            }
		}
		for (int i=1, j=k; i<LEN; i++)
		{
			float a = grid[i-1][j];
			float b = grid[i][j];
			a = (a == 0)? 0 : log2(a);
			b = (b == 0)? 0 : log2(b);
			if (a == b)
            {
				mg += a;
                //i++;
            }
		}
	}
	return mg;
}

int empty(Grid grid)
{
	float count = 0;
	for (int i=0; i<LEN; i++)
	{
		for (int j=0; j<LEN; j++)
		{
			if (grid[i][j] == 0)
			{
				count ++;
			}
		}
	}
	return count; 
}

float w_monotonicity(Grid grid)
{
	float t1, t2, t3, t4;
	float mono = 0;
	t1=t2=t3=t4=0;
	for (int k=0; k<LEN; k++)
	{
		for (int i=k, j=1; j<LEN; j++)
		{
			float a = grid[i][j-1];
			float b = grid[i][j];
			a = (a == 0)? 0 : log2(a);
			b = (b == 0)? 0 : log2(b);
			if (a > b)
				t1 += b-a;
			else if(a < b)
				t2 += a-b;
		}
		for (int i=1, j=k; i<LEN; i++)
		{
			float a = grid[i-1][j];
			float b = grid[i][j];
			a = (a == 0)? 0 : log2(a);
			b = (b == 0)? 0 : log2(b);
			if (a > b)
				t4 += b - a;
			else if (a < b)
				t3 += a - b;
		}
	}
	mono = max(t1, t2) + max(t3, t4);
	return mono;
}

float weight(Grid grid)
{
	/*计算权重*/
	float blank 	= log2(empty(grid))			* 2; // lg1 ~ lg16 -> 0 ~ 4
	float smooth 	= w_smooth(grid)		    * 1; // 0 ~ max*2  
	float mono		= w_monotonicity(grid)		* 1;
	//float max 		= log2(maxTile(grid))		* 0;
	float wt = 
				+ blank 	
				+ smooth 	
				+ mono
				//+ max
				;
	return wt;
}

float evaluateMax(float alpha, float beta, int depth, Grid grid)
{
	Grid temp;
    memGet(temp);
	copy(temp, grid);
	float w;
	if (depth > 0)
	{
		for (int i=0; i<4; i++)
		{
			if(shift(temp, Directions[i]));
			{
				w = evaluateMin(alpha, beta, depth-1, temp);
			}
			if (w > alpha)
			{
				alpha = w;
				if (alpha >= beta)
					return alpha;
			}
		}
	}
	else
	{
		for (int i=0; i<4; i++)
		{
			copy(temp, grid);
			if(shift(temp, Directions[i]));
			{
				w = weight(temp);
			}
			if (w > alpha)
			{
				alpha = w;
				if (alpha >= beta)
					return alpha;
			}
		}
	}
    memFree(temp);
	return alpha;
}
float evaluateMin(float alpha, float beta, int depth, Grid grid)
{
	Grid temp;
    memGet(temp);
	float w;
	for (int n=2; n<=4; n+=2)
	{
		for (int r=0; r<LEN; r++)
		{
			for (int c=0; c<LEN; c++)
			{
				if (grid[r][c] == 0)
				{
					copy(temp, grid);
					temp[r][c] = n;
					w = evaluateMax(alpha, beta, depth - 1, temp);
					if (w < beta)
					{
						beta = w;
						if (w <= alpha)
						{
							return beta;
						}
					}

				}
			}
		}
	}
    memFree(temp);
	return beta;
}
char aI(Grid grid)
{
	Grid temp;
    memGet(temp);
	char key = '*';
	float w;
	float alpha = -1e6;
	float beta = 1e6;
	int depth = 2;
	for (int i=0; i<4; i++)
	{
		copy(temp, grid);
		if(shift(temp, Directions[i]))
		{
			w = evaluateMin(alpha, beta, depth-1, temp);
			if (w > alpha)
			{
				alpha = w;
				key = Directions[i];
			}
		}
	}
	memFree(temp);
	return key;
}

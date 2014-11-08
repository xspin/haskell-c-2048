#include "head.h"

float log2(int n)
{
	if (n <= 0)
	{
		return 0;
	}
	float t = log(n) / log(2);
	return t;
}
float log2(float n)
{
	if (n <= 0)
	{
		return 0;
	}
	float t = log(n) / log(2);
	return t;
}
int index(char key)
{
	/*方向定位*/
	key = toupper(key);
	switch(key)
	{
		case 'U':
			return 0;
		case 'R':
			return 1;
		case 'D':
			return 2;
		case 'L':
			return 3;
		default:
            //cout << "Key Error!!" <<endl;
			//cout << "key: " << key <<endl;
            printf("Key Error! -> key: %c\n", key);
			exit(0);
	}
}

void zero(Grid g)
{
	for(int i=0; i<LEN; i++)
    {
        for(int j=0; j<LEN; j++)
        {
            g[i][j] = 0;
        }
    }
}

void copy(Grid ar, Grid br)
{
	for(int i=0; i<LEN; i++)
        for(int j=0; j<LEN; j++)
            ar[i][j] = br[i][j];
}

bool shift(Grid grid, char key, int* score, bool move)
{
	/* 向某个方向移动 */
	int idx = index(key);
	int x = dir[idx][0];
	int y = dir[idx][1];
	int plus = x + y;
	int minus = x - y;
	int k = LEN - 1;
	bool canMove = false;
	Tuple flag;
	for (int i=0; i<=k; i++)
	{
		for (int j=0; j<=k; j++)
		{
			int R = x == 0 ? i : j;
			int C = x == 0 ? j : i;
			int row = Convert(R, minus, k);
			int col = Convert(C, plus, k);
			if (j == 0)
			{
				flag.x = row;
				flag.y = col;
				continue;
			}
			int *here = &grid[row][col];
			if (*here == 0)
			{
				if (!move)
					return true;
				continue;
			}

			int *pflag = &grid[flag.x][flag.y];
			if (*pflag == 0)
			{
				if (!move)
					return true;
				canMove = true;
				*pflag = *here;
				*here = 0;
			}
			else if (*here == *pflag)
			{
				if (!move)
					return true;
				canMove = true;
				*pflag *= 2;
				*here = 0;
				flag.x += x;
				flag.y += y;
				if (score != NULL)
					*score += *pflag;
			}
			else
			{
				flag.x += x;
				flag.y += y;
				if (flag.x != row || flag.y != col)
					j--;
			}
		}
	}
	return canMove;
}
bool canShift(Grid grid, char key)
{
	/* 判断 "key" 方向是否能移动 */
	return shift(grid, key, NULL, false);
}

int randChoice(int* ch, int len)
{
	/*从一个数组里随机选择一个*/
	int i = rand() % len;
	return ch[i];
}

bool randPut(Grid grid)
{
	/*向空zx的地方随机填一个2或4 按某概率*/
	int k = 0;
	Tuple empty[LEN*LEN];
	for (int i=0; i<LEN; i++)
	{
		for (int j=0; j<LEN; j++)
		{
			if(grid[i][j] == 0)
			{
				empty[k].x = i;
				empty[k].y = j;
				k++;
			}
		}
	}
	if (k == 0)
		return false;
	int num = (rand()/double(RAND_MAX) < 0.9)? 2 : 4;
	int pos = rand()%k;
	grid[empty[pos].x][empty[pos].y] = num;
	return true;
}

int maxTile(Grid grid, Tuple *pos)
{
	/* 找最大值位置 并返回最大值 */
	int max = 0;
	for (int i=0; i<LEN; i++)
	{
		for (int j=0; j<LEN; j++)
		{
			if(grid[i][j] > max)
			{
				if (pos != NULL)
				{
					pos->x = i;
					pos->y = j;
				}
				max = grid[i][j];
			}
		}
	}
	return max;
}

bool over(Grid grid)
{
	/*判断是否 Game Over*/
	for (int i=0; i<4; i++)
	{
		if (canShift(grid, Directions[i]))
			return false;
	}
	return true;
}

bool win(Grid grid)
{
	for (int i=0; i<LEN; i++)
	{
		for (int j=0; j<LEN; j++)
		{
			if (grid[i][j] == 2048 )
			{
				return true;
			}
		}
	}
	return false;
}
void init(Grid grid)
{
	srand((unsigned)time(NULL)); //以时间为随机种子
    memGet(grid);
	zero(grid);
	randPut(grid);
	randPut(grid);
}
void memGet(Grid g)
{
   //g = (int**)malloc(sizeof(int*)*LEN);
   for(int i=0; i<LEN; i++)
   {
       g[i] = (int*)malloc(sizeof(int)*LEN);
   }
}
void memFree(Grid g)
{
    for(int i=0; i<LEN; i++)
        free(g[i]);
}

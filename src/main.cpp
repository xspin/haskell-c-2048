/*================================================
#  Author:      Spin    
#  Email: 		code@e-spin.org           
#  FileName:        
#  Version:     v1.0    
#  LastChange:      
#  Description: 2048 Game    
#  History:         
==================================================*/
//#include "head.h"
//#include "ai.h"
#include "run.h"


//=========================================================
void start(Grid grid, bool AI = false)
{
	FILE *fp = fopen("log", "r");
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
        fprintf(fp, "BEST:0\n");
	}
	int bestScore = 0;
	fscanf(fp, "BEST:%d", &bestScore);
	fclose(fp);
	int cnt = 0;
	int score = 0;
	char key;
	bool limit = true;
    //AI = false;
	while(!over(grid))
	{
		disp(grid);
		printf("|-------------------> Scores: %d\n", score);
		if (!AI)
		{
			printf("|---> ");
            key = getchar();
			switch(key)
			{
				case 'w': key = 'U'; break;
				case 'd': key = 'R'; break;
				case 's': key = 'D'; break;
				case 'a': key = 'L'; break;
				case 'q': break;
			}
		}
		else
		{
			key = aI(grid);
		}
		if (shift(grid, key, &score))
		{
			randPut(grid);
			cnt ++;
		}
		if(limit && win(grid))
		{
			disp(grid);
			printf("\n\n================= Congratulation!!! ========================\n");
			printf("\n\n             ######## You Win !!! ######\n\n\n");
			//system("sleep 5");
			//cout << "Continue?(Y/N): " <<endl;
			//char ch;
			//cin >> ch;
			//if (ch == 'n' || ch == 'N')
				//break;
			limit = false;
			system("sleep 1");
		}
	}
	disp(grid);
	if (score > bestScore)
	{
		bestScore = score;
		printf("################New Record!!###########\n");
	}
	printf("\n           ***** Game Over! *****\n");
	printf("||==================>> Score: %d || Best Score: %d\n", score, bestScore);
	printf("Steps: %d\n", cnt);

	fp = fopen("log", "w");
	fprintf(fp, "BEST:%d", bestScore);
	fclose(fp);
}

void run()
{
	int theTime = time(NULL);
	Grid grid;
	init(grid);
	bool ai = true;
	start(grid, ai);
	printf("[time: %lds]\n", time(NULL)-theTime);
}
int main()
{
    run();
    //int bestScore = getScore();
    //printf("%d\n", bestScore);
    return 0;
}


#ifndef __FFI_H
#define __FFI_H

#ifdef __cplusplus
extern "C"
{
#endif

    extern void init(Grid grid);
    extern int run(Grid grid, char ch, int* score, bool ai);
    extern void disp(Grid grid);
    extern void memGet(Grid grid);
    extern void memFree(Grid grid);
    extern int getScore(void);
    extern void putScore(int bestScore);

    //Grid GRID;
    //int SCORE = 0; 

#ifdef __cplusplus
}
#endif

#endif

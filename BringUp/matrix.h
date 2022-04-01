#ifndef _MATRIX_H_
#define _MATRIX_H_
//Create a dynamic matrix.
int** CreatMat(int N);
//Checks if 2 matrices are equal.

bool MatCmp(int** S,int** SJ,int N);
//Free a dynamic matrix.
void FreeMat(int*** T,int N);
void RandMat(int*** T,int N,int max,int min);
void PrintMat(int** T,int N);

/**
 * Counts how many connection does circle have.
 * \param S 6x6 matrix.
 * \param i 7x7[i][] pos.
 * \param j 7x7[][j] pos.
 * \returns number of connections.
 **/
int con(int** S,int i,int j);
void MatCpy(int** source, int*** target, size_t N);
void MatnCpy(size_t N,int** source,int (*target)[][N]);

int Grids_Init(int*** T,int*** S,int*** SJ);

#endif
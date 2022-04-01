#include<stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <stdbool.h>

#include"matrix.h"

//Create a dynamic matrix.
int** CreatMat(int N){
    int i,**T;
    T = (int**)malloc(sizeof(int*)*N);
    if(T!=NULL){
        for(i=0;i<N;i++){
            T[i]=(int*)malloc(sizeof(int)*N);
            if(T[i]==NULL){
                printf("\nCreatMat()::Allocation failed at block %d",i);
                for(i=i;i>=0;i--){
                    free(T[i]);
                    T[i]=NULL;
                }
                free(T);
                T=NULL;
                return T;
            }
        }
    }
    return T;
}

bool MatCmp(int** S,int** SJ,int N){
    int i,j;
    bool equal = 1;
    for(i=0;i<N;i++){
        for(j=0;j<N;j++){
            if(S[i][j]!=SJ[i][j]){
                equal = 0;
                break;
            }
        }
    }
    return equal;
}

//Free a dynamic matrix.
void FreeMat(int*** T,int N){
    int i;
    if((*T)!=NULL){
        for(i=0;i<N;i++){
            if((*T)[i]!=NULL){
                free((*T)[i]);
                (*T)[i]=NULL;
            }
        }
        free(*T);
        *T = NULL;
    }
}
void RandMat(int*** T,int N,int max,int min){
    srand(time(NULL));
    int i,j;
    for(i=0;i<N;i++){
        for(j=0;j<N;j++){
            (*T)[i][j]=rand()%(max+1-min)+min;
        }
    }
}
void PrintMat(int** T,int N){
    int i,j;
    for(i=0;i<N;i++){
        printf("\n");
        for(j=0;j<N;j++){
            printf("%d  ",T[i][j]);
        }
    }
}

/**
 * Counts how many connection does circle have.
 * \param S 6x6 matrix.
 * \param i 7x7[i][] pos.
 * \param j 7x7[][j] pos.
 * \returns number of connections.
 **/
int con(int** S,int i,int j){
    int cmpt=0;
    if((i-1>=0)&&(j-1>=0)&& S[i-1][j-1]==2){
        cmpt++;
    }
    if((i-1>=0)&&(j<6)&& S[i-1][j]==1){
        cmpt++;
    }
    if((i<6)&&(j-1>=0)&& S[i][j-1]==1){
        cmpt++;
    }
    if((i<6)&&(j<6)&& S[i][j]==2){
        cmpt++;
    }
    return cmpt;
}
void MatCpy(int** source, int*** target, size_t N){
    int i,j;
    for(i=0;i<N;i++){
        for(j=0;j<N;j++){
            (*target)[i][j]=source[i][j];
        }
    }
}

void MatnCpy(size_t N,int** source,int (*target)[][N]){
    int i,j;
    for(i=0;i<N;i++){
        for(j=0;j<N;j++){
            (*target)[i][j]=source[i][j];
        }
    }
}

int Grids_Init(int*** T,int*** S,int*** SJ){
    int i,j;
    *T=CreatMat(7);
    if(*T==NULL){
        printf("\nGrids_Init()::Failed to allocate memory.");
        return 1;
    }
    *S=CreatMat(6);
    if(*S==NULL){
        FreeMat(T,7);
        printf("\nGrids_Init()::Failed to allocate memory.");
        return 1;
    }
    *SJ=CreatMat(6);
    if(*SJ==NULL){
        FreeMat(T,7);FreeMat(S,6);
        printf("\nGrids_Init()::Failed to allocate memory.");
        return 1;
    }
    RandMat(S,6,2,1);
    RandMat(SJ,6,0,0);
    PrintMat(*S,6);
    printf("\n\n");
    for(i=0;i<7;i++){
        for(j=0;j<7;j++){
            (*T)[i][j]=con(*S,i,j);
        }
    }
    PrintMat(*T,7);
    return 0;
}
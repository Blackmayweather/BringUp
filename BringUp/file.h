#ifndef _FILE_H_
#define _FILE_H_

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<stdbool.h>
#include<time.h>
#include<SDL2/SDL.h>


typedef struct {
    char name[24];
    Uint16 score;
    char date[21];
}score_t;

typedef struct {
    Uint16 id;
    bool won;
    score_t score;
    int game[6][6];
    int SJ[6][6];
}player_t;

typedef struct {
    FILE* file;
    char path[64];
}file_t;

bool _save_player(file_t* fm,player_t* player);
bool _load_player(file_t* fm,player_t* player);
bool seekid(file_t* fm,Uint16 id);
file_t openfile(const char path[64],const char* mode);
void closefile(file_t* fm);
void New_player_id(file_t *fm,player_t *player);
int save_game(player_t *player,file_t *fm);
bool load_game(Uint16 id,file_t *fm,player_t* player);
score_t* Top_creat5(file_t* fm);
#endif
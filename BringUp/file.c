#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<stdbool.h>
#include<time.h>
#include<SDL2/SDL.h>

#include "file.h"

bool _save_player(file_t* fm,player_t* player){
    if(fwrite(player,sizeof(player_t),1,fm->file)<=0){
        printf("\n_save_player()::failed.");
        return 1;
    }
    return 0;
}
bool _load_player(file_t* fm,player_t* player){
    player_t p;
    if(fread(&p,sizeof(player_t),1,fm->file)<=0){
        return 1;
    }else{
        *player = p;
        return 0;
    }
}
bool seekid(file_t* fm,Uint16 id){
    player_t p;
    rewind(fm->file);
    while(feof(fm->file)==0){
        //fread(&p,sizeof(player_t),1,fm->file);
        if(!_load_player(fm,&p)){
            if(p.id==id){
                return true;
            }
        }
    }
    return false;
}
file_t openfile(const char path[64],const char* mode){
    file_t f1;
    f1.file = fopen(path,mode);
    strcpy(f1.path,path);
    return f1;
}
void closefile(file_t *fm){
    fclose(fm->file);
}
void New_player_id(file_t *fm,player_t *player){
    rewind(fm->file);
    player->id=0;
    player_t p;
    while(feof(fm->file)==0){
        //fread(&p,sizeof(player_t),1,fm->file);
        if(!_load_player(fm,&p)){
            player->id+=(p.id==player->id)?1:0;
        }
    }
}
int save_game(player_t *player,file_t *fm){
    if(seekid(fm,player->id)){
        player_t tp;
        file_t tmpf = {tmpfile(),""};
        rewind(fm->file);
        while(feof(fm->file)==0){
            if(!_load_player(fm,&tp)){
                if(tp.id!=player->id){
                    _save_player(&tmpf,&tp);
                }else{
                    _save_player(&tmpf,player);
                }
            }
        }
        fclose(fm->file); remove(fm->path);
        rewind(tmpf.file);
        fm->file = fopen(fm->path,"w+");
        if(!fm->file){
            printf("save_game()::file recreation failed.");
            return 1;
        }

        while(feof(tmpf.file)==0){
            if(!_load_player(&tmpf,&tp))
            _save_player(fm,&tp);
        }fclose(tmpf.file);
    }else{
        _save_player(fm,player);
    }
    return 0;
}
bool load_game(Uint16 id,file_t *fm,player_t* player){
    if(seekid(fm,id)){
        player_t p;
        rewind(fm->file);
        while(feof(fm->file)==0){
            if(!_load_player(fm,&p)){
                if(p.id==id){
                    *player=p;
                    return 1;
                }
            }
        }
    }else return 0;
}
score_t* Top_creat5(file_t* fm){
    Uint8 i;
    player_t p; p.won = false;
    score_t Top[5]={{"0",0,"0"},{"0",0,"0"},{"0",0,"0"},{"0",0,"0"},{"0",0,"0"}};
    rewind(fm->file);
    while(feof(fm->file)==0){
        if(!_load_player(fm,&p) && p.won!=false){
            for(i=0;i<5;i++){
                if(Top[i].score!=0){
                    if(p.score.score<Top[i].score){
                        Uint8 j=0;
                        for(j=4;j>i;j--){
                            Top[j]=Top[j-1];
                        }
                        Top[i]=p.score; 
                        break;
                    }
                }else{
                    Top[i]=p.score;
                    break;
                }
            }
        }
    }
    score_t* T = malloc(sizeof(score_t)*5);
    if(T==NULL){
        printf("\nTop_creat5()::failed to allocate memory.");
        return NULL;}
    for(i=0;i<5;i++){
        T[i]=Top[i];
    }
    return T;
}
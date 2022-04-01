#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<stdbool.h>

#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>
#include<SDL2/SDL_ttf.h>

#include"matrix.h"
#include"file.h"

#define WIN_WIDTH 480
#define WIN_HEIGHT 720
#define FPS 62
#define Title "Bring Up"
#define Icon "src/images/icon.png"
#define font01 "src/fonts/RobotoMono-Light.ttf"
#define font01_size 32
#define SaveFile "src/BringUp_save.dat"
#define font02 "src/fonts/Roboto-Regular.ttf"
#define font02_size 16
#define img01 "src/images/rec01.png"
#define img02 "src/images/rec02.png"
#define img03 "src/images/rec03.png"
#define img04 "src/images/rec04.png"
#define img05 "src/images/rec05.png"
#define img06 "src/images/rec06.png"
#define map1 "src/images/hehe.png"
#define Player_size 340

#pragma region Type----

typedef struct {
    SDL_Color clr;
    SDL_Rect rect;
    SDL_Texture *txtu;
    char* text;
}text_t;

typedef struct {
    SDL_Texture *txtu;
    SDL_Rect rect;
}image_t;

typedef struct {
    SDL_Color clr;
    SDL_Rect rect;
    bool pressed;
}button_t;

struct list{
    SDL_Texture** t;
    struct list* next;
}; typedef struct list list_t;

#pragma endregion

#pragma region Functions----

int String_to_int(const char* T){
    int a=0,i=0;
    if(T[0]!='\0'&&(T[0]>'0'&&T[0]<'9')){
        while(T[i]!='\0'){
            if(T[i]>'0'&&T[i]<'9'){
                a += (int)((T[i]-48)*10*i);
            }
        }
    }else{
        return -1;
    }
    return a;
}

char* get_date(char (*buffer)[]){

    time_t timer;
    struct tm* tm_info;

    timer = time(NULL);
    tm_info = localtime(&timer);

    strftime(*buffer, 26, "%Y-%m-%d %H:%M:%S", tm_info);
    return *buffer;
}

char* NewString(const char* T){
    char* S = malloc(sizeof(T)); strcpy(S,T);
    return S;
}
//prints errors after function a is called.
void Get_Err(int a, int success,const char* tag){
    if(a!=success){
        if(tag=="SDL"){
            printf("\n%s",SDL_GetError);
        }else{
            if(tag=="TTF"){
                printf("\n%s",TTF_GetError);
            }else{
                if(tag=="IMG"){
                    printf("\n%s",IMG_GetError);
                }
            }
        }
    }
}
//compact way to edit a rect struct.
void Rect_edit(SDL_Rect* rect,int x,int y,int w,int h){
    rect->x=x;rect->y=y;rect->w=w;rect->h=h;
}
void Rect_Center(SDL_Rect* rect,SDL_Rect* Drect){
    if(rect!=NULL)
    {if(Drect==NULL){
        rect->x=WIN_WIDTH/2-rect->w/2;
        rect->y=WIN_HEIGHT/2-rect->h/2;
    }else{
        rect->x=Drect->x+Drect->w/2-rect->w/2;
        rect->y=Drect->y+Drect->h/2-rect->h/2;
    }}
}
/**
 * Time_to_strings milleseconds to  MMM : SS : MSS text format.
 * \name Time_to_string()
 * \param time Uint32 representing milleseconds usuaaly obtained from SDL_GetTicks().
 * \returns MMM : SS : MSS formated string.
 * */
char* Time_to_string(Uint32 time, char** S){
    int ms,min,sec;
    min = (int)(time/60000);
    sec=(int)(time%60000/1000);
    ms =(int)(time%60000%1000);
    const char T[] = {'0'+min/100%10,'0'+min%100/10%10 ,'0'+min%10 ,':','0'+sec/10%10 ,'0'+sec%10%10 ,'.','0'+ms/100%10,'0'+ms%100/10%10,'0'+ms%10,'\0'};
    strcpy(*S,T);
    return *S;
}

bool Texture_Clear(SDL_Renderer *r,SDL_Texture *target,SDL_Rect *rect){
    if(SDL_SetTextureBlendMode(target,SDL_BLENDMODE_NONE)){return 1;}
    if(SDL_SetRenderTarget(r,target)){return 1;}
    if(SDL_SetRenderDrawBlendMode(r,SDL_BLENDMODE_NONE)){return 1;}
    if(SDL_SetRenderDrawColor(r,0,0,0,0)){return 1;}
    SDL_RenderFillRect(r,rect);
    SDL_RenderPresent(r);
    if(SDL_SetRenderTarget(r,NULL)){return 1;}
    if(SDL_SetRenderDrawBlendMode(r,SDL_BLENDMODE_BLEND)){return 1;}
    if(SDL_SetTextureBlendMode(target,SDL_BLENDMODE_BLEND)){return 1;}
    return 0;    
}

/**
 * Renders shape from map to target texture or window for NULL target.
 * \returns 0 on succes, 1 on else.
 * */
int Gshape(SDL_Renderer *r,SDL_Texture *target,SDL_Texture *map,int s,int i,int j){
    if(map ==NULL){
        printf("\nGShape()::Source texture missing.");
        return 1;
    }
    SDL_SetRenderTarget(r,target);
    SDL_Rect srect,drect;
    switch(s){
        case 0:case 1:case 2:case 3:case 4: 
            Rect_edit(&srect,0+20*s,0,20,20); Rect_edit(&drect,60+j*60-10,60+i*60-10,20,20);
        break;
        //case 5:empty for no diag.
        case 6: Rect_edit(&srect,60,20,60,60); Rect_edit(&drect,60+j*60,60+i*60,60,60);
        break;
        case 7: Rect_edit(&srect,0,20,60,60); Rect_edit(&drect,60+j*60,60+i*60,60,60);
        break;
    }
    SDL_RenderCopy(r,map,&srect,&drect);
    if(target!=NULL)SDL_RenderPresent(r);
    SDL_SetRenderTarget(r,NULL);
    return 0;
}

//Renders specified grid to screen, dep=0 for 7x7 , dep=5 for 6x6 matrices.
int Render_Grid(SDL_Renderer *r,SDL_Texture *target,SDL_Texture *map,int **SJ,int N,Uint8 dep){
    if(map==NULL){
        printf("\nRender_Grid()::Source texture missing.");
        return 1;
    }
    int i,j;
    for(i=0;i<N;i++){
        for(j=0;j<N;j++){
            if(Gshape(r,target,map,SJ[i][j]+dep,i,j)!=0){
                printf("\n Render_Grid()::Failed to render shape %d , pos: %d , %d .",SJ[i][j]+dep,j,i);
            }
        }
    }
    return 0;
}

int Render(SDL_Renderer *r,SDL_Texture *target,SDL_Texture *source,SDL_Rect* rect){
    if(source==NULL){
        printf("\nRender()::source texture missing");
        return 1;
    }else if(target==NULL){
        SDL_RenderCopy(r,source,NULL,rect);
        SDL_RenderPresent(r);
        return 0;
    }else{
        SDL_SetRenderTarget(r,target);
        SDL_RenderCopy(r,source,NULL,rect);
        SDL_RenderPresent(r);
        SDL_SetRenderTarget(r,NULL);
        return 0;
    }
}

SDL_Texture* Text_CreateTexture(SDL_Renderer *r,TTF_Font *font,text_t *text,SDL_Surface *Load_srfc){

    Load_srfc = TTF_RenderUTF8_Blended(font,text->text,text->clr);
    if(Load_srfc==NULL){
        printf("\n%s",TTF_GetError());
        return NULL;
    }
    text->txtu = SDL_CreateTextureFromSurface(r,Load_srfc);
    if(text->txtu==NULL){
        SDL_FreeSurface(Load_srfc); Load_srfc = NULL;
        printf("\n%s",SDL_GetError());
        return NULL;
    }
    SDL_SetTextureBlendMode(text->txtu,SDL_BLENDMODE_BLEND);
    SDL_QueryTexture(text->txtu,NULL,NULL,&text->rect.w,&text->rect.h);
    SDL_FreeSurface(Load_srfc); Load_srfc = NULL;
    return text->txtu;
}
SDL_Texture* Text_CreateTexture_Wrapped(SDL_Renderer *r,TTF_Font *font,text_t *text,SDL_Surface *Load_srfc,int N){

    Load_srfc = TTF_RenderUTF8_Blended_Wrapped(font,text->text,text->clr,N);
    if(Load_srfc==NULL){
        printf("\n%s",TTF_GetError());
        return NULL;
    }
    text->txtu = SDL_CreateTextureFromSurface(r,Load_srfc);
    if(text->txtu==NULL){
        SDL_FreeSurface(Load_srfc); Load_srfc = NULL;
        printf("\n%s",SDL_GetError());
        return NULL;
    }
    SDL_SetTextureBlendMode(text->txtu,SDL_BLENDMODE_BLEND);
    SDL_QueryTexture(text->txtu,NULL,NULL,&text->rect.w,&text->rect.h);
    SDL_FreeSurface(Load_srfc); Load_srfc = NULL;
    return text->txtu;
}

int Text_RenderNew(SDL_Renderer *r,SDL_Texture *target,TTF_Font *font,text_t *text,SDL_Surface *Load_srfc){
    if(strlen(text->text)<=0){
        printf("\nText_RednderNew()::Text has zero length.");
        return 1;
    }
    if(Render(r,target,Text_CreateTexture(r,font,text,Load_srfc),&text->rect)!=0){
        printf("\nText_Render():: Texture Render failed .");
        return 1;
    }
    SDL_DestroyTexture(text->txtu);
    text->txtu=NULL;
    return 0;
}
int Text_RenderNew_Wrapped(SDL_Renderer *r,SDL_Texture *target,TTF_Font *font,text_t *text,SDL_Surface *Load_srfc,int wrap){
    if(Render(r,target,Text_CreateTexture_Wrapped(r,font,text,Load_srfc,wrap),&text->rect)!=0){
        printf("\nText_Render_Wrapped():: Texture Render failed .");
        return 1;
    }
    SDL_DestroyTexture(text->txtu);
    text->txtu=NULL;
    return 0;
}

char* Top_Render5(SDL_Renderer* r,TTF_Font* font,SDL_Texture* target,SDL_Surface* Load_srfc,score_t* T,size_t size){
    text_t text = {{135,204,255,255},{116,20,265,265},NULL,(char*)malloc(size)};
    int i; strcpy(text.text,"    TOP 5:\n");
    for(i=0;i<5;i++){
        sprintf(text.text+strlen(text.text),"%s: %d ,%s\n",T[i].name,T[i].score,T[i].date);
    }
    Text_RenderNew_Wrapped(r,target,font,&text,Load_srfc,348);
    SDL_DestroyTexture(text.txtu);
    return text.text;
}

void Text_Input_process_event(SDL_Event *e,text_t *text,size_t maxlength){
    if(e->type == SDL_TEXTINPUT || e->type == SDL_KEYDOWN){
        if(e->type == SDL_KEYDOWN && e->key.keysym.scancode == SDL_SCANCODE_BACKSPACE && strlen(text->text)>0){
            text->text[strlen(text->text)-1]=text->text[strlen(text->text)];
        }else if(e->type == SDL_TEXTINPUT && strlen(text->text)<maxlength){
            printf("%s\n",e->text.text);
            strcat(text->text,e->text.text);
        }
    }
}
void Text_Handle_Input(SDL_Renderer *r,text_t *text,TTF_Font *font,SDL_Surface *Load_srfc,SDL_Texture *back,bool* Q){
    #ifndef FPS
        #define FPS 62
    #endif
    #ifndef WIN_WIDTH
        #define WIN_WIDTH 480
    #endif
    #ifndef WIN_HEIGHT
        #define WIN_HEIGHT 720
    #endif
    

    image_t frgrd = {SDL_CreateTexture(r,SDL_PIXELFORMAT_RGBA8888,SDL_TEXTUREACCESS_TARGET,WIN_WIDTH,WIN_HEIGHT),{0,0,WIN_WIDTH,WIN_HEIGHT}};
        Texture_Clear(r,frgrd.txtu,NULL);

    bool quit=0;
    SDL_StartTextInput();
        printf("\n");
    Uint32 count;
    while(!quit){
        count = SDL_GetTicks();
        SDL_Event e;
        while(SDL_PollEvent(&e)){
            if(e.type == SDL_QUIT ||
            (e.type == SDL_WINDOWEVENT && e.window.event == SDL_WINDOWEVENT_CLOSE) ||
            (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE)) {
                strcpy(text->text,"");
                quit = 1;
                *Q=1;
            }else if(e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_RETURN){
                quit = 1;
            }
            Text_Input_process_event(&e,text,20);
        }
        
        SDL_RenderCopy(r,back,NULL,NULL);
        SDL_RenderCopy(r,frgrd.txtu,NULL,NULL);
        Texture_Clear(r,frgrd.txtu,&text->rect);
        if(strlen(text->text)>0){
            Text_RenderNew(r,frgrd.txtu,font,text,Load_srfc);
        }
        
        
        if((1000/FPS)>(SDL_GetTicks()-count)){
                //Delay(%16)~62FPS
                SDL_Delay((1000/FPS)-(SDL_GetTicks()-count));
        }
        SDL_RenderPresent(r);
        
    }
    if(SDL_IsTextInputActive)SDL_StopTextInput;
    SDL_DestroyTexture(frgrd.txtu);
}

bool Score_text_load(file_t *fm, char (*buff)[]){
    player_t p;
    (*buff)[0]='\0';
    if(!_load_player(fm,&p)){
        sprintf(*buff+strlen(*buff),"%s : ",p.score.name);
        sprintf(*buff+strlen(*buff),"%d ,",p.score.score);
        (p.won)?sprintf(*buff+strlen(*buff),"WON\n"):sprintf(*buff+strlen(*buff),"nWON");
        sprintf(*buff+strlen(*buff),"%s|-id_",p.score.date);
        sprintf(*buff+strlen(*buff),"%d",p.id);
        return 1;
    }else return 0;
}

void Score_Init_tab(image_t (*scrb)[],int x,int y,int w,int h){
    int i;
    for(i=0;i<12;i++){
        (*scrb)[i].txtu=NULL;
        Rect_edit(&(*scrb)[i].rect,x,y+h*i+7*i,w,h);
    }
}

Uint8 Score_load_tab(SDL_Renderer *r,TTF_Font *font,SDL_Surface *Load_srfc,image_t (*scrb)[],file_t *fm){
    rewind(fm->file);
    int i=0;
    text_t text = {{135,204,255,255},{(*scrb)[0].rect.x,(*scrb)[0].rect.y,(*scrb)[0].rect.w,(*scrb)[0].rect.h},NULL,(char*)malloc(70)};
    char buff[70];
    while(Score_text_load(fm,&buff) && i<12){
        strcpy(text.text,buff);
        Rect_edit(&(*scrb)[i].rect,
            (*scrb)[0].rect.x,(*scrb)[0].rect.y+(*scrb)[0].rect.h*i+7*i,
            (*scrb)[0].rect.w,(*scrb)[0].rect.h);
        (*scrb)[i].txtu = Text_CreateTexture_Wrapped(r,font,&text,Load_srfc,436);
        i++;
    }
    free(text.text);
    return (Uint8)i;
}

void Score_render(SDL_Renderer *r,SDL_Texture *target,TTF_Font *font,SDL_Surface* Load_srfc,image_t (*scrb)[]){
    int i=0;
    while((*scrb)[i].txtu!=NULL){
        Render(r,target,(*scrb)[i].txtu,&(*scrb)[i].rect);
        i++;
    }
}


//Renders button to screen if and returns pressed or not pressed.
static bool button(SDL_Renderer *r,button_t *btn,bool draw){
    if(draw && !btn->pressed){
        SDL_SetRenderDrawColor(r,btn->clr.r,btn->clr.g,btn->clr.b,btn->clr.a);
        SDL_RenderFillRect(r,&btn->rect);
    }

    if(btn->pressed == true){
        btn->pressed = false;
        return true;
    }
    return false;
}

//Check if button is pressed, and pass it to btn.pressed bool.
void button_process_event(SDL_Event *e,button_t *btn){
    if(e->type == SDL_MOUSEBUTTONDOWN && e->button.button == SDL_BUTTON_LEFT && 
        e->button.y >= btn->rect.y && e->button.y <= btn->rect.y+btn->rect.h &&
        e->button.x >= btn->rect.x && e->button.x <= btn->rect.x+btn->rect.w){
            btn->pressed = true;
        }
}

//Reacts to clickOnCell and iterates between the diags.
static void grid_process_event(int*** SJ,SDL_Event* e){
    if(e->type==SDL_MOUSEBUTTONDOWN && e->button.button == SDL_BUTTON_LEFT 
    &&e->button.x>=60 && e->button.x<=420 && e->button.y>=60 && e->button.y<=420){
        (*SJ)[(e->button.y-60)/60][(e->button.x-60)/60]=
            ((*SJ)[(e->button.y-60)/60][(e->button.x-60)/60]+1)%3;
    }
}                                                                               

#pragma endregion

int main(int argc, char* argv[]){

    #pragma region Init------

    if(SDL_Init(SDL_INIT_EVERYTHING)!=0){
        printf("\nSDL_Init_Error:: %s",SDL_GetError());
        return 1;
    }
    if(TTF_Init()!=0){
        printf("\nTTF_Init_Error:: %s",TTF_GetError());
        SDL_Quit();
        return 1;
    }
    if(!IMG_Init(IMG_INIT_PNG)){
        printf("\nIMG_Init_Error:: %s",IMG_GetError());
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow(Title,SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,WIN_WIDTH,WIN_HEIGHT,0);
    if(window==NULL){
        printf("\nWindow_Init_Error:: %s",SDL_GetError());
        IMG_Quit();
        TTF_Quit();
        SDL_Quit();
        return 1;
    }
    SDL_Renderer *renderer = SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED|SDL_RENDERER_TARGETTEXTURE);
    if(renderer==NULL){
        printf("\nRenderer_Init_Error:: %s",SDL_GetError());
        SDL_DestroyWindow(window);
        IMG_Quit();
        TTF_Quit();
        SDL_Quit();
        return 1;
    }SDL_SetRenderDrawBlendMode(renderer,SDL_BLENDMODE_BLEND);
    //fonts
    TTF_Font *font1 = TTF_OpenFont(font01,font01_size);
    if(font1==NULL){
        printf("\nFont_Init_Error:: %s",TTF_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        IMG_Quit();
        TTF_Quit();
        SDL_Quit();
        return 1;
    }
    if(TTF_GetFontKerning(font1)==0)TTF_SetFontKerning(font1,1);

    TTF_Font *font2 = TTF_OpenFont(font02,font02_size);
    if(font2==NULL){
        printf("\nFont_Init_Error:: %s",TTF_GetError());
        TTF_CloseFont(font1);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        IMG_Quit();
        TTF_Quit();
        SDL_Quit();
        return 1;
    }
    if(TTF_GetFontKerning(font2)==0)TTF_SetFontKerning(font2,1);

    file_t file1 = openfile(SaveFile,"a+");
    if(!file1.file){
        printf("\nError opening save file");
    }

    player_t Player = {0,false,{"",0,""}};
    score_t *top = NULL;

    SDL_Surface *Load_srfc = NULL;
    SDL_SetSurfaceBlendMode(Load_srfc,SDL_BLENDMODE_BLEND);

    //Window Icon:
    SDL_SetWindowIcon(window,IMG_Load(Icon));

    #pragma endregion

    int **T,**S,**SJ;

    printf("%d",sizeof(player_t));

    image_t map = {IMG_LoadTexture(renderer,map1),{0,0,120,80}};

    image_t frgrd = {SDL_CreateTexture(renderer,SDL_PIXELFORMAT_RGBA8888,SDL_TEXTUREACCESS_TARGET,WIN_WIDTH,WIN_HEIGHT),{0,0,WIN_WIDTH,WIN_HEIGHT}};
    Texture_Clear(renderer,frgrd.txtu,NULL);
    
    //game screen.
    image_t bckgrd01 = {IMG_LoadTexture(renderer,img01),{0,0,WIN_WIDTH,WIN_HEIGHT}};

    //menu screen.
    image_t bckgrd02 = {IMG_LoadTexture(renderer,img02),{0,0,WIN_WIDTH,WIN_HEIGHT}};

    //game stop screen.
    image_t bckgrd03 = {IMG_LoadTexture(renderer,img03),{0,0,WIN_WIDTH,WIN_HEIGHT}};

    image_t bckgrd04 = {IMG_LoadTexture(renderer,img04),{0,0,WIN_WIDTH,WIN_HEIGHT}};

    image_t bckgrd05 = {IMG_LoadTexture(renderer,img05),{0,0,WIN_WIDTH,WIN_HEIGHT}};

    image_t bckgrd06 = {IMG_LoadTexture(renderer,img06),{0,0,WIN_WIDTH,WIN_HEIGHT}};

    image_t scrbrd[12];
        Score_Init_tab(&scrbrd,26,26,436,38);
        Uint8 scoretabcount=0;

    //timer text.(Dynamic)
    text_t text_time = {{26,26,26,255},{170,583,265,60},NULL,(char*)malloc(12)};
    //game stop text.(Dynamic)
    text_t text_stop = {{135,204,255,255},{100,180,265,120},NULL,(char*)malloc(strlen("You won.\n score: 123456")+1)};

    text_t text_save = {{26,26,26,255},{138,422,265,60},NULL,"SAVE & QUIT"};

    //(Dynamic)
    text_t text_input1 = {{26,150,220,255},{122,422,265,60},NULL,(char*)malloc(24)};

    //button #1.
    button_t btn_menu = {{135,204,255,255},{107,335,265,60},false};
    //button #2.
    button_t btn_start = {{135,204,255,255},{107,415,265,60},false};
    //button #3.
    button_t btn_cont = {{135,204,255,255},{107,495,265,60},false};
    //button #4.
    button_t btn_score = {{135,204,255,255},{107,575,265,60},false};

    button_t btn_clear = {{135,204,255,255},{170,495,55,55},false};
    button_t btn_pause = {{135,204,255,255},{260,495,55,55},false};
    button_t btn_up = {{135,204,255,255},{440,575,15,15},false};
    button_t btn_down = {{135,204,255,255},{440,600,15,15},false};

    Uint32 count,start,elapsed=0;

    enum {STATE_IN_MENU,STATE_IN_GAME,STATE_WON,STATE_PAUSED,STATE_IN_LOAD,STATE_IN_SCORE} state=0;

    bool quit = 0;

    while(!quit){
        count = SDL_GetTicks();
        SDL_Event evt;
        while(SDL_PollEvent(&evt)){
            // quit on close, window close, or 'escape' key hit
            if(evt.type == SDL_QUIT ||
            (evt.type == SDL_WINDOWEVENT && evt.window.event == SDL_WINDOWEVENT_CLOSE) ||
            (evt.type == SDL_KEYDOWN && evt.key.keysym.sym == SDLK_ESCAPE)) {
                quit = 1;
            }
            switch(state){
                case STATE_IN_MENU:
                    button_process_event(&evt,&btn_start);
                    button_process_event(&evt,&btn_cont);
                    button_process_event(&evt,&btn_score);
                break;
                case STATE_IN_GAME:
                    grid_process_event(&SJ,&evt);
                    button_process_event(&evt,&btn_pause);
                    button_process_event(&evt,&btn_clear);
                break;
                case STATE_WON:
                    button_process_event(&evt,&btn_start);
                    button_process_event(&evt,&btn_cont);
                    button_process_event(&evt,&btn_score);
                break;
                case STATE_PAUSED:
                    button_process_event(&evt,&btn_start);
                    button_process_event(&evt,&btn_cont);
                    button_process_event(&evt,&btn_score);
                break;
                case STATE_IN_LOAD:
                    button_process_event(&evt,&btn_menu);
                    button_process_event(&evt,&btn_start);
                    button_process_event(&evt,&btn_cont);
                    button_process_event(&evt,&btn_score);
                break;
                case STATE_IN_SCORE:
                    button_process_event(&evt,&btn_up);
                    button_process_event(&evt,&btn_down);
                    button_process_event(&evt,&btn_score);
                break;
                default:
            }
        }
        SDL_SetRenderDrawColor(renderer,26,26,26,255);
        SDL_RenderClear(renderer);

    
        switch(state){
            case STATE_IN_MENU:
                Render(renderer,NULL,bckgrd02.txtu,NULL);
                //SATRT.
                if(button(renderer,&btn_start,0)){
                    state = STATE_IN_GAME;
                    if(Grids_Init(&T,&S,&SJ))printf("\nErr.");
                    New_player_id(&file1,&Player);
                        Player.won = false;
                        get_date(&Player.score.date);
                    MatnCpy(6,S,&Player.game);
                    Texture_Clear(renderer,frgrd.txtu,NULL);
                    Render_Grid(renderer,frgrd.txtu,map.txtu,T,7,0);
                    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION,"Bring Up","game starts now..",window);
                    elapsed = 0;
                }
                //CONTINUE.
                if(button(renderer,&btn_cont,0)){
                    state=STATE_IN_LOAD;
                    Texture_Clear(renderer,frgrd.txtu,NULL);
                    Render(renderer,frgrd.txtu,bckgrd04.txtu,NULL);
                }
                //SCOREBOARD.
                if(button(renderer,&btn_score,0)){
                    state = STATE_IN_SCORE;
                    Texture_Clear(renderer,frgrd.txtu,NULL);
                    scoretabcount = Score_load_tab(renderer,font2,Load_srfc,&scrbrd,&file1);
                    Score_render(renderer,frgrd.txtu,font2,Load_srfc,&scrbrd);
                }
            break;
            case STATE_IN_GAME:
                elapsed += SDL_GetTicks()-start;
                start = SDL_GetTicks();
                SDL_RenderCopy(renderer,bckgrd01.txtu,NULL,NULL);
                Render_Grid(renderer,NULL,map.txtu,SJ,6,5);
                SDL_RenderCopy(renderer,frgrd.txtu,NULL,&frgrd.rect);
                //render time.
                text_time.text = Time_to_string(elapsed,&text_time.text);
                Text_RenderNew(renderer,NULL,font1,&text_time,Load_srfc);
                //CLEAR.
                if(button(renderer,&btn_clear,0)){
                    RandMat(&SJ,6,0,0);
                }
                //PAUSE.
                if(button(renderer,&btn_pause,0)){
                    state = STATE_PAUSED;
                    Render(renderer,frgrd.txtu,bckgrd06.txtu,&bckgrd06.rect);
                    Player.won = false;
                        Player.score.score = elapsed/1000;
                        MatnCpy(6,SJ,&Player.SJ);MatnCpy(6,S,&Player.game);
                    sprintf(text_stop.text,"PAUSED.\ntime: %ds",elapsed/1000%1000000);
                    Texture_Clear(renderer,frgrd.txtu,NULL);
                    Text_RenderNew_Wrapped(renderer,frgrd.txtu,font1,&text_stop,Load_srfc,260);
                    text_stop.text[0]='\0';
                }
                if(MatCmp(S,SJ,6)){
                    state = STATE_WON;
                    Render(renderer,frgrd.txtu,bckgrd03.txtu,&bckgrd03.rect);
                    Player.won = true;
                        Player.score.score = elapsed/1000;
                        MatnCpy(6,SJ,&Player.SJ);MatnCpy(6,S,&Player.game);
                        get_date(&Player.score.date);
                    sprintf(text_stop.text,"You won.\nscore: %d",elapsed/1000%1000000);
                    Texture_Clear(renderer,frgrd.txtu,NULL);
                    Text_RenderNew_Wrapped(renderer,frgrd.txtu,font1,&text_stop,Load_srfc,260);
                    text_stop.text[0]='\0';
                }

            break;
            case STATE_WON:
                SDL_RenderCopy(renderer,frgrd.txtu,NULL,&frgrd.rect);
                //SAVE & QUIT.
                if(button(renderer,&btn_start,1)){
                    Text_Handle_Input(renderer,&text_input1,font1,Load_srfc,frgrd.txtu,&quit);
                    if(!quit && strlen(text_input1.text)>0){
                        //save game.
                    }
                }else Text_RenderNew(renderer,NULL,font1,&text_save,Load_srfc);
                //MAIN MENU.
                if(button(renderer,&btn_cont,0)){
                    state = STATE_IN_MENU;
                }
                //REPLAY.
                if(button(renderer,&btn_score,0)){
                    //new game.
                    state = STATE_IN_GAME;
                    FreeMat(&T,7);FreeMat(&S,6);FreeMat(&SJ,6);
                    if(Grids_Init(&T,&S,&SJ))printf("\nErr.");
                    Player.won = false;
                        New_player_id(&file1,&Player);
                        MatnCpy(6,S,&Player.game);
                    Texture_Clear(renderer,frgrd.txtu,NULL);
                    Render_Grid(renderer,frgrd.txtu,map.txtu,T,7,0);
                    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION,"Bring Up","game starts now..",window);
                    elapsed = 0;
                }
            break;
            case STATE_PAUSED:
                SDL_RenderCopy(renderer,frgrd.txtu,NULL,&frgrd.rect);
                //SAVE & QUIT.
                if(button(renderer,&btn_start,1)){
                    Text_Handle_Input(renderer,&text_input1,font1,Load_srfc,frgrd.txtu,&quit);
                    if(!quit && strlen(text_input1.text)>0){
                        //save game.
                    }
                }else Text_RenderNew(renderer,NULL,font1,&text_save,Load_srfc);
                //MAIN MENU.
                if(button(renderer,&btn_cont,0)){
                    state = STATE_IN_MENU;
                }
                //RESUME.
                if(button(renderer,&btn_score,0)){
                    //new game.
                    state = STATE_IN_GAME;
                    Texture_Clear(renderer,frgrd.txtu,NULL);
                    Render_Grid(renderer,frgrd.txtu,map.txtu,T,7,0);
                }
            break;
            case STATE_IN_LOAD:
                SDL_RenderCopy(renderer,frgrd.txtu,NULL,NULL);
                //MAIN MENU.
                if(button(renderer,&btn_menu,0)){
                    state = STATE_IN_MENU;
                }
                //LOAD.
                if(button(renderer,&btn_start,0)||button(renderer,&btn_cont,0)){
                    //Load game.
                }
                //SCOREBOARD.
                if(button(renderer,&btn_score,0)){
                    state = STATE_IN_SCORE;
                    Texture_Clear(renderer,frgrd.txtu,NULL);
                    scoretabcount = Score_load_tab(renderer,font2,Load_srfc,&scrbrd,&file1);
                    Score_render(renderer,frgrd.txtu,font2,Load_srfc,&scrbrd);
                }
            break;
            case STATE_IN_SCORE:
                SDL_RenderCopy(renderer,bckgrd05.txtu,NULL,NULL);
                SDL_RenderCopy(renderer,frgrd.txtu,NULL,NULL);
                if(button(renderer,&btn_score,0)){
                    Texture_Clear(renderer,frgrd.txtu,&frgrd.rect);
                    state = STATE_IN_MENU;
                }
                if(button(renderer,&btn_up,0)){
                    //Score_scroll_up();
                }
                if(button(renderer,&btn_down,0)){
                    //Score_scroll_down();
                }
            break;
            default:
        }

        

        SDL_RenderPresent(renderer);
        if((1000/FPS)>(SDL_GetTicks()-count)){
            //Delay(%16)~62FPS
            SDL_Delay((1000/FPS)-(SDL_GetTicks()-count));
        }
    }
    
    #pragma region Destroy

    closefile(&file1);
    TTF_CloseFont(font2);
    TTF_CloseFont(font1);
    SDL_FreeSurface(Load_srfc);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    TTF_Quit();
    SDL_Quit();
    printf("\nSuccess.");
    
    #pragma endregion
    return 0;
}
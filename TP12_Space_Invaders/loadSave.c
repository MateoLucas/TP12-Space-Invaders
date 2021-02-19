/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "loadSave.h"
#include "aliens.h"
#include "ship.h"
#include "muro.h"
#include "shots.h"
#include "fx.h"
extern long score;

int save_game()
{
    FILE *pSave = fopen("saved_game.txt", "w");
    
    //score  save
    fputc(ESTRUCTURA,pSave);//importante que empiece con ESTRUCTURA
    fprintf(pSave,"%ld",score);
    fputc(ESTRUCTURA,pSave);
    
    //alien save
    fputc(ESTRUCTURA, pSave);
    int i;
    int bug_type;
    
    fprintf(pSave,"%d",mother.x);
    fputc(INTRA_ESTRUC,pSave);
    fprintf(pSave,"%d",mother.y);
    fputc(INTRA_ESTRUC,pSave);
    fprintf(pSave,"%d",mother.life);
    fputc(INTRA_ESTRUC,pSave);//el type siempre es thiccboi
    fprintf(pSave,"%c", mother.used ? 't' : 'f');
    
    for(i=0; i < ALIENS_N ;i++)
    {
        fprintf(pSave,"%d",aliens[i].x);
        fputc(INTRA_ESTRUC,pSave);
        fprintf(pSave,"%d",aliens[i].y);
        fputc(INTRA_ESTRUC,pSave);
        switch (aliens[i].type)
        {
            case ALIEN_TYPE_BUG:
                fprintf(pSave,"%c",'b');
                break;
               
            case ALIEN_TYPE_THICCBOI:
               fprintf(pSave,"%c",'t');
                break;
        }
        
        fputc(INTRA_ESTRUC,pSave);
        fprintf(pSave,"%d",aliens[i].shot_timer);
        fputc(INTRA_ESTRUC,pSave);
        fprintf(pSave,"%d",aliens[i].blink);
        fputc(INTRA_ESTRUC,pSave);
        fprintf(pSave,"%d",aliens[i].life);
        fputc(INTRA_ESTRUC,pSave);
        fprintf(pSave,"%c", aliens[i].used ? 't' : 'f');
        fputc(ESTRUCTURA,pSave);
    }
    
    //ship save
    fputc(ESTRUCTURA,pSave);
    fprintf(pSave,"%d",ship.x);
    fputc(INTRA_ESTRUC,pSave);
    fprintf(pSave,"%d",ship.y);
    fputc(INTRA_ESTRUC,pSave); 
    fprintf(pSave,"%d",ship.shot_timer);
    fputc(INTRA_ESTRUC,pSave);
    fprintf(pSave,"%d",ship.lives);
    fputc(INTRA_ESTRUC,pSave);
    fprintf(pSave,"%d",ship.respawn_timer);
    fputc(INTRA_ESTRUC,pSave);
    fprintf(pSave,"%d",ship.invincible_timer);
    fputc(INTRA_ESTRUC,pSave);
    printf(pSave, "%c", ships_one_shot ? 't' : 'f');
    fputc(ESTRUCTURA,pSave);
    //shots
    for(i=0; i < SHOTS_N;i++)
    {
        fprintf(pSave, "%d",shots[i].x);
        fputc(INTRA_ESTRUC,pSave);
        fprintf(pSave, "%d",shots[i].y);
        fputc(INTRA_ESTRUC,pSave);
        fprintf(pSave, "%d",shots[i].dx);
        fputc(INTRA_ESTRUC,pSave);
        fprintf(pSave, "%d",shots[i].dx);
        fputc(INTRA_ESTRUC,pSave);
        fprintf(pSave, "%d",shots[i].frame);
        fputc(INTRA_ESTRUC,pSave);
        fprintf(pSave,"%c", shots[i].ship ? 't' : 'f');
        fputc(INTRA_ESTRUC,pSave);
        fprintf(pSave,"%c", shots[i].used ? 't' : 'f');
        fputc(INTRA_ESTRUC,pSave);
    }
    //fx
    fputc(ESTRUCTURA,pSave);
    for(i=0; i < FX_N;i++)
    {
        fprintf(pSave, "%d",fx[i].x);
        fputc(INTRA_ESTRUC,pSave);
        fprintf(pSave, "%d",fx[i].y);
        fputc(INTRA_ESTRUC,pSave);
        fprintf(pSave,"%c", fx[i].spark ? 't' : 'f');
        fputc(INTRA_ESTRUC,pSave);
        fprintf(pSave,"%c", fx[i].used ? 't' : 'f');
        fputc(INTRA_ESTRUC,pSave);
        
    }
    //muros
    fputc(ESTRUCTURA,pSave);
    for(i=0 ; i < MURO_N ; i++)//x,y,life btyple
    {
        fprintf(pSave, "%d",muro_arr[i].x);
        fputc(INTRA_ESTRUC,pSave);
        fprintf(pSave, "%d",muro_arr[i].y);
        fputc(INTRA_ESTRUC,pSave);
        fprintf(pSave, "%d",muro_arr[i].life);
        fputc(INTRA_ESTRUC,pSave);
        fprintf(pSave,"%c", muro_arr[i].used ? 't' : 'f');
        fputc(INTRA_ESTRUC,pSave);
    }
    //aliens_m
    fputc(ESTRUCTURA,pSave);
    
    fputc(END_LOAD,pSave);
    fflush(pSave);
    int check= fclose(pSave);
    return check;
}
int load_game()
{
    FILE *pLoad = fopen("saved_game.txt","r");
    char load = fgetc(pLoad);
    if((load != NULL)&&(load == ESTRUCTURA))
    {
        //score
        fscanf(pLoad,"%ld",&score);
        load = fgetc(pLoad);//ESTRUCTURA
        //aliens
        int i;
        for(i=0;i<= ALIENS_N-1;i++)
        {
            fscanf(pLoad, "%d",&aliens[i].x);//x
            load = fgetc(pLoad);// INTRA_STRUCT
            fscanf(pLoad, "%d",&aliens[i].y);//y
            load = fgetc(pLoad);// INTRA_STRUCT
            load = fgetc(pLoad);
            switch (load)
            {
                    case 'b':
                        aliens[i].type = ALIEN_TYPE_BUG;
                        break;
                    case 't':
                        aliens[i].type = ALIEN_TYPE_THICCBOI;
                        break;
            }
            load = fgetc(pLoad);// INTRA_STRUCT
            fscanf(pLoad, "%d",&aliens[i].shot_timer);//shot_timer
            load = fgetc(pLoad);// INTRA_STRUCT
            fscanf(pLoad, "%d",&aliens[i].blink);//blink
            load = fgetc(pLoad);// INTRA_STRUCT
            fscanf(pLoad, "%d",&aliens[i].life);//lives
            load = fgetc(pLoad);// INTRA_STRUCT
            aliens[i].used = ((fgetc(pLoad)=='t') ? true : false);//used  
            load = fgetc(pLoad);//ESTRUCTURA
        }
        //ship
        load = fgetc(pLoad);//ESTRUCTURA
        fscanf(pLoad, "%d",&ship.x);//x
        load = fgetc(pLoad);// INTRA_STRUCT
        fscanf(pLoad, "%d",&ship.y);//y
        load = fgetc(pLoad);// INTRA_STRUCT
        fscanf(pLoad, "%d",&ship.shot_timer);//shot_timer
        load = fgetc(pLoad);// INTRA_STRUCT
        fscanf(pLoad, "%d",&ship.lives);//lives
        load = fgetc(pLoad);// INTRA_STRUCT
        fscanf(pLoad, "%d",&ship.respawn_timer);//respawn
        load = fgetc(pLoad);// INTRA_STRUCT
        fscanf(pLoad, "%d",&ship.invincible_timer);//lives
        load = fgetc(pLoad);// INTRA_STRUCT
        
        
        
    }
    int check = fclose(pLoad);
    return check;
}

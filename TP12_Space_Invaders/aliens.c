/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include <stdbool.h>
#include "aliens.h"
#include "otros.h"
#include "sprites.h"
#include "shots.h"
#include "fx.h"

int ALIEN_W[] = {14, 13, 45};
int ALIEN_H[] = { 9, 10, 27};

extern SPRITES sprites;

typedef enum ALIEN_TYPE
{
    ALIEN_TYPE_BUG = 0,
    ALIEN_TYPE_ARROW,
    ALIEN_TYPE_THICCBOI,
    ALIEN_TYPE_N
} ALIEN_TYPE;

typedef struct ALIEN
{
    int x, y;
    ALIEN_TYPE type;
    int shot_timer;
    int blink;
    int life;
    bool used;
} ALIEN;




ALIEN aliens[ALIENS_N];

//aliens[i].used = false;

void aliens_init()
{

    int t;
    int columnas = 0;
    for(t=0; t < ALIENS_N; t++)
    {
        int s = t/11;
        columnas++;
        aliens[t].used = true;
        aliens[t].type = ALIEN_TYPE_BUG;
        aliens[t].shot_timer = between(1, 99);
        aliens[t].blink = 0;
        aliens[t].life = 2;
        if(columnas >= N_COLS)
        {
            columnas=0;
        }
        aliens[t].x = REFERENCE_X + columnas*ALIEN_SPACE_BUFFER;
        aliens[t].y = REFERENCE_Y+ s* ALIEN_SPACE_BUFFER;
    }
}

/*
bool aliens_last(int alien_n)
{
    int i;
    bool answer=true;
    for(i=alien_n+1; i < ALIENS_N; i++)
    {
        if( aliens[i].used && (aliens[i].x == aliens[alien_n].x) &&(aliens[alien_n].y < aliens[i].y))
            answer = false;              
    }
    return answer;
}
*/
void aliens_update()
{   
    long score;
    int j;
    bool detect_border=false;
    bool static direction = false;//true = derecha, flase = izquierda
    int speed_2 = 1;
    int static n_frames = 0;
    int static speed = 100;
    n_frames++;
    if(n_frames >= (speed/10))
        n_frames = 0;
    if(speed <= 0)
        speed_2 = 2;
        
    for(j=0;j<ALIENS_N;j++)//while
    {
        if((aliens[j].used)&&((aliens[j].x >= BUFFER_W-ALIEN_BUG_W)||aliens[j].x<= 0))
        {
            detect_border=true;
        }
    }
    int i;
    if(detect_border)
    {
        if(direction)
        {
            direction = false;
            
        }else
        {
            direction = true;   
        }
    }
    
    for(i=0;i<ALIENS_N;i++)
    {
        if(aliens[i].used)
        {
            if(detect_border)
            {
                aliens[i].y += ALIEN_BUG_H/2;
                if(direction)
                {
                    aliens[i].x +=1;
                }else
                {
                    aliens[i].x -=1;
                }
                
            }
        
            if(direction)
            {
                aliens[i].x += speed_2*((n_frames == 0) ? 1 : 0);
            }else
            {
                aliens[i].x -= speed_2*((n_frames == 0) ? 1 : 0);
            }
        

            if(aliens[i].blink)
                aliens[i].blink--;

            if(shots_collide(false, aliens[i].x, aliens[i].y, ALIEN_W[aliens[i].type], ALIEN_H[aliens[i].type]))
            {
                aliens[i].life--;
                aliens[i].blink = 4;
            }
        
            int cx = aliens[i].x + (ALIEN_W[aliens[i].type] / 2);
            int cy = aliens[i].y + (ALIEN_H[aliens[i].type] / 2);
            if(aliens[i].life <= 0)
            {
                
                fx_add(false, cx, cy);
                speed-=3;
                switch(aliens[i].type)
                {
                case ALIEN_TYPE_BUG:
                    score += 200;
                    break;

                case ALIEN_TYPE_ARROW:
                    score += 150;
                    break;

                case ALIEN_TYPE_THICCBOI:
                    score += 800;
                    fx_add(false, cx-10, cy-4);
                    fx_add(false, cx+4, cy+10);
                    fx_add(false, cx+8, cy+8);
                    break;
                }

                aliens[i].used = false;
                continue;
            }   

            aliens[i].shot_timer--;
            if(aliens[i].shot_timer == 0)
            {
                switch(aliens[i].type)
                {
                case ALIEN_TYPE_BUG:
                    if(!aliens[i+N_COLS].used)
                    {
                        shots_add(false, true, cx, cy);
                        aliens[i].shot_timer = 150;
                    }
                    break;
                case ALIEN_TYPE_ARROW:
                    shots_add(false, true, cx, aliens[i].y);
                    aliens[i].shot_timer = 80;
                    break;
                case ALIEN_TYPE_THICCBOI:
                    shots_add(false, true, cx-5, cy);
                    shots_add(false, true, cx+5, cy);
                    shots_add(false, true, cx-5, cy + 8);
                    shots_add(false, true, cx+5, cy + 8);
                    aliens[i].shot_timer = 200;
                    break;
                }
            }
        }
    }
}

void aliens_draw()
{
    for(int i = 0; i < ALIENS_N; i++)
    {
        if(!aliens[i].used)
            continue;
        if(aliens[i].blink > 2)
            continue;

        al_draw_bitmap(sprites.alien[aliens[i].type], aliens[i].x, aliens[i].y, 0);
    }
}

 ALIEN mother;

void alien_m_init()
{
    mother.x=BUFFER_W;
    mother.y=BUFFER_H/7;
    mother.type=ALIEN_TYPE_THICCBOI;
    mother.life=1;
    mother.used=true;
    
}

void alien_m_draw()
{
    if (mother.used)
        al_draw_bitmap(sprites.alien[mother.type],mother.x,mother.y,0);
}

void alien_m_update()
{
    mother.x-=3;
    if (shots_collide(false,mother.x,mother.y,ALIEN_THICCBOI_W,ALIEN_THICCBOI_H))
        mother.life--;
    if (mother.life==0)
        mother.used=false;
    if (mother.x<=-ALIEN_THICCBOI_W)
    {
        mother.used=true;
        mother.x=BUFFER_W;
    }
}

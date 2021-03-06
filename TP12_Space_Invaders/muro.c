/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */


#include "muro.h"
#include "otros.h"
#include <stdbool.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_image.h>
#include "shots.h"
#include "sprites.h"


MURO muro_arr[MURO_N];
ALLEGRO_BITMAP* muro_img[4];

void muro_init ()
{
    muro_arr[0].used=true;
    muro_arr[0].life=2;
    muro_arr[0].type=1;
    muro_arr[0].x=BUFFER_W/9;
    muro_arr[0].y=MURO_H;// numeri magico?
    int j;
    for(j=1;j<MURO_N;j++)
    {   
        muro_arr[j].type=1;
        muro_arr[j].used=true;
        muro_arr[j].life=2;       
        muro_arr[j].x=muro_arr[j-1].x;
        if (j%3==0&&j%12!=0)
        {
            muro_arr[j].x+=MURO_S;   
            muro_arr[j].y=muro_arr[j-3].y;
            muro_arr[j].type+=2;
            muro_arr[j-1].type+=2;
        }
        else if (j%12==0)
        {
            muro_arr[j].x+=BUFFER_H/6;   
            muro_arr[j].y=muro_arr[j-3].y;
        }
        else
            muro_arr[j].y=muro_arr[j-1].y-+MURO_S;      
    }
}

void muro_draw ()
{
    for (int i=0;i<MURO_N;i++)
    {
        if(muro_arr[i].used==true)
        {
        //if (muro_arr[i].type==1)
            al_draw_bitmap(muro_img[muro_arr[i].life-1], muro_arr[i].x, muro_arr[i].y, 0);
       // else
            //al_draw_bitmap(muro3, muro_arr[i].x, muro_arr[i].y, 0);
        }

    }
}

void muro_update ()
{
    for(int i=0;i<MURO_N;i++)
    {
        if((shots_collide(false,muro_arr[i].x,muro_arr[i].y,MURO_S,MURO_S))||(shots_collide(true,muro_arr[i].x,muro_arr[i].y,MURO_S,MURO_S)))
        {
            muro_arr[i].life--;
        }
        if(muro_arr[i].life==0)
        {
            muro_arr[i].used=false;
            
        }
    }        
}
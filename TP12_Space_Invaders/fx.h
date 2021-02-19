/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   fx.h
 * Author: mariano
 *
 * Created on February 2, 2021, 6:37 PM
 */

#include <stdbool.h>

#ifndef FX_H
#define FX_H

#define FX_N 55 //ALIEN_N
#define STARS_N ((BUFFER_W / 2) - 1)

typedef struct FX
{
    int x, y;
    int frame;
    bool spark;
    bool used;
} FX;

void audio_init();
void audio_deinit();
void fx_init();
void fx_add(bool spark, int x, int y);
void fx_update();
void fx_draw();
void stars_init();
void stars_update();
void stars_draw();
void hud_init();
void hud_deinit();
void hud_update();
void hud_draw();


#endif /* FX_H */


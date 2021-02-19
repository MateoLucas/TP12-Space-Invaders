/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   aliens.h
 * Author: mariano
 *
 * Created on February 2, 2021, 6:37 PM
 */

#include "otros.h"
#include "sprites.h"

#ifndef ALIENS_H
#define ALIENS_H

#define ALIENS_N 55
#define ALIEN_SPACE_BUFFER 15
#define REFERENCE_X (BUFFER_W/4)-(((ALIEN_BUG_W*5)+ALIEN_SPACE_BUFFER)/2)//5 por la cantidad de columnas
#define REFERENCE_Y (BUFFER_H/10)
#define N_COLS 11

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

void aliens_init();
void aliens_update();
void aliens_draw();
void alien_m_init();
void alien_m_draw();
void alien_m_update();


#endif /* ALIENS_H */


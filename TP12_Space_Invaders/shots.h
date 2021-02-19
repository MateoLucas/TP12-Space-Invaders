/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   shots.h
 * Author: mariano
 *
 * Created on February 2, 2021, 6:38 PM
 */

#ifndef SHOTS_H
#define SHOTS_H

#define SHOTS_N 128
typedef struct SHOT
{
    int x, y, dx, dy;
    int frame;
    bool ship;
    bool used;
} SHOT;

void shots_init();
bool shots_add(bool ship, bool straight, int x, int y);
void shots_update();
bool shots_collide(bool ship, int x, int y, int w, int h);
void shots_draw();


#endif /* SHOTS_H */


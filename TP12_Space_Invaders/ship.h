/*;
 *
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ship.h
 * Author: mariano
 *
 * Created on February 2, 2021, 6:37 PM
 */

#ifndef SHIP_HEADER
#define SHIP_HEADER

#define SHIP_SPEED 3
#define SHIP_MAX_X (BUFFER_W - SHIP_W)
#define SHIP_MAX_Y (BUFFER_H - SHIP_H)

typedef struct SHIP
{
    int x, y;
    int shot_timer;
    int lives;
    int respawn_timer;
    int invincible_timer;
} SHIP;

void ship_init();
void ship_update();
void ship_draw();

#endif /* SHIP_H */


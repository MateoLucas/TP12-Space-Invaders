/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   sprites.h
 * Author: mariano
 *
 * Created on February 2, 2021, 6:38 PM
 */

#ifndef SPRITES_H
#define SPRITES_H

#define SHIP_W 12
#define SHIP_H 13

#define SHIP_SHOT_W 2
#define SHIP_SHOT_H 9

#define LIFE_W 6
#define LIFE_H 6


#define ALIEN_BUG_W      14
#define ALIEN_BUG_H      9
#define ALIEN_ARROW_W    13
#define ALIEN_ARROW_H    10
#define ALIEN_THICCBOI_W 45
#define ALIEN_THICCBOI_H 27

#define ALIEN_SHOT_W 4
#define ALIEN_SHOT_H 4

#define EXPLOSION_FRAMES 4
#define SPARKS_FRAMES    3

void sprites_init();
void sprites_deinit();


#endif /* SPRITES_H */


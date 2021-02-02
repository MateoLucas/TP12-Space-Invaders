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

#ifndef ALIENS_H
#define ALIENS_H

#define ALIENS_N 55
#define ALIEN_SPACE_BUFFER 15
#define REFERENCE_X (BUFFER_W/4)-(((ALIEN_BUG_W*5)+ALIEN_SPACE_BUFFER)/2)//5 por la cantidad de columnas
#define REFERENCE_Y (BUFFER_H/10)
#define N_COLS 11

void aliens_init();
void aliens_update();
void aliens_draw();
void alien_m_init();
void alien_m_draw();
void alien_m_update();


#endif /* ALIENS_H */


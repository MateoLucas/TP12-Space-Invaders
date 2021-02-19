/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   muro.h
 * Author: mariano
 *
 * Created on February 2, 2021, 6:38 PM
 */

#ifndef MURO_
#define MURO_

#define MURO_S 8
#define MURO_H 195
#define MURO_N 48
#define STRUCT_N 4
#define M_POR_S 8

typedef struct MURO
{
    int x, y;
    int life;
    int type;
    bool used;
} MURO;

void muro_init ();
void muro_draw ();
void muro_update ();


#endif /* MURO_ */


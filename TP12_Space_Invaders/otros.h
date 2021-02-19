/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   otros.h
 * Author: mariano
 *
 * Created on February 2, 2021, 6:38 PM
 */

#include <stdbool.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_image.h>

#ifndef OTROS_H
#define OTROS_H

#define BUFFER_W 280
#define BUFFER_H 250

#define DISP_SCALE 3
#define DISP_W (BUFFER_W * DISP_SCALE)
#define DISP_H (BUFFER_H * DISP_SCALE)

#define KEY_SEEN     1
#define KEY_RELEASED 2

void must_init(bool test, const char *description);
int between(int lo, int hi);
float between_f(float lo, float hi);
bool collide(int ax1, int ay1, int ax2, int ay2, int bx1, int by1, int bx2, int by2);
void disp_init();
void disp_deinit();
void disp_pre_draw();
void disp_post_draw();
void keyboard_init();
void keyboard_update(ALLEGRO_EVENT* event);


#endif /* OTROS_H */


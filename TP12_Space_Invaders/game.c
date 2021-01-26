/*



 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.\sleep(1);
 


 * File:   main.c
 * Author: mateo
 *
 * Created on December 3, 2020, 1:22 PM
 


#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_primitives.h>




// uso srand(time(NULL)) para incializar
//rand() % 10 + 1;10 rango 1 valor inicial

    

#define KEY_SEEN     1
#define KEY_RELEASED 2
#define DISPLAYH (600)
#define DISPLAYW (2*DISPLAYH)
#define FPS (1.0 / 30.0)
#define HERO_LENGTH 76//es 75 pero lo hago par- ya no tiene utilidad
#define XWING_WIDTH 60
#define XWING_HEIGHT 53
#define EXPLOSION_FRAMES 19
#define FX_N 128
#define SPARKS_FRAMES 2
#define SPARK_WIDTH 50
#define SPARK_HEIGHT 50

void must_init(bool test, const char *description)
{
    if(test) return;
    printf("couldn't initialize %s\n", description);
    exit(1);
}
/*
typedef struct FX
{
    int x, y;
    int frame;
    bool spark;
    bool used;
} FX;


FX fx[FX_N];

void fx_init()
{
    for(int i = 0; i < FX_N; i++)
        fx[i].used = false;
}

void fx_add(bool spark, int x, int y)
{
    if(!spark)
        al_play_sample(sample_explode[between(0, 2)], 0.75, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);

    for(int i = 0; i < FX_N; i++)
    {
        if(fx[i].used)
            continue;

        fx[i].x = x;
        fx[i].y = y;
        fx[i].frame = 0;
        fx[i].spark = spark;
        fx[i].used = true;
        return;
    }
}

void fx_update()
{
    for(int i = 0; i < FX_N; i++)
    {
        if(!fx[i].used)
            continue;

        fx[i].frame++;

        if((!fx[i].spark && (fx[i].frame == (EXPLOSION_FRAMES * 2)))
        || ( fx[i].spark && (fx[i].frame == (SPARKS_FRAMES * 2)))
        )
            fx[i].used = false;
    }
}

void fx_draw()
{
    for(int i = 0; i < FX_N; i++)
    {
        if(!fx[i].used)
            continue;

        int frame_display = fx[i].frame / 2;
        ALLEGRO_BITMAP* bmp =
            fx[i].spark
            ? sprites.sparks[frame_display]
            : sprites.explosion[frame_display]
        ;

        int x = fx[i].x - (al_get_bitmap_width(bmp) / 2);
        int y = fx[i].y - (al_get_bitmap_height(bmp) / 2);
        al_draw_bitmap(bmp, x, y, 0);
    }
}


int main()
{

    must_init(al_init(), "allegro");
    must_init(al_install_keyboard(), "keyboard");

    ALLEGRO_TIMER* timer = al_create_timer(FPS);
    must_init(timer, "timer");

    ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
    must_init(queue, "queue");

    ALLEGRO_DISPLAY* disp = al_create_display(DISPLAYW, DISPLAYH);
    must_init(disp, "display");

    ALLEGRO_FONT* font = al_create_builtin_font();
    must_init(font, "font");

    must_init(al_init_image_addon(), "image addon");
    must_init(al_install_mouse(), "mouse");
    
    ALLEGRO_BITMAP* milenium_falcon = al_load_bitmap("x-wing.png");
    must_init(milenium_falcon, "milenium_falcon");
    
    ALLEGRO_BITMAP* espada_verde = al_load_bitmap("espada_verde.png");
    must_init(espada_verde, "espada_verde");
    al_hide_mouse_cursor(disp);
    
    ALLEGRO_BITMAP* background = al_load_bitmap("background2.png");
    must_init(background, "background");
    
    ALLEGRO_BITMAP* sheet = al_load_bitmap("spritesheet.png");
    must_init(sheet, "spritesheet");
    
    ALLEGRO_BITMAP* sparks_sprite = al_load_bitmap("sparks_sprite.png");
    must_init(sparks_sprite, "sparks_sprite.png");
    
    ALLEGRO_BITMAP* sprite_grab(ALLEGRO_BITMAP* sheet, int x, int y, int w, int h)
    {
        ALLEGRO_BITMAP* sprite = al_create_sub_bitmap(sheet, x, y, w, h);
        must_init(sprite, "sprite grab");
        return sprite;
    }
    ALLEGRO_BITMAP* sparks[SPARKS_FRAMES];
    sparks[0]= sprite_grab(sparks_sprite,160,90,SPARK_WIDTH,SPARK_HEIGHT);//COMPLETAR
    ALLEGRO_BITMAP* explosion[EXPLOSION_FRAMES];
    explosion[0] = sprite_grab(sheet,0, 0, XWING_WIDTH, XWING_HEIGHT);//la spritesheet esta escalada para que coincidan de tamaño con xwing
    explosion[1] = sprite_grab(sheet,XWING_WIDTH, 0, XWING_WIDTH, XWING_HEIGHT);
    explosion[2] = sprite_grab(sheet,2*XWING_WIDTH, 0, XWING_WIDTH, XWING_HEIGHT);
    explosion[3] = sprite_grab(sheet,3*XWING_WIDTH, 0, XWING_WIDTH, XWING_HEIGHT);
    
    explosion[4] = sprite_grab(sheet, 0, XWING_HEIGHT, XWING_WIDTH, XWING_HEIGHT);
    explosion[5] = sprite_grab(sheet,XWING_WIDTH, XWING_HEIGHT, XWING_WIDTH, XWING_HEIGHT);  
    explosion[6] = sprite_grab(sheet,2*XWING_WIDTH, XWING_HEIGHT, XWING_WIDTH, XWING_HEIGHT);
    explosion[7] = sprite_grab(sheet,3*XWING_WIDTH, XWING_HEIGHT, XWING_WIDTH, XWING_HEIGHT);
    
    explosion[9] = sprite_grab(sheet,0, 2*XWING_HEIGHT, XWING_WIDTH, XWING_HEIGHT);
    explosion[10] = sprite_grab(sheet,XWING_WIDTH, 2*XWING_HEIGHT, XWING_WIDTH, XWING_HEIGHT);
    explosion[11] = sprite_grab(sheet,2*XWING_WIDTH, 2*XWING_HEIGHT, XWING_WIDTH, XWING_HEIGHT);
    explosion[12] = sprite_grab(sheet,3*XWING_WIDTH, 2*XWING_HEIGHT, XWING_WIDTH, XWING_HEIGHT);
    
    explosion[13] = sprite_grab(sheet, 0, 3*XWING_HEIGHT, XWING_WIDTH, XWING_HEIGHT);
    explosion[13] = sprite_grab(sheet,XWING_WIDTH, 3*XWING_HEIGHT, XWING_WIDTH, XWING_HEIGHT);
    explosion[14] = sprite_grab(sheet,2*XWING_WIDTH, 3*XWING_HEIGHT, XWING_WIDTH, XWING_HEIGHT);
    explosion[15] = sprite_grab(sheet, 3*XWING_WIDTH, 3*XWING_HEIGHT, XWING_WIDTH, XWING_HEIGHT);
    
    explosion[16] = sprite_grab(sheet,XWING_WIDTH/2, 4*XWING_HEIGHT, XWING_WIDTH, XWING_HEIGHT);
    explosion[17] = sprite_grab(sheet,XWING_WIDTH + XWING_WIDTH/2, 4*XWING_HEIGHT, XWING_WIDTH, XWING_HEIGHT);
    explosion[18] = sprite_grab(sheet,2*XWING_WIDTH + XWING_WIDTH/2, 4*XWING_HEIGHT, XWING_WIDTH, XWING_HEIGHT);
    
    must_init(al_install_audio(), "audio");
    must_init(al_init_acodec_addon(), "audio codecs");
    must_init(al_reserve_samples(128), "reserve samples");
    must_init(al_init_primitives_addon(), "primitives");
    
    ALLEGRO_SAMPLE* Tslamp = al_load_sample("Tslamp.wav");
    must_init(Tslamp, "Tslamp.wav");
    /*
    ALLEGRO_SAMPLE* sample_shot;
    sample_shot = al_load_sample("shot.flac");
    must_init(sample_shot, "shot sample");
    ALLEGRO_SAMPLE* sample_explosion;
    sample_explosion = al_load_sample("explosion.flac");
    must_init(sample_explosion, "explosion sample");
    
   
    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(disp));
    al_register_event_source(queue, al_get_timer_event_source(timer));
    al_register_event_source(queue, al_get_mouse_event_source()); 
    
    bool done = false;
    bool redraw = true;
    ALLEGRO_EVENT event;

    al_start_timer(timer);

    unsigned char key[ALLEGRO_KEY_MAX];
    memset(key, 0, sizeof(key));
    
    float x, y, Xmouse, Ymouse;
    x = 75;
    y = DISPLAYH - ((DISPLAYH*10)/100) - (HERO_LENGTH/2);//margen de 10% mas media altura de la nave
    Xmouse = 10;
    Ymouse = (DISPLAYH/2);

    int MFspeed = 10;
    
    bool music_pause;
    
    
    
    while(1)
    {
       al_wait_for_event(queue, &event);

        switch(event.type)
        {
            case ALLEGRO_EVENT_TIMER:
                if(key[ALLEGRO_KEY_LEFT])
                    if(x>60)//NUMERO MAGICO SOLUCIONAR
                        x--;
                if(key[ALLEGRO_KEY_RIGHT])
                    if(x<114)//NUMERO MAGICO SOLUCIONAR
                        x++;

                if(key[ALLEGRO_KEY_ESCAPE])
                    done = true;

                for(int i = 0; i < ALLEGRO_KEY_MAX; i++)
                    key[i] &= KEY_SEEN;

                redraw = true;
                break;

            case ALLEGRO_EVENT_KEY_DOWN:
                key[event.keyboard.keycode] = KEY_SEEN | KEY_RELEASED;
                if(event.keyboard.keycode == ALLEGRO_KEY_P)
                    al_play_sample(Tslamp, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                if(event.keyboard.keycode != ALLEGRO_KEY_ESCAPE)
                    break;
                break;
            case ALLEGRO_EVENT_KEY_UP:
                key[event.keyboard.keycode] &= KEY_RELEASED;
                break;
            case ALLEGRO_EVENT_MOUSE_AXES:
                Xmouse = event.mouse.x;////NUMERO MAGICO SOLUCIONAR
                Ymouse = event.mouse.y;
                break;
            case ALLEGRO_EVENT_DISPLAY_CLOSE:
                done = true;
                break;
        }

        if(done)
            break;

        if(redraw && al_is_event_queue_empty(queue))
        {
            al_clear_to_color(al_map_rgb(0, 0, 0));
            al_draw_text(font, al_map_rgb(255, 255, 255), 640/2, 480/2, ALLEGRO_ALIGN_CENTER, "Tslamp.");
            al_draw_textf(font, al_map_rgb(255, 255, 255), 0, 0, 0, "X: %.1f Y: %.1f", x, y);
            al_draw_bitmap(milenium_falcon, x*MFspeed , y , 0);
            al_draw_bitmap(sparks[0],x*MFspeed,y,0);
            al_draw_bitmap(espada_verde, Xmouse, Ymouse,0);
            //al_draw_bitmap(explosion[17],x*MFspeed,y,0);
            al_draw_line(DISPLAYW/2+1, DISPLAYH-(DISPLAYH/100), DISPLAYW/2+1, DISPLAYH/100, al_map_rgb_f(1, 1, 0), 1);
            al_draw_line(DISPLAYW-1, DISPLAYH-(DISPLAYH/100), DISPLAYW-1, DISPLAYH/100, al_map_rgb_f(1, 1, 0), 1);
            al_draw_line(DISPLAYW/2+1, DISPLAYH/100, DISPLAYW-1, DISPLAYH/100, al_map_rgb_f(1, 1, 0), 1);
            al_draw_line(DISPLAYW/2+1, DISPLAYH-(DISPLAYH/100), DISPLAYW-1, DISPLAYH-(DISPLAYH/100), al_map_rgb_f(1, 1, 0), 1);
            al_flip_display();
            redraw = false;
        }
    }
al_destroy_bitmap(milenium_falcon);
al_destroy_bitmap(background);
al_destroy_bitmap(sheet);
al_destroy_bitmap(sparks_sprite);
al_destroy_sample(Tslamp);
//al_destroy_sample(sample_shot);
//al_destroy_sample(sample_explosion);

al_destroy_font(font);
al_destroy_display(disp);
al_destroy_timer(timer);
al_destroy_event_queue(queue);
}
*/

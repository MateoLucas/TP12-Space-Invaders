



#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_image.h>
#include "aliens.h"
#include "ship.h"
#include "fx.h"
#include "loadSave.h"
#include "menu.h"
#include "muro.h"
#include "otros.h"
#include "shots.h"
#include "sprites.h"




/*SAVE GAME 
 * 
 * 
 * guarda en el siguiente orden: Score ; 55 aliens(x,y,tipo,,shot timer, blink, life, used ),ship(x,y,shot_timer,lives,respawn_timer,invincible_timer)*/




// --- main ---

int main()
{
    must_init(al_init(), "allegro");
    must_init(al_install_keyboard(), "keyboard");

    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 60.0);
    must_init(timer, "timer");

    ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
    must_init(queue, "queue");

    disp_init();

    audio_init();

    must_init(al_init_image_addon(), "image");
    sprites_init();

    hud_init();

    must_init(al_init_primitives_addon(), "primitives");

    must_init(al_install_audio(), "audio");
    must_init(al_init_acodec_addon(), "audio codecs");
    must_init(al_reserve_samples(16), "reserve samples");

    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(disp));
    al_register_event_source(queue, al_get_timer_event_source(timer));

    keyboard_init();
    fx_init();
    shots_init();
    ship_init();
    aliens_init();
    stars_init();
    muro_init();
    alien_m_init();

    
    ALLEGRO_EVENT event;
    al_start_timer(timer);
     bool done = false;
    while(1)//menu
    {
        menu();
        while(event.type!=ALLEGRO_KEY_D)
        {
            new_game_select();
        }
        while(event.type!=ALLEGRO_KEY_D)
        {
            load_game_select();
        }
        if (false)
        {
            frames = 0;
            score = 0;
           
            bool redraw = true;
            bool pause = false;
            while(1)//juego
            {
                al_wait_for_event(queue, &event);

                switch(event.type)
                {


                    case ALLEGRO_EVENT_TIMER:
                        if(!pause)
                        {
                            fx_update();
                            shots_update();
                            stars_update();
                            ship_update();
                            aliens_update();
                            hud_update();
                            muro_update();
                            alien_m_update();
                        }

                        if(key[ALLEGRO_KEY_P])//corregir
                        {
                            pause = pause ? false : true;
                            al_rest(1);
                        }
                        if(key[ALLEGRO_KEY_S])
                        {
                            int save;
                            save = save_game();
                            if(!save==NULL)
                                printf("save error!");
                        }
                        if(key[ALLEGRO_KEY_L])//corregir
                        {
                            int load;
                            load = load_game();
                            if(load == NULL)
                                printf("load error!");
                        }
                        if(key[ALLEGRO_KEY_ESCAPE])
                            done = true;

                        redraw = true;
                        frames++;
                        break;

                    case ALLEGRO_EVENT_DISPLAY_CLOSE:
                        done = true;
                        break;
                }

                if(done)
                    break;

                keyboard_update(&event);

                if(redraw && al_is_event_queue_empty(queue))
                {
                    disp_pre_draw();
                    al_clear_to_color(al_map_rgb(0,0,0));

                    stars_draw();
                    aliens_draw();
                    shots_draw();
                    fx_draw();
                    ship_draw();
                    muro_draw();
                    alien_m_draw();

                    hud_draw();

                    disp_post_draw();
                    redraw = false;
                }

            }
        }
        if(done)
            break;
    }
    sprites_deinit();
    hud_deinit();
    audio_deinit();
    disp_deinit();
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);

    return 0;
}

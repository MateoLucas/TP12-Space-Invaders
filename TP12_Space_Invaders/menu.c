/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
void menu ()
{
         al_draw_text(
            font,
            al_map_rgb_f(255,0,0),
            BUFFER_W / 2, BUFFER_H / 5,
            ALLEGRO_ALIGN_CENTER,
            "S P A C E   I N V A D E R S"
        );
        al_draw_text(
            font,
            al_map_rgb_f(1,1,1),
            BUFFER_W / 2, 2*BUFFER_H / 5,
            ALLEGRO_ALIGN_CENTER,
            "NEW GAME"
        );
        al_draw_text(
            font,
            al_map_rgb_f(1,1,1),
            BUFFER_W / 2, 3*BUFFER_H / 5,
            ALLEGRO_ALIGN_CENTER,
            "LOAD GAME"
        );      
        al_draw_text(
            font,
            al_map_rgb_f(1,1,1),
            BUFFER_W / 2, 4*BUFFER_H / 5,
            ALLEGRO_ALIGN_CENTER,
            "QUIT"
        );
        al_flip_display();
        al_rest(0.5);
        
}

void new_game_select()
{
        al_draw_text(
            font,
            al_map_rgb_f(255,0,0),
            BUFFER_W / 2, 2*BUFFER_H / 5,
            ALLEGRO_ALIGN_CENTER,
            "NEW GAME"
        );
        al_flip_display();
        al_rest(0.5);
                al_draw_text(
            font,
            al_map_rgb_f(1,1,1),
            BUFFER_W / 2, 2*BUFFER_H / 5,
            ALLEGRO_ALIGN_CENTER,
            "NEW GAME"
        );
        al_flip_display();
        al_rest(0.5);
}

void load_game_select()
{
        al_draw_text(
            font,
            al_map_rgb_f(250,0,0),
            BUFFER_W / 2, 3*BUFFER_H / 5,
            ALLEGRO_ALIGN_CENTER,
            "LOAD GAME"
        );      
        al_flip_display();
        al_rest(0.5);
        al_draw_text(
            font,
            al_map_rgb_f(1,1,1),
            BUFFER_W / 2, 3*BUFFER_H / 5,
            ALLEGRO_ALIGN_CENTER,
            "LOAD GAME"
        );      
        al_flip_display();
        al_rest(0.5);
}


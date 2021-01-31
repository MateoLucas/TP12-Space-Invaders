/* this is a complete copy of the source from allegro vivace's 'gameplay' section.
 *
 * for gcc users, it can be compiled & run with:
 *
 * gcc game.c -o game $(pkg-config allegro-5 allegro_font-5 allegro_primitives-5 allegro_audio-5 allegro_acodec-5 allegro_image-5 --libs --cflags)
 * ./game
 */

#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_image.h>


// --- general ---

long frames;
long score;

void must_init(bool test, const char *description)
{
    if(test) return;

    printf("couldn't initialize %s\n", description);
    exit(1);
}

int between(int lo, int hi)
{
    return lo + (rand() % (hi - lo));
}

float between_f(float lo, float hi)
{
    return lo + ((float)rand() / (float)RAND_MAX) * (hi - lo);
}

bool collide(int ax1, int ay1, int ax2, int ay2, int bx1, int by1, int bx2, int by2)
{
    if(ax1 > bx2) return false;
    if(ax2 < bx1) return false;
    if(ay1 > by2) return false;
    if(ay2 < by1) return false;

    return true;
}


// --- display ---

#define BUFFER_W 280
#define BUFFER_H 220

#define DISP_SCALE 3
#define DISP_W (BUFFER_W * DISP_SCALE)
#define DISP_H (BUFFER_H * DISP_SCALE)

ALLEGRO_DISPLAY* disp;
ALLEGRO_BITMAP* buffer;

void disp_init()
{
    al_set_new_display_option(ALLEGRO_SAMPLE_BUFFERS, 1, ALLEGRO_SUGGEST);
    al_set_new_display_option(ALLEGRO_SAMPLES, 8, ALLEGRO_SUGGEST);

    disp = al_create_display(DISP_W, DISP_H);
    must_init(disp, "display");

    buffer = al_create_bitmap(BUFFER_W, BUFFER_H);
    must_init(buffer, "bitmap buffer");
}

void disp_deinit()
{
    al_destroy_bitmap(buffer);
    al_destroy_display(disp);
}

void disp_pre_draw()
{
    al_set_target_bitmap(buffer);
}

void disp_post_draw()
{
    al_set_target_backbuffer(disp);
    al_draw_scaled_bitmap(buffer, 0, 0, BUFFER_W, BUFFER_H, 0, 0, DISP_W, DISP_H, 0);

    al_flip_display();
}


// --- keyboard ---

#define KEY_SEEN     1
#define KEY_RELEASED 2
unsigned char key[ALLEGRO_KEY_MAX];

void keyboard_init()
{
    memset(key, 0, sizeof(key));
}

void keyboard_update(ALLEGRO_EVENT* event)
{
    switch(event->type)
    {
        case ALLEGRO_EVENT_TIMER:
            for(int i = 0; i < ALLEGRO_KEY_MAX; i++)
                key[i] &= KEY_SEEN;
            break;

        case ALLEGRO_EVENT_KEY_DOWN:
            key[event->keyboard.keycode] = KEY_SEEN | KEY_RELEASED;
            break;
        case ALLEGRO_EVENT_KEY_UP:
            key[event->keyboard.keycode] &= KEY_RELEASED;
            break;
    }
}


// --- sprites ---

#define SHIP_W 12
#define SHIP_H 13

#define SHIP_SHOT_W 2
#define SHIP_SHOT_H 9

#define LIFE_W 6
#define LIFE_H 6

const int ALIEN_W[] = {14, 13, 45};
const int ALIEN_H[] = { 9, 10, 27};

#define ALIEN_BUG_W      ALIEN_W[0]
#define ALIEN_BUG_H      ALIEN_H[0]
#define ALIEN_ARROW_W    ALIEN_W[1]
#define ALIEN_ARROW_H    ALIEN_H[1]
#define ALIEN_THICCBOI_W ALIEN_W[2]
#define ALIEN_THICCBOI_H ALIEN_H[2]

#define ALIEN_SHOT_W 4
#define ALIEN_SHOT_H 4

#define EXPLOSION_FRAMES 4
#define SPARKS_FRAMES    3


typedef struct SPRITES
{
    ALLEGRO_BITMAP* _sheet;

    ALLEGRO_BITMAP* ship;
    ALLEGRO_BITMAP* ship_shot[2];
    ALLEGRO_BITMAP* life;

    ALLEGRO_BITMAP* alien[3];
    ALLEGRO_BITMAP* alien_shot;

    ALLEGRO_BITMAP* explosion[EXPLOSION_FRAMES];
    ALLEGRO_BITMAP* sparks[SPARKS_FRAMES];

    ALLEGRO_BITMAP* powerup[4];
} SPRITES;
SPRITES sprites;

ALLEGRO_BITMAP* muro1;
ALLEGRO_BITMAP* muro3;

ALLEGRO_BITMAP* sprite_grab(int x, int y, int w, int h)
{
    ALLEGRO_BITMAP* sprite = al_create_sub_bitmap(sprites._sheet, x, y, w, h);
    must_init(sprite, "sprite grab");
    return sprite;
}

void sprites_init()
{
    sprites._sheet = al_load_bitmap("spritesheet.png");
    must_init(sprites._sheet, "spritesheet");

    muro1 = al_load_bitmap("Muro.png");
    must_init(muro1, "Muro.png");
    muro3 = al_load_bitmap("Muro_roto.png");
    must_init(muro3, "Muro_roto.png");
    
    sprites.ship = sprite_grab(0, 0, SHIP_W, SHIP_H);

    sprites.life = sprite_grab(0, 14, LIFE_W, LIFE_H);

    sprites.ship_shot[0] = sprite_grab(13, 0, SHIP_SHOT_W, SHIP_SHOT_H);
    sprites.ship_shot[1] = sprite_grab(16, 0, SHIP_SHOT_W, SHIP_SHOT_H);

    sprites.alien[0] = sprite_grab(19, 0, ALIEN_BUG_W, ALIEN_BUG_H);
    sprites.alien[1] = sprite_grab(19, 10, ALIEN_ARROW_W, ALIEN_ARROW_H);
    sprites.alien[2] = sprite_grab(0, 21, ALIEN_THICCBOI_W, ALIEN_THICCBOI_H);

    sprites.alien_shot = sprite_grab(13, 10, ALIEN_SHOT_W, ALIEN_SHOT_H);

    sprites.explosion[0] = sprite_grab(33, 10, 9, 9);
    sprites.explosion[1] = sprite_grab(43, 9, 11, 11);
    sprites.explosion[2] = sprite_grab(46, 21, 17, 18);
    sprites.explosion[3] = sprite_grab(46, 40, 17, 17);

    sprites.sparks[0] = sprite_grab(34, 0, 10, 8);
    sprites.sparks[1] = sprite_grab(45, 0, 7, 8);
    sprites.sparks[2] = sprite_grab(54, 0, 9, 8);

    sprites.powerup[0] = sprite_grab(0, 49, 9, 12);
    sprites.powerup[1] = sprite_grab(10, 49, 9, 12);
    sprites.powerup[2] = sprite_grab(20, 49, 9, 12);
    sprites.powerup[3] = sprite_grab(30, 49, 9, 12);
}

void sprites_deinit()
{
    al_destroy_bitmap(sprites.ship);

    al_destroy_bitmap(sprites.ship_shot[0]);
    al_destroy_bitmap(sprites.ship_shot[1]);

    al_destroy_bitmap(sprites.alien[0]);
    al_destroy_bitmap(sprites.alien[1]);
    al_destroy_bitmap(sprites.alien[2]);

    al_destroy_bitmap(sprites.sparks[0]);
    al_destroy_bitmap(sprites.sparks[1]);
    al_destroy_bitmap(sprites.sparks[2]);

    al_destroy_bitmap(sprites.explosion[0]);
    al_destroy_bitmap(sprites.explosion[1]);
    al_destroy_bitmap(sprites.explosion[2]);
    al_destroy_bitmap(sprites.explosion[3]);

    al_destroy_bitmap(sprites.powerup[0]);
    al_destroy_bitmap(sprites.powerup[1]);
    al_destroy_bitmap(sprites.powerup[2]);
    al_destroy_bitmap(sprites.powerup[3]);

    al_destroy_bitmap(sprites._sheet);
    
    al_destroy_bitmap(muro1);
    al_destroy_bitmap(muro3);
}

#define MURO_S 8
#define MURO_H 8
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

MURO muro_arr[MURO_N];

void muro_init ()
{
    int i;
    int j;
            muro_arr[0].used = true;
        muro_arr[0].life=2;
        muro_arr[0].x=BUFFER_H;
        muro_arr[0].y=BUFFER_W;
    for (j=1;j<3;j++)
    {   
        muro_arr[j].used = true;
        muro_arr[j].life=2;
        muro_arr[j].x=BUFFER_H/9+j*MURO_S;
        muro_arr[j].y=BUFFER_W;
         
    }
    /*for(int j=M_POR_S;j<=32;j=j+M_POR_S)
    {
        for (i=j-MURO_N/4;i<j-MURO_N/8;i++)
        {
            muro_arr[i].used = true;
            muro_arr[i].life=2;
            if(i%8==0)
            {
                muro_arr[i].x=BUFFER_W/9+((j/4)-2)*BUFFER_W/9;
                muro_arr[i].y=BUFFER_H-30;    
            }
            else
            {
                if ((i%2)==0)
                {
                    muro_arr[i].x=muro_arr[i-1].x+MURO_H;
                    muro_arr[i].y=muro_arr[i-1].y;
                }
                else
                {
                    muro_arr[i].y=muro_arr[i-1].y-MURO_H;
                    muro_arr[i].x=muro_arr[i-1].x;
                }
            }
        }
        for (i;i<j;i++)
        {
            muro_arr[i].used = true;
            muro_arr[i].life=2;
            if(i>=j-2&&i<j)
            {    
                muro_arr[i].y=muro_arr[i-MURO_N/8].y;
                muro_arr[i].x=muro_arr[i-MURO_N/8].x+MURO_H;            
            }
            else
            {
                muro_arr[i].y=muro_arr[i-MURO_N/8].y;
                muro_arr[i].x=muro_arr[i-MURO_N/8].x+3*MURO_H;
            }
        }    
    }    
    */
    
}

void muro_draw ()
{
    //for (int i=0;i<MURO_N;i++)
   // {
        
            al_draw_bitmap(muro1, muro_arr[0].x, muro_arr[0].y, 0);
        //else
            //al_draw_bitmap(muro3, muro_arr[i].x, muro_arr[i].y, 0);
    //}
}


// --- audio ---

ALLEGRO_SAMPLE* sample_shot;
ALLEGRO_SAMPLE* sample_explode[2];

void audio_init()
{
    al_install_audio();
    al_init_acodec_addon();
    al_reserve_samples(128);

    sample_shot = al_load_sample("shot.flac");
    must_init(sample_shot, "shot sample");

    sample_explode[0] = al_load_sample("explode1.flac");
    must_init(sample_explode[0], "explode[0] sample");
    sample_explode[1] = al_load_sample("explode2.flac");
    must_init(sample_explode[1], "explode[1] sample");
}

void audio_deinit()
{
    al_destroy_sample(sample_shot);
    al_destroy_sample(sample_explode[0]);
    al_destroy_sample(sample_explode[1]);
}


// --- fx ---

typedef struct FX
{
    int x, y;
    int frame;
    bool spark;
    bool used;
} FX;

#define FX_N 55 //ALIEN_N
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


// --- shots ---

typedef struct SHOT
{
    int x, y, dx, dy;
    int frame;
    bool ship;
    bool used;
} SHOT;

#define SHOTS_N 128
SHOT shots[SHOTS_N];
bool ship_shot=false;
void shots_init()
{
    for(int i = 0; i < SHOTS_N; i++)
        shots[i].used = false;
}

bool shots_add(bool ship, bool straight, int x, int y)
{
    al_play_sample(
        sample_shot,
        0.3,
        0,
        ship ? 1.0 : between_f(1.5, 1.6),
        ALLEGRO_PLAYMODE_ONCE,
        NULL
    );

    for(int i = 0; i < SHOTS_N; i++)
    {
        if(shots[i].used)
            continue;

        shots[i].ship = ship;

        if(ship && !ship_shot)
        {
            shots[i].x = x - (SHIP_SHOT_W / 2);
            shots[i].y = y;
        }
        else // alien
        {
            int j;
           
            shots[i].x = x - (ALIEN_SHOT_W / 2);
            shots[i].y = y - (ALIEN_SHOT_H / 2);
            shots[i].dx = 0;
            shots[i].dy = 2;


            // if the shot has no speed, don't bother
            if(!shots[i].dx && !shots[i].dy)
                return true;

            shots[i].frame = 0;
        }

        shots[i].frame = 0;
        shots[i].used = true;

        return true;
    }
    return false;
}

void shots_update()
{
    for(int i = 0; i < SHOTS_N; i++)
    {
        if(!shots[i].used)
            continue;

        if(shots[i].ship)
        {
            shots[i].y -= 5;

            if(shots[i].y < -SHIP_SHOT_H)
            {
                shots[i].used = false;
                continue;
            }
        }
        else // alien
        {
            shots[i].x += shots[i].dx;
            shots[i].y += shots[i].dy;

            if((shots[i].x < -ALIEN_SHOT_W)
            || (shots[i].x > BUFFER_W)
            || (shots[i].y < -ALIEN_SHOT_H)
            || (shots[i].y > BUFFER_H)
            ) {
                shots[i].used = false;
                continue;
            }
        }

        shots[i].frame++;
    }
}

bool shots_collide(bool ship, int x, int y, int w, int h)
{
    for(int i = 0; i < SHOTS_N; i++)
    {
        if(!shots[i].used)
            continue;

        // don't collide with one's own shots
        if(shots[i].ship == ship)
            continue;

        int sw, sh;
        if(ship)
        {
            sw = ALIEN_SHOT_W;
            sh = ALIEN_SHOT_H;
        }
        else
        {
            sw = SHIP_SHOT_W;
            sh = SHIP_SHOT_H;
        }

        if(collide(x, y, x+w, y+h, shots[i].x, shots[i].y, shots[i].x+sw, shots[i].y+sh))
        {
            fx_add(true, shots[i].x + (sw / 2), shots[i].y + (sh / 2));
            shots[i].used = false;
            return true;
        }
    }

    return false;
}

void shots_draw()
{
    for(int i = 0; i < SHOTS_N; i++)
    {
        if(!shots[i].used)
            continue;

        int frame_display = (shots[i].frame / 2) % 2;

        if(shots[i].ship)
            al_draw_bitmap(sprites.ship_shot[frame_display], shots[i].x, shots[i].y, 0);
        else // alien
        {
            ALLEGRO_COLOR tint =
                frame_display
                ? al_map_rgb_f(1, 1, 1)
                : al_map_rgb_f(0.5, 0.5, 0.5)
            ;
            al_draw_tinted_bitmap(sprites.alien_shot, tint, shots[i].x, shots[i].y, 0);
        }
    }
}


// --- ship ---

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
SHIP ship;

void ship_init()
{
    ship.x = (BUFFER_W / 2) - (SHIP_W / 2);
    ship.y = BUFFER_H - SHIP_H;
    ship.shot_timer = 0;
    ship.lives = 3;
    ship.respawn_timer = 0;
    ship.invincible_timer = 120;
}

void ship_update()
{
    if(ship.lives < 0)
        return;

    if(ship.respawn_timer)
    {
        ship.respawn_timer--;
        return;
    }

    if(key[ALLEGRO_KEY_LEFT])
        ship.x -= SHIP_SPEED;
    if(key[ALLEGRO_KEY_RIGHT])
        ship.x += SHIP_SPEED;
    //if(key[ALLEGRO_KEY_UP])
        ship.y -= SHIP_SPEED;
    //if(key[ALLEGRO_KEY_DOWN])
        ship.y += SHIP_SPEED;

    if(ship.x < 0)
        ship.x = 0;
    if(ship.y < 0)
        ship.y = 0;

    if(ship.x > SHIP_MAX_X)
        ship.x = SHIP_MAX_X;
    if(ship.y > SHIP_MAX_Y)
        ship.y = SHIP_MAX_Y;

    if(ship.invincible_timer)
        ship.invincible_timer--;
    else
    {
        if(shots_collide(true, ship.x, ship.y, SHIP_W, SHIP_H))
        {
            int x = ship.x + (SHIP_W / 2);
            int y = ship.y + (SHIP_H / 2);
            fx_add(false, x, y);
            fx_add(false, x+4, y+2);
            fx_add(false, x-2, y-4);
            fx_add(false, x+1, y-5);

            ship.lives--;
            ship.respawn_timer = 90;
            ship.invincible_timer = 180;
        }
    }

    if(ship.shot_timer)
        ship.shot_timer--;
    else if(key[ALLEGRO_KEY_X])
    {
        int x = ship.x + (SHIP_W / 2);
        if(shots_add(true, false, x, ship.y))
            ship.shot_timer = 5;
    }
}

void ship_draw()
{
    if(ship.lives < 0)
        return;
    if(ship.respawn_timer)
        return;
    if(((ship.invincible_timer / 2) % 3) == 1)
        return;

    al_draw_bitmap(sprites.ship, ship.x, ship.y, 0);
}


// --- aliens ---

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

#define ALIENS_N 55

#define ALIEN_SPACE_BUFFER 15
ALIEN aliens[ALIENS_N];
#define REFERENCE_X (BUFFER_W/4)-(((ALIEN_BUG_W*5)+ALIEN_SPACE_BUFFER)/2)//5 por la cantidad de columnas
#define REFERENCE_Y (BUFFER_H/10)
//aliens[i].used = false;
#define N_COLS 11
void aliens_init()
{

    int t;
    int columnas = 0;
    for(t=0; t < ALIENS_N; t++)
    {
        int s = t/11;
        columnas++;
        aliens[t].used = true;
        aliens[t].type = ALIEN_TYPE_BUG;
        aliens[t].shot_timer = between(1, 99);
        aliens[t].blink = 0;
        aliens[t].life = 4;
        if(columnas >= N_COLS)
        {
            columnas=0;
        }
        aliens[t].x = REFERENCE_X + columnas*ALIEN_SPACE_BUFFER;
        aliens[t].y = REFERENCE_Y+ s* ALIEN_SPACE_BUFFER;
    }
}
bool aliens_last(int alien_n)
{
    int i;
    bool answer=false;
    int dead=0;
    int count=0;
    if (alien_n>(ALIENS_N-N_COLS))
    {
        answer=true;
    }

    else if (!aliens[alien_n+N_COLS].used)
    {
        answer=true;
        //for(i=alien_n+N_COLS ; i<=ALIENS_N ; i+=N_COLS)
        //{
          //  count++;
            //if(!aliens[i].used)
              //  dead++;
        //}
        //if(dead == count)
          //  answer = true;
   }
}
void aliens_update()
{   
    int j;
    bool detect_border=false;
    bool static direction = false;//true = derecha, flase = izquierda
    //int static alien_speed_factor = 2*frames/1000;
    
    for(j=0;j<ALIENS_N;j++)//while
    {
        if((aliens[j].used)&&((aliens[j].x >= BUFFER_W)||aliens[j].x<= 0))
        {
            detect_border=true;
        }
    }
    int i;
    if(detect_border)
    {
        if(direction)
        {
            direction = false;
            
        }else
        {
            direction = true;   
        }
    }
    
    for(i=0;i<ALIENS_N;i++)
    {
        if(aliens[i].used)
        {
            if(detect_border)
            {
                aliens[i].y += ALIEN_BUG_H/2;        
            }
        
            if(direction)
            {
                aliens[i].x += 1;
            }else
            {
                aliens[i].x -= 1;
            }
        

            if(aliens[i].blink)
                aliens[i].blink--;

            if(shots_collide(false, aliens[i].x, aliens[i].y, ALIEN_W[aliens[i].type], ALIEN_H[aliens[i].type]))
            {
                ship_shot = false;
                aliens[i].life--;
                aliens[i].blink = 4;
            }
        
            int cx = aliens[i].x + (ALIEN_W[aliens[i].type] / 2);
            int cy = aliens[i].y + (ALIEN_H[aliens[i].type] / 2);
            if(aliens[i].life <= 0)
            {
                
                fx_add(false, cx, cy);

                switch(aliens[i].type)
                {
                case ALIEN_TYPE_BUG:
                    score += 200;
                    break;

                case ALIEN_TYPE_ARROW:
                    score += 150;
                    break;

                case ALIEN_TYPE_THICCBOI:
                    score += 800;
                    fx_add(false, cx-10, cy-4);
                    fx_add(false, cx+4, cy+10);
                    fx_add(false, cx+8, cy+8);
                    break;
                }

                aliens[i].used = false;
                continue;
            }   

            aliens[i].shot_timer--;
            if(aliens[i].shot_timer == 0)
            {
                switch(aliens[i].type)
                {
                case ALIEN_TYPE_BUG:
                    if(aliens_last(i))
                    {
                        shots_add(false, true, cx, cy);
                        aliens[i].shot_timer = 150;
                    }
                    break;
                case ALIEN_TYPE_ARROW:
                    shots_add(false, true, cx, aliens[i].y);
                    aliens[i].shot_timer = 80;
                    break;
                case ALIEN_TYPE_THICCBOI:
                    shots_add(false, true, cx-5, cy);
                    shots_add(false, true, cx+5, cy);
                    shots_add(false, true, cx-5, cy + 8);
                    shots_add(false, true, cx+5, cy + 8);
                    aliens[i].shot_timer = 200;
                    break;
                }
            }
        }
    }
}

void aliens_draw()
{
    for(int i = 0; i < ALIENS_N; i++)
    {
        if(!aliens[i].used)
            continue;
        if(aliens[i].blink > 2)
            continue;

        al_draw_bitmap(sprites.alien[aliens[i].type], aliens[i].x, aliens[i].y, 0);
    }
}


// --- stars ---

typedef struct STAR
{
    float y;
    float speed;
} STAR;

#define STARS_N ((BUFFER_W / 2) - 1)
STAR stars[STARS_N];

void stars_init()
{
    for(int i = 0; i < STARS_N; i++)
    {
        stars[i].y = between_f(0, BUFFER_H);
        stars[i].speed = between_f(0.1, 1);
    }
}

void stars_update()
{
    for(int i = 0; i < STARS_N; i++)
    {
        stars[i].y += stars[i].speed;
        if(stars[i].y >= BUFFER_H)
        {
            stars[i].y = 0;
            stars[i].speed = between_f(0.1, 1);
        }
    }
}

void stars_draw()
{
    float star_x = 1.5;
    for(int i = 0; i < STARS_N; i++)
    {
        float l = stars[i].speed * 0.8;
        al_draw_pixel(star_x, stars[i].y, al_map_rgb_f(l,l,l));
        star_x += 2;
    }
}


// --- hud ---

ALLEGRO_FONT* font;
long score_display;

void hud_init()
{
    font = al_create_builtin_font();
    must_init(font, "font");

    score_display = 0;
}

void hud_deinit()
{
    al_destroy_font(font);
}

void hud_update()
{
    if(frames % 2)
        return;

    for(long i = 5; i > 0; i--)
    {
        long diff = 1 << i;
        if(score_display <= (score - diff))
            score_display += diff;
    }
}

void hud_draw()
{
    al_draw_textf(
        font,
        al_map_rgb_f(1,1,1),
        1, 1,
        0,
        "%06ld",
        score_display
    );

    int spacing = LIFE_W + 1;
    for(int i = 0; i < ship.lives; i++)
        al_draw_bitmap(sprites.life, 1 + (i * spacing), 10, 0);

    if(ship.lives < 0)
        al_draw_text(
            font,
            al_map_rgb_f(1,1,1),
            BUFFER_W / 2, BUFFER_H / 2,
            ALLEGRO_ALIGN_CENTER,
            "G A M E  O V E R"
        );
}
#define ESTRUCTURA '#'
#define INTRA_ESTRUC '|'
#define END_LOAD '$'
/*SAVE GAME 
 * 
 * 
 * guarda en el siguiente orden: Score ; 55 aliens(x,y,tipo,,shot timer, blink, life, used ),ship(x,y,shot_timer,lives,respawn_timer,invincible_timer)*/
int save_game()
{
    FILE *pSave = fopen("saved_game.txt", "w");
    
    //score  save
    fputc(ESTRUCTURA,pSave);//importante que empiece con ESTRUCTURA
    fprintf(pSave,"%ld",score);
    fputc(ESTRUCTURA,pSave);
    
    //alien save
    fputc(ESTRUCTURA, pSave);
    int i;
    int bug_type;
    for(i=0; i <= ALIENS_N-1 ;i++)
    {
        fprintf(pSave,"%d",aliens[i].x);
        fputc(INTRA_ESTRUC,pSave);
        fprintf(pSave,"%d",aliens[i].y);
        fputc(INTRA_ESTRUC,pSave);
        switch (aliens[i].type)
        {
            case ALIEN_TYPE_BUG:
                fprintf(pSave,"%c",'b');
                break;
               
            case ALIEN_TYPE_THICCBOI:
               fprintf(pSave,"%c",'t');
                break;
        }
        
        fputc(INTRA_ESTRUC,pSave);
        fprintf(pSave,"%d",aliens[i].shot_timer);
        fputc(INTRA_ESTRUC,pSave);
        fprintf(pSave,"%d",aliens[i].blink);
        fputc(INTRA_ESTRUC,pSave);
        fprintf(pSave,"%d",aliens[i].life);
        fputc(INTRA_ESTRUC,pSave);
        fprintf(pSave,"%c", aliens[i].used ? 't' : 'f');
        fputc(ESTRUCTURA,pSave);
    }
    
    //ship save
    fputc(ESTRUCTURA,pSave);
    fprintf(pSave,"%d",ship.x);
    fputc(INTRA_ESTRUC,pSave);
    fprintf(pSave,"%d",ship.y);
    fputc(INTRA_ESTRUC,pSave); 
    fprintf(pSave,"%d",ship.shot_timer);
    fputc(INTRA_ESTRUC,pSave);
    fprintf(pSave,"%d",ship.lives);
    fputc(INTRA_ESTRUC,pSave);
    fprintf(pSave,"%d",ship.respawn_timer);
    fputc(INTRA_ESTRUC,pSave);
    fprintf(pSave,"%d",ship.invincible_timer);
    fputc(INTRA_ESTRUC,pSave);
    
    fputc(END_LOAD,pSave);
    fflush(pSave);
    int check= fclose(pSave);
    return check;
}

int load_game()
{
    FILE *pLoad = fopen("saved_game.txt","r");
    char load = fgetc(pLoad);
    if((load != NULL)&&(load == ESTRUCTURA))
    {
        //score
        fscanf(pLoad,"%ld",&score);
        load = fgetc(pLoad);//ESTRUCTURA
        //aliens
        int i;
        for(i=0;i<= ALIENS_N-1;i++)
        {
            fscanf(pLoad, "%ld",&aliens[i].x);//x
            load = fgetc(pLoad);// INTRA_STRUCT
            fscanf(pLoad, "%ld",&aliens[i].y);//y
            load = fgetc(pLoad);// INTRA_STRUCT
            load = fgetc(pLoad);
            switch (load)
            {
                    case 'b':
                        aliens[i].type = ALIEN_TYPE_BUG;
                        break;
                    case 't':
                        aliens[i].type = ALIEN_TYPE_THICCBOI;
                        break;
            }
            load = fgetc(pLoad);// INTRA_STRUCT
            fscanf(pLoad, "%ld",&aliens[i].shot_timer);//shot_timer
            load = fgetc(pLoad);// INTRA_STRUCT
            fscanf(pLoad, "%ld",&aliens[i].blink);//blink
            load = fgetc(pLoad);// INTRA_STRUCT
            fscanf(pLoad, "%ld",&aliens[i].life);//lives
            load = fgetc(pLoad);// INTRA_STRUCT
            aliens[i].used = ((fgetc(pLoad)=='t') ? true : false);//used  
            load = fgetc(pLoad);//ESTRUCTURA
        }
        //ship
        load = fgetc(pLoad);//ESTRUCTURA
        fscanf(pLoad, "%ld",&ship.x);//x
        load = fgetc(pLoad);// INTRA_STRUCT
        fscanf(pLoad, "%ld",&ship.y);//y
        load = fgetc(pLoad);// INTRA_STRUCT
        fscanf(pLoad, "%ld",&ship.shot_timer);//shot_timer
        load = fgetc(pLoad);// INTRA_STRUCT
        fscanf(pLoad, "%ld",&ship.lives);//lives
        load = fgetc(pLoad);// INTRA_STRUCT
        fscanf(pLoad, "%ld",&ship.respawn_timer);//respawn
        load = fgetc(pLoad);// INTRA_STRUCT
        fscanf(pLoad, "%ld",&ship.invincible_timer);//lives
        load = fgetc(pLoad);// INTRA_STRUCT
        
        
        
    }
    int check = fclose(pLoad);
    return check;
}


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

    frames = 0;
    score = 0;

    bool done = false;
    bool redraw = true;
    ALLEGRO_EVENT event;

    al_start_timer(timer);
    
    int save;
    save = save_game();
    if(save==NULL)
        printf("save error!");
    
    int load;
    load = load_game();
    if(load == NULL)
        printf("load error!");
    
    while(1)
    {
        al_wait_for_event(queue, &event);

        switch(event.type)
        {
            case ALLEGRO_EVENT_TIMER:
                fx_update();
                shots_update();
                stars_update();
                ship_update();
                aliens_update();
                hud_update();

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

            hud_draw();

            disp_post_draw();
            redraw = false;
        }
    }

    sprites_deinit();
    hud_deinit();
    audio_deinit();
    disp_deinit();
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);

    return 0;
}

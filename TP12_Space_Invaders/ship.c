/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

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
        if(!ships_one_shot)//el unico tiro posible se uso
        {
            int x = ship.x + (SHIP_W / 2);
            if(shots_add(true, false, x, ship.y))
            ship.shot_timer = 5;
            ships_one_shot = true;
        }
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
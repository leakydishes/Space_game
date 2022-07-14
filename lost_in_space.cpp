#include "splashkit.h"
#include "lost_in_space.h"
#include "player.h"
#include "power_up.h"
#include "virus.h"

// global constants for game size
#define MIN_SIZE -1500
#define MAX_SIZE 1500
#define MAX_GAME_SIZE 3000
#define SCREEN_WIDTH 1200
#define SCREEN_HEIGHT 800
#define TIME 60000 // 1000 ms x 60 sec = 1 min

game_data new_game() ////declares game, menu, player
{
    game_data result;
    result.menu = true;
    result.player = new_player();
    result.game_timer = create_timer("MY_TIMER");
    start_timer("MY_TIMER");
    result.timer_pct = 0.95;
    return result;
}

void load_resources()
{
    load_resource_bundle("game_bundle", "lost_in_space.txt"); // load game resources

    // load sound resources
    load_music("lofi", "lofi.mp3");
    load_sound_effect("game_over_sound", "game_over_sound.wav");
    load_sound_effect("infection_sound", "infection_sound.wav");
    load_sound_effect("power_up_sound", "power_up_sound.wav");
    load_sound_effect("bonus_sound", "bonus_sound.wav");
}

void menu_screen(const game_data &game) // start menu
{
    clear_screen(COLOR_BLACK); // clear screen for redrawing
    draw_text("OBJECTIVES", COLOR_WHITE, 60, 100, option_to_screen());
    draw_text("1. USE ARROWS TO MOVE & SPEED", COLOR_WHITE, 100, 150, option_to_screen());
    draw_text("2. YOU HAVE 60 SEC - COLLECT HUMAN THINGS FOR POINTS", COLOR_WHITE, 100, 200, option_to_screen());
    draw_text("3. COLLECT CHEESE FOR FUEL, SYRINGES FOR ENERGY", COLOR_WHITE, 100, 250, option_to_screen());
    draw_text("4. 10 x HUMAN THINGS = 1 LEVEL + 60 SEC", COLOR_WHITE, 100, 300, option_to_screen());
    draw_text("PRESS ENTER TO START", COLOR_WHITE, 950, 690, option_to_screen());

    draw_text("HIT 1 HARRY", COLOR_WHITE, 380, 540, option_to_screen());
    draw_text("HIT 2 FRANCIS", COLOR_WHITE, 550, 540, option_to_screen());
    draw_text("HIT 3 CHARLIE", COLOR_WHITE, 715, 540, option_to_screen());
    draw_text("IN GAME", COLOR_WHITE, 565, 590, option_to_screen());
    draw_bitmap("ufo1", 360, 375, option_part_bmp(0, 0, bitmap_width("ufo1"), bitmap_height("ufo1"), option_to_screen()));
    draw_bitmap("ufo2", 530, 380, option_part_bmp(0, 0, bitmap_width("ufo1"), bitmap_height("ufo1"), option_to_screen()));
    draw_bitmap("ufo3", 695, 385, option_part_bmp(0, 0, bitmap_width("ufo3"), bitmap_height("ufo3"), option_to_screen()));
    draw_rectangle(COLOR_WHITE, 20, 20, 1160, 760, option_to_screen());
    refresh_screen(60);
}

bool menu_select(game_data &game) // handle user input menu
{
    if (key_down(RETURN_KEY)) // Enter key starts a new game
    {
        game = new_game();
        return false;
    }
    else
        return true;
}

void game_over(game_data &game) // game over screen
{

    if (game.player.health_pct <= 0 || game.player.cheese_pct <= 0 || game.timer_pct <= 0)
    {
        pause_timer("MY_TIMER");
        clear_screen(COLOR_BLACK);
        draw_text("GAME OVER", COLOR_WHITE, 600, 250, option_to_screen());
        draw_text("THIS IS A SAD DAY...", COLOR_WHITE, 600, 300, option_to_screen());
        draw_text("SCORE: " + to_string(game.player.score), COLOR_WHITE, 600, 350, option_to_screen());
        draw_text("INFECTIONS: " + to_string(game.player.infection), COLOR_WHITE, 600, 400, option_to_screen());
        draw_bitmap("duck", 430, 380, option_part_bmp(0, 0, bitmap_width("duck"), bitmap_height("duck"), option_to_screen()));
        draw_text("PRESS X TO EXIT", COLOR_WHITE, 600, 450, option_to_screen());
        draw_text("PRESS R TO RESTART", COLOR_WHITE, 600, 500, option_to_screen());
        sprite_set_x(game.player.player_sprite, 361); // horizontal
        sprite_set_y(game.player.player_sprite, 377); // vertical
        refresh_screen(60);

        if (key_down(R_KEY))
        {
            game.menu = true;
            if (game.menu) // menu
            {
                menu_screen(game);
                game.menu = menu_select(game); // Enter starts game in menu_select
            }
        }
    }
}

void play_game(const game_data &game) // draws the game by const reference //void gamemode (current_gamemode);
{
    clear_screen(COLOR_BLACK); // clear screen for redrawing

    draw_hud(game); // HUD

    draw_player(game.player); // player

    for (int i = 0; i < game.power_ups.size(); i++) // power ups random positions
    {
        draw_power_up(game.power_ups[i]);
    }

    for (int i = 0; i < game.virus.size(); i++) // virus random positions
    {
        draw_virus(game.virus[i]);
    }
}

void check_power_up_position(game_data &game) // power up deleted if outside global constants
{
    for (int i = 0; i < game.power_ups.size(); i++) // Checks power up position and adjusts to screen size with HUD 1120 x, 650 y
    {
        double power_up_y = sprite_y(game.power_ups[i].power_up_sprite); // power_up size y
        double power_up_x = sprite_x(game.power_ups[i].power_up_sprite); // power_up size x

        if ((power_up_y > MAX_SIZE) || (power_up_y < MIN_SIZE))
        {
            remove_power_up(game, i);
        }
        if ((power_up_x > MAX_SIZE) || (power_up_x < MIN_SIZE))
        {
            remove_power_up(game, i);
        }
    }
}

void check_virus_position(game_data &game) // Checks virus position and adjusts to screen size
{
    for (int i = 0; i < game.virus.size(); i++)
    {
        double virus_y = sprite_y(game.virus[i].virus_sprite); // virus size y
        double virus_x = sprite_x(game.virus[i].virus_sprite); // virus size x

        if ((virus_y > MAX_SIZE) || (virus_y < MIN_SIZE)) // virus deleted if outside global constants
        {
            remove_virus(game, i);
        }
        if ((virus_x > MAX_SIZE) || (virus_x < MIN_SIZE))
        {
            remove_virus(game, i);
        }
    }
}

void update_game(game_data &game) // updates the game by reference
{
    check_collisions(game); // check collisions

    check_level(game); // check level and increase time

    update_player(game.player); // update player

    if (game.timer_pct > 0) // decrease timer as time increases
    {
        game.timer_pct -= 0.000278;
    }

    // 3% add power up
    if (0.03 > rnd())
    {
        add_power_up(game);
    }

    // 2% add virus at less rate then power ups
    if (0.02 > rnd())
    {
        add_virus(game);
    }

    // add power up --> connects to new power up
    for (int i = 0; i < game.power_ups.size(); i++) //  loop through and draw all power ups.
    {
        check_power_up_position(game);
        update_power_up(game.power_ups[i]);
    }

    // add virus --> connects to new virus
    for (int i = 0; i < game.virus.size(); i++) //  loop through and draw all virus
    {
        check_virus_position(game);
        update_virus(game.virus[i]);
    }
}

void add_power_up(game_data &game) // power ups to game
{
    int random = rnd(MIN_SIZE, MAX_SIZE);
    game.power_ups.push_back(new_power_up(random, random)); // places power up in random space
}

void add_virus(game_data &game) // virus to game
{
    int random = rnd(MIN_SIZE, MAX_SIZE);
    game.virus.push_back(new_virus(random, random)); // places virus in random space
}

void check_level(game_data &game)
{
    if (game.player.score == 10) // power_ups = 10
    {
        game.player.level = 2.00; // 2 level

        if (game.timer_pct <= 1.00)
        {
            game.timer_pct += 0.10; // 10 sec add timer
            reset_timer("MY_TIMER");
        }
    }
    else if (game.player.score == 20) // power_ups = 20
    {
        game.player.level = 3; // 3 level
        if (game.timer_pct <= 1.00)
        {
            game.timer_pct += 0.10; // 10 sec add timer
            reset_timer("MY_TIMER");
        }
    }
    else if (game.player.score == 30) // power_ups = 30
    {
        game.player.level = 4; // 4 level
        if (game.timer_pct <= 1.00)
        {
            game.timer_pct += 0.10; // 10 sec add timer
            reset_timer("MY_TIMER");
        }
    }
    else if (game.player.score == 40) // power_ups = 40
    {
        game.player.level = 5; // 5 level
        if (game.timer_pct <= 1.00)
        {
            game.timer_pct += 0.10; // 10 sec add timer
            reset_timer("MY_TIMER");
        }
    }
    else if (game.player.score == 50) // power_ups = 50
    {
        game.player.level = 6; // 6 level
        if (game.timer_pct <= 1.00)
        {
            game.timer_pct += 0.10; // 10 sec add timer
            reset_timer("MY_TIMER");
        }
    }
}

void check_collisions(game_data &game) // When checked will remove power ups and virus and influence of player
{
    // checks collisions in game impacts player score
    for (int i = game.power_ups.size() - 1; i >= 0; i--)
    {
        if (sprite_collision(game.player.player_sprite, game.power_ups[i].power_up_sprite))
        {
            game.player.score += 1;   // score + when powerup found
            apply_power_up(game, i);  // apply changes from power up
            remove_power_up(game, i); // remove power up from game
        }
    }

    for (int i = game.virus.size() - 1; i >= 0; i--)
    {
        if (sprite_collision(game.player.player_sprite, game.virus[i].virus_sprite))
        {
            game.player.infection += 1; // infection + when powerup found
            apply_virus(game, i);       // apply changes from virus
            remove_virus(game, i);      // remove virus from game
        }
    }
}

void apply_virus(game_data &game, int index) // Different virus impact health
{
    if (game.virus[index].kind == VIRUS1) // 5% damage to player
    {
        if (game.player.virus_pct < 1.00)
        {
            game.player.virus_pct += 0.10;  // adds to virus points (double)
            game.player.health_pct -= 0.05; // virus impacts health (syringe)
            play_sound_effect("infection_sound");
        }
    }
    else if (game.virus[index].kind == VIRUS2) // 10% damage to player
    {
        if (game.player.virus_pct < 1.00)
        {
            game.player.virus_pct += 0.10;  // adds to virus points (double)
            game.player.health_pct -= 0.10; // virus impacts health (syringe)
            play_sound_effect("infection_sound");
        }
    }
    else if (game.virus[index].kind == VIRUS3) // 15% damage to player
    {
        if (game.player.virus_pct < 1.00)
        {
            game.player.virus_pct += 0.10;  // adds to virus points (double)
            game.player.health_pct -= 0.15; // virus impacts health (syringe)
            play_sound_effect("infection_sound");
        }
    }
}

void apply_power_up(game_data &game, int index) // power ups: fuel, life, bonuses and sound
{
    if (game.power_ups[index].kind == CHEESE) // assignment statement
    {
        if (game.player.cheese_pct <= 1.00)
        {
            game.player.cheese_pct += 0.20;
            play_sound_effect("bonus_sound");
        }
    }
    else if (game.power_ups[index].kind == SYRINGE)
    {
        if (game.player.health_pct <= 1.00)
        {
            game.player.health_pct += 0.20;
            play_sound_effect("bonus_sound");
        }
    }
    else if (game.power_ups[index].kind == CAKE)
    {
        if (game.player.cake_pct < 1.00)
        {
            game.player.cake_pct += 1.00;
            play_sound_effect("power_up_sound");
        }
    }
    else if (game.power_ups[index].kind == HORSE)
    {
        if (game.player.horse_pct < 1.00)
        {
            game.player.horse_pct += 1.00;
            play_sound_effect("power_up_sound");
        }
    }
    else if (game.power_ups[index].kind == DUCK)
    {
        if (game.player.duck_pct < 1)
        {
            game.player.duck_pct += 1.00;
            play_sound_effect("power_up_sound");
        }
    }
    else if (game.power_ups[index].kind == BURGER)
    {
        if (game.player.burger_pct < 1.00)
        {
            game.player.burger_pct += 1.00;
            play_sound_effect("power_up_sound");
        }
    }
}

void remove_power_up(game_data &game, int index) // remove power ups from game
{
    int selected_idx = game.power_ups.size() - 1;         // make a new last index for removal
    game.power_ups[index] = game.power_ups[selected_idx]; // points from index to removal index
    game.power_ups.pop_back();                            // removed the power up
}

void remove_virus(game_data &game, int index) // remove virus from game
{
    int selected_idx = game.virus.size() - 1;     // make a new last index for removal
    game.virus[index] = game.virus[selected_idx]; // points from index to removal index
    game.virus.pop_back();                        // removed the virus
}

void draw_hud(const game_data &game) // player_data in parameter which passes by reference. //(HUD) Heads up display
{
    // timer countdown
    draw_text("TIMER: " + to_string((TIME - timer_ticks("MY_TIMER")) / 1000), COLOR_WHITE, 675, 730, option_to_screen()); // 60 seconds count down timer

    // screen framing top HUD
    draw_rectangle(COLOR_WHITE, 0, 0, 1200, 130, option_to_screen()); // y, x

    // Top clock timer small menu drawing HUD
    draw_rectangle(COLOR_WHITE, 130, 0, 150, 130, option_to_screen()); // y, x
    draw_bitmap("time", 175, 30, option_part_bmp(0, 0, bitmap_width("time") * game.timer_pct, bitmap_height("time"), option_to_screen()));
    draw_text("TIMER: ", COLOR_WHITE, 180, 110, option_to_screen());

    // draw_bitmap("burger_small", 400, 30, option_part_bmp(0, 0, bitmap_width("burger_small") * game.player.burger_pct, bitmap_height("burger_small"), option_to_screen()));
    draw_bitmap("syringe_small", 350, 30, option_part_bmp(0, 0, bitmap_width("syringe_small") * game.player.health_pct, bitmap_height("syringe_small"), option_to_screen()));
    draw_bitmap("cheese_small", 470, 30, option_part_bmp(0, 0, bitmap_width("cheese_small") * game.player.cheese_pct, bitmap_height("cheese_small"), option_to_screen()));
    draw_bitmap("cake_small", 570, 30, option_part_bmp(0, 0, bitmap_width("cake_small") * game.player.cake_pct, bitmap_height("cake_small"), option_to_screen()));
    draw_bitmap("duck_small", 670, 30, option_part_bmp(0, 0, bitmap_width("duck_small") * game.player.duck_pct, bitmap_height("duck_small"), option_to_screen()));
    draw_bitmap("horse_small", 770, 30, option_part_bmp(0, 0, bitmap_width("horse_small") * game.player.horse_pct, bitmap_height("horse_small"), option_to_screen()));
    draw_bitmap("burger_small", 870, 30, option_part_bmp(0, 0, bitmap_width("burger_small") * game.player.burger_pct, bitmap_height("burger_small"), option_to_screen()));
    draw_text("COLLECTIONS: ", COLOR_WHITE, 600, 110, option_to_screen());

    // instructions
    draw_rectangle(COLOR_WHITE, 1000, 0, 220, 130, option_to_screen());
    draw_text("PRESS 1 HARRY", COLOR_WHITE, 1040, 20, option_to_screen());
    draw_text("PRESS 2 FRANCIS", COLOR_WHITE, 1040, 50, option_to_screen());
    draw_text("PRESS 3 CHARLIE", COLOR_WHITE, 1040, 80, option_to_screen());
    draw_text("EXIT Press X ", COLOR_WHITE, 1040, 110, option_to_screen());

    // (HUD) background shape
    draw_rectangle(COLOR_WHITE, 0, 670, 1200, 130, option_to_screen());

    // (HUD) rectangle divider
    draw_rectangle(COLOR_WHITE, 440, 670, 180, 130, option_to_screen()); // 1st box
    draw_rectangle(COLOR_WHITE, 620, 670, 180, 130, option_to_screen()); // 2nd box
    draw_rectangle(COLOR_WHITE, 800, 670, 180, 130, option_to_screen()); // 3rd box

    // Level
    draw_text("LEVEL: " + to_string(game.player.level), COLOR_WHITE, 495, 730, option_to_screen());

    // INFECTIONS
    draw_text("INFECTIONS: " + to_string(game.player.infection), COLOR_WHITE, 1040, 730, option_to_screen());

    // Score
    draw_text("SCORE: " + to_string(game.player.score), COLOR_WHITE, 855, 730, option_to_screen());

    // location
    draw_text("LOCATION: " + point_to_string(center_point(game.player.player_sprite)), COLOR_WHITE, 30, 780, option_to_screen());

    // cheese gauge
    draw_text("CHEESE: ", COLOR_WHITE, 30, 700, option_to_screen());
    draw_bitmap("bar_white_fill", 100, 680, option_part_bmp(0, 0, bitmap_width("bar_white_fill"), bitmap_height("bar_white_fill"), option_to_screen()));
    draw_bitmap("bar_grey_fill", 100, 680, option_part_bmp(0, 0, bitmap_width("bar_grey_fill") * game.player.cheese_pct, bitmap_height("bar_grey_fill"), option_to_screen()));

    // health y, x
    draw_text("HEALTH: ", COLOR_WHITE, 30, 750, option_to_screen());
    draw_bitmap("bar_white_fill", 100, 730, option_part_bmp(0, 0, bitmap_width("bar_white_fill"), bitmap_height("bar_white_fill"), option_to_screen()));
    draw_bitmap("bar_grey_fill", 100, 730, option_part_bmp(0, 0, bitmap_width("bar_grey_fill") * game.player.health_pct, bitmap_height("bar_grey_fill"), option_to_screen()));

    // draw mini_map
    draw_rectangle(COLOR_WHITE, 0, 0, 130, 130, option_to_screen());
    draw_minimap(game);
}

point_2d mini_map_coordinate_power_up(const power_up_data &game) // power_up sprite coordinates
{
    float power_up_x = sprite_x(game.power_up_sprite);
    float power_up_y = sprite_y(game.power_up_sprite);
    double mini_map_x = (power_up_x - MIN_SIZE) / MAX_GAME_SIZE * 100 + 15;
    double mini_map_y = (power_up_y - MIN_SIZE) / MAX_GAME_SIZE * 100 + 15;
    return point_at(mini_map_x, mini_map_y);
}

point_2d mini_map_coordinate_player(const player_data &game) // player sprite coordinates
{
    float player_x = sprite_x(game.player_sprite);
    float player_y = sprite_y(game.player_sprite);
    double mini_map_x = (player_x - MIN_SIZE) / MAX_GAME_SIZE * 100 + 15;
    double mini_map_y = (player_y - MIN_SIZE) / MAX_GAME_SIZE * 100 + 15;
    return point_at(mini_map_x, mini_map_y);
}

void draw_minimap(const game_data &game) // draw mini map with reference to power_up and play coordinates
{
    for (int i = 0; i < game.power_ups.size(); i++)
    {
        point_2d map;
        map = mini_map_coordinate_power_up(game.power_ups[i]);
        draw_pixel(COLOR_WHITE, map, option_to_screen());
    }
    // Drawing the position of the player red colour
    draw_pixel(COLOR_RED, mini_map_coordinate_player(game.player), option_to_screen());
}
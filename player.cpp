#include "player.h"
#include "splashkit.h"

/**
 * The ship bitmap function converts a ship kind into a
 * bitmap that can be used.
 *
 * Not exposed by the header.
 *
 * @param kind  The kind of ship you want the bitmap of
 * @return      The bitmap matching this ship kind
 */
bitmap ship_bitmap(ship_kind kind)
{
    switch (kind)
    {
    case UFO1:
        return bitmap_named("ufo1");
    case UFO2:
        return bitmap_named("ufo2");
    default:
        return bitmap_named("ufo3");
    }
}

player_data new_player()
{
    player_data result;
    bitmap default_bitmap = ship_bitmap(UFO1);

    // Create the sprite with 3 layers - we can turn on and off based
    result.player_sprite = create_sprite(default_bitmap); // on the ship kind selected

    sprite_add_layer(result.player_sprite, ship_bitmap(UFO2), "UFO2");
    sprite_add_layer(result.player_sprite, ship_bitmap(UFO3), "UFO3");

    // Default to layer 0 = UFO1 so hide others
    sprite_hide_layer(result.player_sprite, 1);
    sprite_hide_layer(result.player_sprite, 2);

    result.kind = UFO1;

    // Position in the centre of the initial screen
    // UFO1 (78w x 46h). 78/2 = 39-400 = 361x horizontal (46/2 = 23 - 400 = 377y vertical)
    sprite_set_x(result.player_sprite, 361); // horizontal
    sprite_set_y(result.player_sprite, 377); // vertical

    // cheese, syringe and objects initial values * using result
    result.cheese_pct = 0.95;
    result.health_pct = 0.95;
    result.cake_pct = 0.00;
    result.duck_pct = 0.00;
    result.horse_pct = 0.00;
    result.burger_pct = 0.00;
    result.score = 0;
    result.level = 1;

    // virus initial values * using result
    result.infection = 0;
    result.virus_pct = 0.00;
    return result;
}

void draw_player(const player_data &player_to_draw)
{
    draw_sprite(player_to_draw.player_sprite);
}

void update_player(player_data &player_to_update)
{
    // Apply movement based on rotation and velocity in the sprite
    update_sprite(player_to_update.player_sprite);

    // Test edge of screen boundaries to adjust the camera
    double left_edge = camera_x() + SCREEN_BORDER;
    double right_edge = left_edge + screen_width() - 2 * SCREEN_BORDER;
    double top_edge = camera_y() + SCREEN_BORDER;
    double bottom_edge = top_edge + screen_height() - 2 * SCREEN_BORDER;

    // Get the center of the player
    point_2d sprite_center = center_point(player_to_update.player_sprite);

    // Test if the player is outside the area and move the camera
    // the player will appear to stay still and everything else
    // Test top/bottom of screen
    if (sprite_center.y < top_edge)
    {
        move_camera_by(0, sprite_center.y - top_edge);
    }
    else if (sprite_center.y > bottom_edge)
    {
        move_camera_by(0, sprite_center.y - bottom_edge);
    }

    // Test left/right of screen
    if (sprite_center.x < left_edge)
    {
        move_camera_by(sprite_center.x - left_edge, 0);
    }
    else if (sprite_center.x > right_edge)
    {
        move_camera_by(sprite_center.x - right_edge, 0);
    }

    // at 0 the update is activated to decrease fuel or life
    if (player_to_update.cheese_pct > 0) // decrease fuel as time increases
    {
        player_to_update.cheese_pct -= 0.000278;
    }

    if (player_to_update.health_pct > 0) // decrease life as time increases
    {
        player_to_update.health_pct -= 0.000278;
    }
}

void player_switch_to_ship(player_data &player, ship_kind target)
{
    // only do this if there is a change
    if (player.kind != target)
    {
        // show then hide layers
        sprite_show_layer(player.player_sprite, static_cast<int>(target));
        sprite_hide_layer(player.player_sprite, static_cast<int>(player.kind));

        // remember what is currently shown
        player.kind = target;
    }
}

void handle_input(player_data &player)
{
    // Allow the player to switch ships
    if (key_typed(NUM_1_KEY))
    {
        player_switch_to_ship(player, UFO1);
    }
    if (key_typed(NUM_2_KEY))
    {
        player_switch_to_ship(player, UFO2);
    }
    if (key_typed(NUM_3_KEY))
    {
        player_switch_to_ship(player, UFO3);
    }

    // Handle movement - rotating left/right and moving forward/back
    float dx = sprite_dx(player.player_sprite);
    float rotation = sprite_rotation(player.player_sprite);

    // Allow rotation with left/right keys
    if (key_down(LEFT_KEY))
    {
        sprite_set_rotation(player.player_sprite, rotation - PLAYER_ROTATE_SPEED);
    }
    if (key_down(RIGHT_KEY))
    {
        sprite_set_rotation(player.player_sprite, rotation + PLAYER_ROTATE_SPEED);
    }

    // Increase speed with up/down keys - typed to give step increases
    if (key_typed(DOWN_KEY))
    {
        sprite_set_dx(player.player_sprite, dx - PLAYER_SPEED);
    }
    if (key_typed(UP_KEY))
    {
        sprite_set_dx(player.player_sprite, dx + PLAYER_SPEED);
    }
}

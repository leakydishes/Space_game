#ifndef LOST_IN_SPACE_PLAYER
#define LOST_IN_SPACE_PLAYER

#include "splashkit.h"

#include <vector>

using namespace std;

#define PLAYER_SPEED 1.5
#define PLAYER_ROTATE_SPEED 3
#define SCREEN_BORDER 100

/**
 * Different options for the kind of ship.
 * Adjusts the image used.
 */
enum ship_kind
{
    UFO1,
    UFO2,
    UFO3
};

/**
 * The player data keeps track of all of the information related to the player.
 *
 * @field   player_sprite   The player's sprite - used to track position and movement
 * @field   score           The current score for the player
 * @field   kind            Current kind of player ship
 */
struct player_data
{
    sprite player_sprite;
    int score;
    int level;
    int infection;
    ship_kind kind;
    double cheese_pct;
    double health_pct;
    double burger_pct;
    double duck_pct;
    double horse_pct;
    double cake_pct;
    double virus_pct;
};

/**
 * Creates a new player in the centre of the screen with the default ship.
 * Returns the power up amounts.
 * @returns     The new player data
 */
player_data new_player();

/**
 * Draws the player to the screen.
 *
 * @param player_to_draw    The player to draw to the screen
 */
void draw_player(const player_data &player_to_draw);

/**
 * Actions a step update of the player - moving them and adjusting the camera.
 *
 * @param player_to_update      The player being updated
 */
void update_player(player_data &player_to_update);

/**
 * Read user input and update the player based on this interaction.
 *
 * @param player    The player to update, decreases fuel and life as time increases
 */
void handle_input(player_data &player);

#endif
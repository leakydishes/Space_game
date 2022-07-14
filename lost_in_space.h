#ifndef LOST_IN_SPACE
#define LOST_IN_SPACE

#include "splashkit.h"
#include "power_up.h"
#include "player.h"
#include "virus.h"

#include <vector>
using std::vector;
using namespace std;

#define PLAYER_SPEED 1.5
#define PLAYER_ROTATE_SPEED 3
#define SCREEN_BORDER 100

/**
 * @brief manages all of the games data including player and power ups
 *
 */
struct game_data
{
    player_data player;
    vector<power_up_data> power_ups;
    vector<virus_data> virus;
    bool menu;
    timer game_timer;
    double timer_pct;
};

/**
 * Loads the game images, sounds. audio
 * @brief loads game resources and sound
 *
 */
void load_resources();

/**
 * @brief start menu displayed
 *
 * @param game uses game data, user presses enter to proceed
 */
void menu(const game_data &game);

/**
 * @brief menu for game
 *
 *  @param game displays score, level and infection from user
 */
void menu_screen(const game_data &game);

/**
 * @brief boolean logic for game play, activates game once user presses enter
 *
 * @param game game data to proceed and load game
 * @return true
 * @return false
 */
bool menu_select(game_data &game);

/**
 * @brief game over screen
 *
 * @param game displays score, level and infection from user
 */
void game_over(game_data &game);

/**
 * @brief declares a new game
 *
 * @return game_data and game content
 */
game_data new_game();

/**
 * @brief loads menu bool value
 *
 * @param game
 * @return true
 * @return false
 */
void start_game(game_data &game);

/**
 * @brief Checks power up position in game
 *
 * @param game Checks power up position and adjusts to game size (global constants)
 */
void check_power_up_position(game_data &game);

/**
 * @brief Checks virus position in game
 *
 * @param game virus deleted if outside global constants
 */
void check_virus_position(game_data &game);

/**
 * @brief  updates power up 5%, check collisions,
 *         add power up --> connects to new power up
 */
void update_game(game_data &game); // updates the game

/**
 * @brief draw HUD, Draw Power up, draw player
 *
 * @param game draws the game and content
 */
void play_game(const game_data &game);

/**
 * @brief power ups to game at random position
 *
 * @param game draws the game and content
 */
void add_power_up(game_data &game);

/**
 * @brief adds virus to game at random position
 *
 * @param game draws the game and content
 */
void add_virus(game_data &game);

/**
 * @brief Checks the level of score, increases by 1 point every 10 collected score points
 *
 * @param game
 */
void check_level(game_data &game);

/**
 * @brief Checks collisions in game
 *
 * @param game removes a power up and influence of player
 */
void check_collisions(game_data &game);

/**
 * @brief applies power ups, fuel, life, bonuses
 *
 * @param game in game settings
 * @param index calls fuel, life and bonuses with sound effects
 */
void apply_power_up(game_data &game, int index);

/**
 * @brief applies virus and virus impacting health --> syringe
 *
 * @param game in game settings
 * @param index calls virus index
 */
void apply_virus(game_data &game, int index);

/**
 * @brief removes power up
 *
 * @param game in game removes power up from game.
 * @param index creates last index for removal and selects the index required to delete
 */
void remove_power_up(game_data &game, int index);

/**
 * @brief removes virus
 *
 * @param game in game removes virus from game.
 * @param index creates last index for removal and selects the index required to delete
 */
void remove_virus(game_data &game, int index);

/**
 * @brief draws the hud display for player
 *
 * @param player players actions determine the values in hud
 */
void draw_hud(const game_data &game);

/**
 * @brief draws mini map to screen
 *
 * @param game using game data
 */
void draw_minimap(const game_data &game);

/**
 * power_up sprite coordinate data
 * @brief calculates the mini map coordinate using power up data x, y positions
 *
 * @param game using game data
 * @return point_2d using x, y calls from mini_map
 */
point_2d mini_map_coordinate(const power_up_data &game);

/**
 * player sprite coordinate data
 * @brief calculates the mini map coordinate using player data x, y positions
 *
 * @param game using game data
 * @return point_2d using x, y calls from mini_map
 */
point_2d mini_map_coordinate_player(const player_data &game);

#endif
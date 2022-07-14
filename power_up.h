#ifndef LOST_IN_SPACE_POWER_UP
#define LOST_IN_SPACE_POWER_UP

#include "splashkit.h"
#include <vector>

// using namespace std;

enum power_up_kind
{
    DUCK,
    BURGER,
    CAKE,
    HORSE,
    CHEESE,
    SYRINGE
};
/**
 * @brief power_up_data tracks all details for the power ups
 * power_up_sprite tracks position
 * kind the selected power up
 */
struct power_up_data
{
    sprite power_up_sprite;
    power_up_kind kind;
};

/**
 * @brief Randomised power up appears on screen
 *
 * @param x horiztonal position
 * @param y vertical position
 * @return power_up_data
 */
power_up_data new_power_up(double x, double y);

/**
 * @brief draw power to screen
 *
 * @param power_to_draw procedure to draw power to screen
 */
void draw_power_up(const power_up_data &power_to_draw);

/**
 * @brief Camera, actions and position updates of power ups
 *
 * @param power_to_update update action
 */
void update_power_up(power_up_data &power_to_update);

#endif
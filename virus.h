#ifndef LOST_IN_SPACE_VIRUS
#define LOST_IN_SPACE_VIRUS

#include "splashkit.h"
#include <vector>

// using namespace std;

enum virus_kind
{
    VIRUS1,
    VIRUS2,
    VIRUS3,
};
/**
 * @brief  tracks all details for the virus
 * virus_sprite tracks position
 * kind the selected virus
 */
struct virus_data
{
    sprite virus_sprite;
    virus_kind kind;
};

/**
 * @brief Randomised virus appears on screen
 *
 * @param x horiztonal position
 * @param y vertical position
 * @return virus_data
 */
virus_data new_virus(double x, double y);

/**
 * @brief draw virus to screen
 *
 * @param virus_to_draw procedure to draw virus to screen
 */
void draw_virus(const virus_data &virus_to_draw);

/**
 * @brief Camera, actions and position updates of virus
 *
 * @param virus_to_update update action
 */
void update_virus(virus_data &virus_to_update);

#endif
#include "virus.h"
#include "splashkit.h"

// virus types
bitmap virus_bitmap(virus_kind kind)
{
    switch (kind)
    {
    case VIRUS1:
        return bitmap_named("virus1");
    case VIRUS2:
        return bitmap_named("virus2");
    case VIRUS3:
        return bitmap_named("virus3");
    default:
        return bitmap_named("virus1");
    }
}

// creating new virus
virus_data new_virus(double x, double y)
{
    virus_kind kind;
    virus_data result;

    // random virus kind called
    kind = static_cast<virus_kind>(rnd(3));

    // virus bitmap
    result.virus_sprite = create_sprite(virus_bitmap(kind));

    // memory of virus type
    result.kind = kind;

    // position of the virus
    sprite_set_x(result.virus_sprite, x);
    sprite_set_y(result.virus_sprite, y);

    // velocity added
    sprite_set_dx(result.virus_sprite, rnd() * 4 - 2);
    sprite_set_dy(result.virus_sprite, rnd() * 4 - 2);
    return result;
}

// draw virus
void draw_virus(const virus_data &virus_to_draw)
{
    draw_sprite(virus_to_draw.virus_sprite); // drawing virus to screen
}

// update virus
void update_virus(virus_data &virus_to_update) // updating virus
{
    update_sprite(virus_to_update.virus_sprite);
}
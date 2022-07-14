#include "power_up.h"
#include "splashkit.h"

// Power up types
bitmap power_up_bitmap(power_up_kind kind)
{
    switch (kind)
    {
    case DUCK:
        return bitmap_named("duck");
    case BURGER:
        return bitmap_named("burger");
    case CAKE:
        return bitmap_named("cake");
    case HORSE:
        return bitmap_named("horse");
    case CHEESE:
        return bitmap_named("cheese");
    case SYRINGE:
        return bitmap_named("syringe");
    default:
        return bitmap_named("burger");
    }
}

// creating new power ups
power_up_data new_power_up(double x, double y)
{
    power_up_kind kind;
    power_up_data result;

    // random power up kind called
    kind = static_cast<power_up_kind>(rnd(6));

    // power up bitmap
    result.power_up_sprite = create_sprite(power_up_bitmap(kind));

    // memory of powerup type
    result.kind = kind;

    // position of the power up
    sprite_set_x(result.power_up_sprite, x);
    sprite_set_y(result.power_up_sprite, y);

    // velocity added
    sprite_set_dx(result.power_up_sprite, rnd() * 4 - 2);
    sprite_set_dy(result.power_up_sprite, rnd() * 4 - 2);
    return result;
}

// draw power ups
void draw_power_up(const power_up_data &power_to_draw)
{
    draw_sprite(power_to_draw.power_up_sprite); // drawing power up to screen
}

// update power ups
void update_power_up(power_up_data &power_to_update) // updating power up
{
    update_sprite(power_to_update.power_up_sprite);
}
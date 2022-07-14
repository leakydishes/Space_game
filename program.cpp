#include "splashkit.h"
#include "player.h"
#include "power_up.h"
#include "lost_in_space.h"
#include "virus.h"

using namespace std;

int main() // main game & game loop
{
    open_window("Lost In Space, By Te' Claire", 1200, 800);
    load_resources();

    game_data my_game;
    my_game = new_game();
    play_music("lofi");

    while (!quit_requested() && !key_down(X_KEY))
    {
        process_events(); // checks user input

        if (my_game.menu) // menu
        {
            menu_screen(my_game);
            my_game.menu = menu_select(my_game); // Enter starts game in menu_select
        }
        else // game
        {
            handle_input(my_game.player); // player position
            update_game(my_game);         // Perform movement and update the camera
            clear_screen(COLOR_BLACK);    // redraw everything
            play_game(my_game);           // Redraw everything
            game_over(my_game);
            refresh_screen(60);
        }
    }
    close_audio();
    return 0;
}
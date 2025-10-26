#include "jester/jester.h"

int main()
{
    const int player_xp = 50;

    LOG_INIT_DEFAULT();

    set_min_log_level(DEBUG);

    toggle_file(true);

    LOG_DEBUG("player_xp = %d", player_xp);
    LOG_INFO("player_xp = %d", player_xp);
    LOG_WARNING("player_xp = %d", player_xp);
    LOG_ERROR("player_xp = %d", player_xp);
    LOG_FATAL("player_xp = %d", player_xp);

    toggle_color(true);

    LOG_DEBUG("player_xp = %d", player_xp);
    LOG_INFO("player_xp = %d", player_xp);
    LOG_WARNING("player_xp = %d", player_xp);
    LOG_ERROR("player_xp = %d", player_xp);
    LOG_FATAL("player_xp = %d", player_xp);

    return 0;
}

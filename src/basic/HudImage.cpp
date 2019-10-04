#include "HudImage.h"
#include <GameManager.h>

SDL_FRect basic::HudImage::get_dest_rect () {
    const hf::GameManager *manager = hf::GameManager::get_instance();

    int window_w;
    int window_h;
    manager->get_window_size(&window_w, &window_h);

    SDL_FRect dest_rect;
    dest_rect.w = (float)source_rect.w * scale.x;
    dest_rect.h = (float)source_rect.h * scale.y;
    dest_rect.x = (float)(window_w) * screen_anchor.x + position.x - dest_rect.w * pivot.x;
    dest_rect.y = (float)(window_h) * screen_anchor.y - position.y - dest_rect.h * pivot.y;

    return dest_rect;
}

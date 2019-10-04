#include "Image.h"
#include <GameManager.h>
#include <cmath>

void basic::Image::draw() {
    if(texture != NULL){
        SDL_SetTextureColorMod(texture, (Uint8)color.r, (Uint8)color.g, (Uint8)color.b);
        SDL_SetTextureAlphaMod(texture, (Uint8)color.a);

        const hf::GameManager* manager = hf::GameManager::get_instance();

        const hf::Vec2 camera_pos = manager->get_current_scene()->camera_position;
        float camera_zoom = manager->get_current_scene()->camera_zoom;

        SDL_FRect dest_rect = get_dest_rect();

        SDL_FPoint center;
        center.x = pivot.x * dest_rect.w;
        center.y = pivot.y * dest_rect.h;

        manager->draw_texture(this->texture, source_rect, dest_rect, center, rotation);
    }
}

SDL_FRect basic::Image::get_dest_rect(){
    const hf::GameManager* manager = hf::GameManager::get_instance();

    const hf::Vec2 camera_pos = manager->get_current_scene()->camera_position;
    float camera_zoom = manager->get_current_scene()->camera_zoom;

    int window_w;
    int window_h;
    manager->get_window_size(&window_w, &window_h);

    SDL_FRect dest_rect;
    dest_rect.w = (float)source_rect.w * scale.x * camera_zoom;
    dest_rect.h = (float)source_rect.h * scale.y * camera_zoom;
    dest_rect.x = (float)(window_w) * .5f + position.x * camera_zoom - camera_pos.x * camera_zoom - dest_rect.w * pivot.x;
    dest_rect.y = (float)(window_h) * .5f - position.y * camera_zoom + camera_pos.y * camera_zoom - dest_rect.h * pivot.y;

    return dest_rect;
}

SDL_Rect basic::Image::get_dest_rect_i(){
    SDL_FRect f_rect = get_dest_rect();

    SDL_Rect i_rect;
    i_rect.x = (int)std::roundl(f_rect.x);
    i_rect.y = (int)std::roundl(f_rect.y);
    i_rect.w = (int)std::roundl(f_rect.w);
    i_rect.h = (int)std::roundl(f_rect.h);
    return i_rect;
}


void basic::Image::set_texture(SDL_Texture *texture) {
    this->texture = texture;

    if(texture != NULL){
        int tex_w;
        int tex_h;
        SDL_QueryTexture(texture, NULL, NULL, &tex_w, &tex_h);
        set_region(0, 0, tex_w, tex_h);
    }
}

/**
 * \brief Sets the region to be copied from the texture and presented on the screen
 * \param x The horizontal coordinate in the texture where the region begins
 * \param y The vertical coordinate in the texture where the region begins
 * \param width The width of the region
 * \param height The height of the region
 */
void basic::Image::set_region(int x, int y, int width, int height){
    this->source_rect.x = x;
    this->source_rect.y = y;
    this->source_rect.w = width;
    this->source_rect.h = height;
}
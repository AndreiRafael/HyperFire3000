#ifndef BASIC_IMAGE_H
#define BASIC_IMAGE_H

#include <Entity.h>
#include <SDL.h>
#include <Vec2.h>
#include <Color.h>

namespace basic {
class Image : public hf::Entity {
    protected:
        SDL_Texture* texture;
        SDL_Rect source_rect;

        virtual void draw() override;
    public:
        hf::Vec2 position = hf::Vec2();
        hf::Vec2 scale = hf::Vec2(1.f);
        float rotation = 0.f;
        hf::Vec2 pivot = hf::Vec2(.5f);
        hf::Color color = hf::Color(255.f);

        void set_texture(SDL_Texture* texture);
        void set_region(int x, int y, int width, int height);

        /**
         * \brief Gets the screen rect to which the image is rendered
         */
        virtual SDL_FRect get_dest_rect();

        /**
         * \brief Gets an int version of get_dest_rect
         */
        SDL_Rect get_dest_rect_i();
};
}

#endif //BASIC_IMAGE_H

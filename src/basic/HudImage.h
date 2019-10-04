#ifndef BASIC_HUDIMAGE_H
#define BASIC_HUDIMAGE_H

#include "Image.h"

namespace basic {
    class HudImage : public Image {
    protected:
        virtual SDL_FRect get_dest_rect() override;

    public:
        hf::Vec2 screen_anchor = hf::Vec2(.5f);
    };
}

#endif //BASIC_IMAGE_H

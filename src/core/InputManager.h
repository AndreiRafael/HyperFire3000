#ifndef HF_INPUTMANAGER_H
#define HF_INPUTMANAGER_H

#include <SDL.h>

namespace hf {
class InputManager {
    private:
        static InputManager* instance;

        InputManager(){}
        ~InputManager(){}
    public:
        static InputManager* get_instance();

        /**
         * \brief Transforms a normalized touch coordinate into a screen coordinate
         */
        static void normalized_to_screen_coordinate(const float normalized_x, const float normalized_y, int* const screen_x, int* const screen_y);

        /*
         * \brief Processes all input of a frame and updates related structures
         */
        void update();
    };
}

#endif
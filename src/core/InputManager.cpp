#include "InputManager.h"
#include "GameManager.h"

//INPUT MANAGER
hf::InputManager* hf::InputManager::instance = nullptr;

hf::InputManager* hf::InputManager::get_instance() {
    if(instance == nullptr){
        instance = new InputManager();
    }
    return instance;
}

void hf::InputManager::normalized_to_screen_coordinate(const float normalized_x, const float normalized_y, int* const screen_x, int* const screen_y) {
    const hf::GameManager* const game_manager = hf::GameManager::get_instance();
    int window_w;
    int window_h;
    game_manager->get_window_size(&window_w, &window_h);

    if(screen_x != nullptr) {
        *screen_x = (int)((float)window_w * normalized_x);
    }
    if(screen_y != nullptr) {
        *screen_y = (int)((float)window_h * normalized_y);
    }
}

void hf::InputManager::update() {
    auto scene = GameManager::get_instance()->get_current_scene();

    SDL_Event e;
    while(SDL_PollEvent(&e) != 0){
        if(scene != nullptr) {
            scene->broadcast_event(e);
            scene->process_event(e);
        }
    }
}
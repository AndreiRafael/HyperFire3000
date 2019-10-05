#include "ExampleScene.h"
#include <Image.h>
#include <HudImage.h>
#include <GameManager.h>
#include <AudioManager.h>
#include <SDL_image.h>
#include <algorithm>
#include <cmath>
#include <list>

void ExampleScene::begin() {
    auto game_manager = hf::GameManager::get_instance();
    auto audio_manager = hf::AudioManager::get_instance();

    texture = game_manager->load_texture("./example.png");
    audio_clip = audio_manager->load_audio("./example.ogg");

    entity = add_entity<basic::Image>("entity");
    entity->set_texture(texture);
}

void ExampleScene::end() {
    SDL_DestroyTexture(texture);
}

void ExampleScene::update() {
    entity->rotation += hf::GameManager::get_instance()->get_delta_time() * 45.f;
}

void ExampleScene::process_event(const SDL_Event &sdl_event) {
    if(sdl_event.type == SDL_KEYDOWN && sdl_event.key.keysym.sym == SDLK_SPACE){
        auto audio_manager = hf::AudioManager::get_instance();
        audio_manager->play_audio(audio_clip);
    }
}
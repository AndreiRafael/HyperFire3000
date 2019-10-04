#include "GameScene.h"
#include <Image.h>
#include <HudImage.h>
#include <GameManager.h>
#include <AudioManager.h>
#include <SDL_image.h>
#include <algorithm>
#include <cmath>
#include <list>

void GameScene::begin() {
    auto game_manager = hf::GameManager::get_instance();

    auto audio_manager = hf::AudioManager::get_instance();
    audio_jump = audio_manager->load_audio("teste.ogg");

    int window_w;
    int window_h;
    game_manager->get_window_size(&window_w, &window_h);

    camera_zoom = 1.f / (80.f / (float)window_w);

    asset_tex = game_manager->load_texture("single_asset.png");
    bg_tex = game_manager->load_texture("bg.png");

    int bg_w;
    int bg_h;
    SDL_QueryTexture(bg_tex, NULL, NULL, &bg_w, &bg_h);
    bg_graphic = add_entity<basic::HudImage>("bg");
    bg_graphic->set_texture(bg_tex);
    bg_graphic->scale.x = std::ceil((float)window_w / (float)bg_w);
    bg_graphic->scale.y = std::ceil((float)window_h / (float)bg_h);
    bg_graphic->color = hf::Color(180.f, 180.f, 255.f);

    player_graphic = add_entity<basic::Image>("player", 1);
    player_graphic->set_texture(asset_tex);

    box_graphics.resize(20);
    for(int i = 0; i < box_graphics.size(); i++){
        box_graphics[i] = add_entity<basic::Image>("box_" + std::to_string(i), 1);
        box_graphics[i]->set_texture(asset_tex);
        box_graphics[i]->set_region(51, 42, 14, 8);
    }

    reset_scene();
}

void GameScene::end() {
    SDL_DestroyTexture(asset_tex);
    SDL_DestroyTexture(bg_tex);
}

void GameScene::update() {
    switch(game_state) {
        case GameState::GAME:
            game_update();
            break;//GAME
    }
}

void GameScene::process_event(const SDL_Event &sdl_event) {
    if(sdl_event.type == SDL_FINGERDOWN){
        int prev_lane = player_lane;
        if(sdl_event.tfinger.x > .5f){
            player_lane++;
        }
        else{
            player_lane--;
        }
        player_lane = std::clamp(player_lane, -2, 2); // TODO: Tá hardcoded aqui as lanes
        if(player_lane != prev_lane) {
            make_player_jump();
        }
    }
}

void GameScene::reset_scene() {
    auto game_manager = hf::GameManager::get_instance();

    int window_w;
    int window_h;
    game_manager->get_window_size(&window_w, &window_h);

    camera_zoom = 1.f / (80.f / (float)window_w);

    int bg_w;
    int bg_h;
    bg_graphic->color = hf::Color(180.f, 180.f, 255.f);// TODO: Custom color e tal

    player_graphic->position.y = (float)(-window_h / 2) / camera_zoom;
    player_graphic->position.x = 0.f;
    player_prev_x = 0.f;
    player_lane = 0;
    make_player_jump(2.25f);
    animate_player();
    recolor_player();

    for(int i = 0; i < box_graphics.size(); i++){
        box_graphics[i]->active = false;
    }

    next_obstacle = 0.f;
    camera_position = hf::Vec2();
}

void GameScene::set_obstacle(const int lane) {
    auto obstacle = box_graphics[box_index++];
    if(box_index >= box_graphics.size()){
        box_index = 0;
    }

    obstacle->position.y = next_obstacle;
    obstacle->position.x = (float)lane * 16.f;
    obstacle->color = get_color();
    obstacle->active = true;
}

void GameScene::apply_next_height() {
    next_obstacle += distance_between_obstacles;
}

void GameScene::make_player_jump(float multiplier){
    player_prev_x = player_graphic->position.x;
    player_vspeed = std::max(player_vspeed, player_jump_power * multiplier);
    player_lerp = 0.f;

    float p = (float)(rand() % 1000) / 2000.f + 0.75f;
    hf::AudioManager::get_instance()->play_audio(audio_jump);
}

void GameScene::recolor_player(){
    auto prev_color = player_graphic->color;
    do {
        player_graphic->color = color_options[rand() % color_options.size()];
    }while(prev_color.is_similar_to(player_graphic->color, 10.f));
}

void GameScene::animate_player(){
    float pct = abs(player_vspeed) / player_jump_power;

    if(pct > .8f){//esticado
        player_graphic->set_region(55, 0, 6, 16);
    }
    else if(pct > .6f){//amassando
        player_graphic->set_region(54, 16, 8, 14);
    }
    else{//amassado
        player_graphic->set_region(53, 30, 10, 12);
    }
}

hf::Color GameScene::get_color(){
    if(color_list.empty()){
        std::for_each(color_options.begin(), color_options.end(), [&] (hf::Color color) { color_list.push_back(color); } );
    }
    auto it = color_list.begin();
    std::advance(it, rand() % color_list.size());
    hf::Color c = *it;
    color_list.erase(it);
    return c;
}

void GameScene::change_state(const GameState new_state) {
    for_all_entities([] (hf::Entity* entity) { entity->active = false; });
    game_state = new_state;
}

void GameScene::game_update() {
    auto lerp = [] (const float a, const float b, const float time){
        float clamped = std::clamp(time, 0.f, 1.f);
        return a * (1.f - clamped) + b * clamped;
    };

    auto game_manager = hf::GameManager::get_instance();
    float delta_time = game_manager->get_delta_time();

    int window_h;
    game_manager->get_window_size(nullptr, &window_h);

    player_graphic->position.y += player_vspeed * delta_time;
    player_lerp += delta_time * 2.f * M_PI;
    player_lerp = std::clamp(player_lerp, 0.f, (float) M_PI / 2.f);
    float desired_pos = (float) player_lane * 16.f;
    player_graphic->position.x = lerp(player_prev_x, desired_pos, std::sin(player_lerp));
    animate_player();

    player_vspeed -= delta_time * 750.f;

    camera_position.y = lerp(camera_position.y,
                             std::max(camera_position.y, player_graphic->position.y),
                             2.f * delta_time);

    if (player_graphic->position.y + (float) window_h / camera_zoom > next_obstacle) {
        std::vector<bool> used_lanes(5, false);
        std::list<int> index_bank(used_lanes.size());
        int index = 0;
        std::transform(index_bank.begin(), index_bank.end(), index_bank.begin(),
                       [&index](int input) { return index++; });

        apply_next_height();
        int block_count = 2;
        while (block_count > 0 && !index_bank.empty()) {
            auto selected = index_bank.begin();
            int r = rand() % index_bank.size();
            std::advance(selected, r);

            int desired = *selected;
            int count = 0;
            int max = 0;
            int j = 0;

            for (int j = 0; j < used_lanes.size(); j++) {
                if (used_lanes[j] || j == desired) {
                    max = std::max(max, count);
                    count = 0;
                } else {
                    count++;
                }
            }
            max = std::max(max, count);

            if (max >= 2) {
                used_lanes[desired] = true;
                set_obstacle(desired - used_lanes.size() / 2);
                block_count--;
            }
            index_bank.erase(selected);
        }
    }

    if (player_graphic->position.y < camera_position.y - (float) (window_h) / camera_zoom) {
        reset_scene();
    }

    //Detecção das colisões
    auto p_rect = player_graphic->get_dest_rect_i();
    for (auto box : box_graphics) {
        if (!box->active) {
            continue;
        }
        auto b_rect = box->get_dest_rect_i();
        SDL_Rect i_rect;
        if (SDL_IntersectRect(&b_rect, &p_rect, &i_rect) == SDL_TRUE) {
            bool player_over =
                    player_vspeed < 0.f && player_graphic->position.y > box->position.y;

            if (player_over) {//colisão precisa se o player vem de cima
                if (box->color.is_similar_to(player_graphic->color, 10.f)) {
                    make_player_jump(1.25f);
                    box->active = false;
                    recolor_player();
                } else {
                    reset_scene();
                }
            } else {//colisão meio torta pro jogador não ficar bravo
                float area = (float) (i_rect.w * i_rect.h) / camera_zoom;
                if (area > 25.f) {
                    reset_scene();
                }
            }
        }
    }
}

void GameScene::menu_update() {

}

void GameScene::options_update() {

}

void GameScene::postgame_update() {

}
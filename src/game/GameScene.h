#ifndef GAMESCENE_H
#define GAMESCENE_H

#include <Scene.h>
#include <Image.h>
#include <list>

//este jogo é single cena e tal, mas normalmente a lógica não seria em uma cena só
class GameScene : public hf::Scene {
private:
    enum class GameState {
        MAIN_MENU = 0,
        GAME,
        POST_GAME,
        OPTIONS
    };

    GameState game_state = GameState::GAME;

    std::vector<hf::Color> color_options = { hf::Color(255.f, 0.f, 0.f), hf::Color(0.f, 0.f, 255.f), hf::Color(255.f, 215.f, 0.f), hf::Color(255.f, 255.f, 255.f)};
    std::list<hf::Color> color_list;

    const float distance_between_obstacles = 70.f;
    float next_obstacle = 0.f;

    SDL_Texture* asset_tex;
    SDL_Texture* bg_tex;

    basic::Image* bg_graphic;

    basic::Image* player_graphic;
    float player_vspeed = 400.f;
    float player_jump_power = 175.f;
    int player_lane = 0;
    float player_lerp = 1.f;
    float player_prev_x = 0.f;

    int audio_jump = -1;

    std::vector<basic::Image*> box_graphics;
    int box_index;

    //auto functions
    void begin() final override;
    void end() final override;
    void update() final override;
    void process_event(const SDL_Event& sdl_event) final override;
    //custom fuctions
    void reset_scene();

    void set_obstacle(const int lane);
    void apply_next_height();
    void make_player_jump(float multiplier = 1.f);
    void recolor_player();
    void animate_player();
    hf::Color get_color();

    void change_state(const GameState new_state);
    void game_update();
    void menu_update();
    void options_update();
    void postgame_update();
public:

};

#endif //GAMESCENE_H

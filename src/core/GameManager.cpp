#include "GameManager.h"
#include "InputManager.h"
#include "AudioManager.h"

#include <SDL_image.h>

hf::GameManager* hf::GameManager::instance = nullptr;

hf::GameManager* hf::GameManager::get_instance() {
	if (instance == nullptr) {
		instance = new GameManager();
	}
	return instance;
}

void hf::GameManager::clear_instance() {
	if (instance != nullptr) {
		delete instance;
	}
}

void hf::GameManager::init(const std::string title, int width, int height) {
	window = SDL_CreateWindow("NoTitle", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, 0);
    renderer = SDL_CreateRenderer(window, -1, 0);
	set_title(title);
}

void hf::GameManager::de_init() {
	if (renderer != nullptr) {
		SDL_DestroyRenderer(renderer);
	}
	if (window != nullptr) {
		SDL_DestroyWindow(window);
	}
}

void hf::GameManager::begin_loop() {
	while (!should_quit) {
	    if(prev_ticks != -1){
            delta_time = SDL_GetTicks() - prev_ticks;
	    }
	    prev_ticks = SDL_GetTicks();

		if(next_scene != nullptr){
			if(current_scene != nullptr){
				current_scene->end();
				current_scene->clear_entities();
				delete current_scene;
			}
			current_scene = next_scene;
			current_scene->begin();
			next_scene = nullptr;
		}

		hf::InputManager::get_instance()->update();

		hf::AudioManager::get_instance()->update();

		current_scene->update_entities();
		current_scene->update();

        SDL_SetRenderDrawColor(renderer, clear_color.r, clear_color.g, clear_color.b, clear_color.a);
        SDL_RenderClear(renderer);

        current_scene->draw_entities();
        current_scene->draw();

        SDL_RenderPresent(renderer);
	}
}

void hf::GameManager::quit() {
	should_quit = true;
}

SDL_Texture *hf::GameManager::load_texture(const char *file) const {
    return IMG_LoadTexture(renderer, file);
}

void hf::GameManager::draw_texture(SDL_Texture* texture, const SDL_Rect& source_rect, const SDL_FRect& dest_rect, const SDL_FPoint& center, const float angle) const{
    SDL_RenderCopyExF(renderer, texture, &source_rect, &dest_rect, angle, &center, SDL_FLIP_NONE);
}

float hf::GameManager::get_delta_time(){
    return (float)delta_time / 1000.f;
}

int hf::GameManager::get_delta_time_ms(){
    return delta_time;
}

std::string hf::GameManager::get_title() const {
	return title;
}

void hf::GameManager::get_display_size(int* const width, int* const height) const{
    SDL_DisplayMode dm;
    SDL_GetCurrentDisplayMode(0, &dm);

    if(width != nullptr){
        *width = dm.w;
    }
    if(height != nullptr){
        *height = dm.h;
    }
}

void hf::GameManager::get_window_size(int* const width, int* const height) const {
    if(window == nullptr){
        if(width != nullptr) {
            *width = 0;
        }
        if(height != nullptr) {
            *height = 0;
        }
    }
	SDL_GetWindowSize(window, width, height);
}

hf::Scene* hf::GameManager::get_current_scene() const{
	return current_scene;
}

//SETTERS

void hf::GameManager::set_title(const std::string& title) {
    if(title.length() == 0){
        return;
    }

	this->title = title;
	if (window != nullptr) {
		SDL_SetWindowTitle(window, title.c_str());
	}
}

void hf::GameManager::set_clear_color(const Uint8 r, const Uint8 g, const Uint8 b, const Uint8 a){
	clear_color.r = r;
	clear_color.g = g;
	clear_color.b = b;
	clear_color.a = a;
}
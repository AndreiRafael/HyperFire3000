#include <iostream>
#include <string>
#include <time.h>

#include "core/GameManager.h"
#include "core/InputManager.h"
#include "core/AudioManager.h"
#include <ExampleScene.h>

int main(int argc, char* argv[]) {
	srand(time(NULL));
    SDL_Init(SDL_INIT_EVERYTHING);

	auto manager = hf::GameManager::get_instance();
	auto audio_manager = hf::AudioManager::get_instance();

	audio_manager->init(32);

	int width;
    int height;
	manager->get_display_size(&width, &height);

	manager->init("Window", width, height);
	manager->set_clear_color(255, 0, 255, 255);

	manager->set_next_scene<ExampleScene>();
	manager->begin_loop();

	manager->clear_instance();
	SDL_Quit();
	return 0;
}
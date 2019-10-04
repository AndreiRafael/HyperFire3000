#ifndef HF_GAMEMANAGER_H
#define HF_GAMEMANAGER_H

#include <string>
#include <SDL.h>
#include "Scene.h"

namespace hf {
	class GameManager {
	private:
		GameManager() {}
		~GameManager() {}

		static GameManager* instance;

		std::string title;//Window title
		SDL_Window* window = nullptr;//the SDL_Window the app is drawn in
		SDL_Renderer* renderer = nullptr;//the SDL_Renderer used to draw the app
		SDL_Color clear_color;
		bool should_quit = false;
		int prev_ticks = -1;
		int delta_time = 0;

		Scene* current_scene = nullptr;
		Scene* next_scene = nullptr;

	public:
		static GameManager* get_instance();
		static void clear_instance();

		/**
		* \brief initializes the GameManager and all underlying systems
		* \param title A title for the app window
		* \param width The width of the window
		* \param height The height of the window
		*/
		void init(const std::string title, int width, int height);

		/**
		* \brief Deactivates the GameManager and all underlying systems
		*/
		void de_init();

		/**
		* \brief Starts the game loop, only to be stopped after a call to GameManager::quit()
		*/
		void begin_loop();

		/**
		* \brief Sets the GameManager up to quit que current game loop started with GameManager::begin()
		*/
		void quit();

		SDL_Texture* load_texture(const char* file) const;

		/**
		 * \brief Draws a texture on screen using the renderer of the GameManager
		 * \param texture The texture to be drawn
		 * \param source_rect The region of the texture to be drawn
		 * \param dest_rect The position on the screen where the texture will be drawn
		 * \param center The center of rotation of the texture
		 * \param angle The angle at which the texture will be drawn
		 */
        void draw_texture(SDL_Texture* texture, const SDL_Rect& source_rect, const SDL_FRect& dest_rect, const SDL_FPoint& center, const float angle) const;

        //getters

        /**
         * \brief Gets the time elapsed since the previous frame in seconds
         */
        float get_delta_time();

        /**
         * \brief Gets the time elapsed since the previous frame in miliseconds
         */
        int get_delta_time_ms();

		/**
		* \brief Gets the title that was set on init or set_title
		*/
		std::string get_title() const;

        /**
        * \brief Gets the dimensions of the display and stores the information in the provided pointers
        * \param width A pointer to be filled with the width of the display, might be nullptr if it is to be ignored
        * \param height A pointer to be filled with the height of the display, might be nullptr if it is to be ignored
        */
		void get_display_size(int* const width, int* const height) const;

		/**
		* \brief Gets the dimensions of the window and stores the information in the provided pointers
		* \param width A pointer to be filled with the width of the window, might be nullptr if it is to be ignored
		* \param height A pointer to be filled with the height of the window, might be nullptr if it is to be ignored
		*/
		void get_window_size(int* const width, int* const height) const;

		/*
		 * \brief Gets the current scene loaded by the GameManager
		 */
        Scene* get_current_scene() const;

		//setters
		void set_title(const std::string& title);

		void set_clear_color(const Uint8 r, const Uint8 g, const Uint8 b, const Uint8 a);

		template<typename SceneType>
		void set_next_scene(){
		    if(next_scene != nullptr){
                return;
		    }
		    next_scene = new SceneType();
		}
	};
}

#endif
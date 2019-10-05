#ifndef HF_SCENE_H
#define HF_SCENE_H

#include <SDL.h>
#include <vector>
#include <map>
#include <string>
#include <functional>
#include <string_view>
#include "Vec2.h"
#include "Entity.h"

namespace hf {
    class GameManager;
    class InputManager;
    class Scene {
    private:
        int id_generator = 0;
        std::multimap<int, Entity*> entities;

        void clear_entities();

        /**
         * \brief Broadcasts an event received from the GameManager to all entities of the scene
         * \param sdl_event The event sent by the GameManager
         */
        void broadcast_event(const SDL_Event& sdl_event);

        /*
         * \brief Updates all entities registered to this scene
         */
        void update_entities();

        /*
         * \brief Draws all entities registered to this scene
         */
        void draw_entities();

    protected:
        template<typename EntityType>
        EntityType* add_entity(std::string name, int layer = 0){
            EntityType* entity = new EntityType();
            entity->id = id_generator++;
            entity->name = name;
            entities.insert(std::make_pair(layer, entity));
            return entity;
        }

        void for_all_entities(std::function<void(Entity* entity)> function);

        template<typename EntityType>
        EntityType* get_entity(int id) {
            for(auto entity : entities){
                if(entity.second->id == id){
                    return dynamic_cast<EntityType*>(entity.second);
                }
            }
            return nullptr;
        }

        template<typename EntityType>
        EntityType* get_entity(std::string_view name) {
            for(auto entity : entities){
                if(entity.second->name.compare(name) == 0){
                    return dynamic_cast<EntityType*>(entity.second);
                }
            }
            return nullptr;
        }

        /*
         * \brief Custom scene function for user created Scenes, runs when the Scene is loaded by the manager
         */
        virtual void begin() {}

        /*
         * \brief Custom scene update for user created Scenes, runs once per frame
         */
        virtual void update() {}

        /*
         * \brief Custom scene draw function for user created Scenes, runs once per frame after all entities have been updated
         */
        virtual void draw() {}

        /*
         * \brief Custom scene function for user created Scenes, runs when the Scene is unloaded by the manager
         */
        virtual void end() {}

        virtual void process_event(const SDL_Event& sdl_event) {}
    public:
        hf::Vec2 camera_position;
        float camera_zoom = 1.f;

        friend class GameManager;
        friend class InputManager;
    };
}

#endif //HF_SCENE_H

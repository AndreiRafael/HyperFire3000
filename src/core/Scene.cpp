#include "Scene.h"
#include <algorithm>

void hf::Scene::clear_entities(){
    std::for_each(entities.begin(), entities.end(), [] (std::pair<int, Entity*> pair) { delete pair.second; });
}

void hf::Scene::broadcast_event(const SDL_Event& sdl_event){
    for(auto entity : entities){
        entity.second->process_event(sdl_event);
    }
}

void hf::Scene::update_entities(){
    for(auto it = entities.begin(); it != entities.end();){
        auto curr = it++;
        auto entity = curr->second;

        if(entity->should_destroy){
            delete entity;
            entities.erase(curr);
        }
        else if(entity->active){
            entity->update();
        }
    }
}

void hf::Scene::draw_entities(){
    for(auto it = entities.begin(); it != entities.end();){
        auto entity = it->second;

        if(!entity->should_destroy && entity->active) {
            it->second->draw();
        }
        it++;
    }
}

void hf::Scene::for_all_entities(std::function<void(Entity* entity)> function){
    for(auto it = entities.begin(); it != entities.end(); it++){
        function(it->second);
    }
}
#ifndef HF_ENTITY_H
#define HF_ENTITY_H

#include <string>
#include <SDL.h>

namespace hf{
    class Scene;
    class Entity {
    private:
        int id;
        std::string name;

        bool should_destroy = false;

    protected:
        virtual void update() {}
        virtual void draw() {}
        virtual void process_event(const SDL_Event& sdl_event) {}

    public:
        bool active = true;

        /*
         * \brief Sets the entity up for destruction, which takes effect the next time it is updated by the scene
         * \remarks Immediately after this has been called, neither update nor draw will execute, even though it might not have been deleted yet
         */
        void destroy();

        friend class Scene;
    };
}

#endif //HF_ENTITY_H

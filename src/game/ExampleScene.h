#ifndef EXAMPLESCENE_H
#define EXAMPLESCENE_H

#include <Scene.h>
#include <Image.h>
#include <list>

//este jogo é single cena e tal, mas normalmente a lógica não seria em uma cena só
class ExampleScene : public hf::Scene {
private:
    SDL_Texture* texture;
    int audio_clip;

    basic::Image* entity;

    //auto functions
    void begin() final override;
    void end() final override;
    void update() final override;
    void process_event(const SDL_Event& sdl_event) final override;
public:

};

#endif //GAMESCENE_H

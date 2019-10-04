#ifndef HF_AUDIOMANAGER_H
#define HF_AUDIOMANAGER_H

#include <string_view>
#include <vector>

#include <AL/al.h>
#include <AL/alc.h>
#include <alu.h>

namespace hf {
    class AudioManager {
    private:
        static AudioManager* instance;
        AudioManager() {}
        ~AudioManager() {}

        ALCdevice* al_device = nullptr;
        ALCcontext* al_context = nullptr;
        std::vector<ALuint> al_buffers;
        std::vector<ALuint> al_sources;
        int source_index = 0;

        void update();

    public:
        static AudioManager* get_instance ();

        void init(int source_count);
        int load_audio(std::string_view path);
        void play_audio(int audio_id, float pitch = 1.f, float panning = 0.f);

        friend class GameManager;
    };
}

#endif //HF_AUDIOMANAGER_H

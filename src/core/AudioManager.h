#ifndef HF_AUDIOMANAGER_H
#define HF_AUDIOMANAGER_H

#include <string_view>
#include <vector>

#include <SDL_mixer.h>

namespace hf {
    class AudioManager {
    private:
        static AudioManager* instance;
        AudioManager() {}
        ~AudioManager() {}

        int current_channel = -1;
        int channel_count = MIX_CHANNELS;
        std::vector<Mix_Chunk*> chunks;

    public:
        static AudioManager* get_instance ();

        void init(int channel_count);
        int load_audio(std::string_view path);
        void play_audio(int audio_id, float panning = 0.f);

        friend class GameManager;
    };
}

#endif //HF_AUDIOMANAGER_H

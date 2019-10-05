#include "AudioManager.h"

#include <exception>
#include <iostream>
#include <string>
#include <algorithm>

#include <SDL.h>
#include <cstdlib>

hf::AudioManager* hf::AudioManager::instance = nullptr;

hf::AudioManager* hf::AudioManager::get_instance () {
    if(instance == nullptr){
        instance = new hf::AudioManager();
    }
    return instance;
}

void hf::AudioManager::init(int channel_count) {
    Mix_Init(MIX_INIT_FLAC | MIX_INIT_MOD | MIX_INIT_MP3 | MIX_INIT_OGG | MIX_INIT_MID | MIX_INIT_OPUS);
    Mix_OpenAudio(22050, AUDIO_S16SYS, 2, 4096);
    this->channel_count = channel_count;
    Mix_AllocateChannels(channel_count);
}

int hf::AudioManager::load_audio(std::string_view path) {
    auto chunk = Mix_LoadWAV(path.data());
    if(chunk == NULL) {
        std::cout << "ERRO: " << Mix_GetError() << '\n';
        throw std::runtime_error("AudioManager: Could not open WAV file!");
    }
    else{
        std::cout << "SUCESSO!";
    }
    chunks.push_back(chunk);
    return (int)chunks.size() - 1;
}

void hf::AudioManager::play_audio(int audio_id, float panning) {
    if(audio_id < 0 || audio_id >= chunks.size()) {
        throw std::runtime_error("AudioManager: Tried to play an invalid audio clip!");
    }
    if(++current_channel >= channel_count){
        current_channel = 0;
    }
    int right = (int)(128.f - 128.f * std::clamp(panning, -1.f, 1.f));
    Mix_SetPanning(current_channel, 255 - right, right);
    Mix_PlayChannel(current_channel, chunks[audio_id], 0);
}
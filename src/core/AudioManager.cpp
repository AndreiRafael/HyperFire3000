#include "AudioManager.h"

#include <exception>
#include <iostream>
#include <string>
#include <algorithm>

#include <stb_vorbis.h>
#include <SDL.h>
#include <cstdlib>

hf::AudioManager* hf::AudioManager::instance = nullptr;

hf::AudioManager* hf::AudioManager::get_instance () {
    if(instance == nullptr){
        instance = new hf::AudioManager();
    }
    return instance;
}

void hf::AudioManager::update(){
    alcProcessContext(al_context);
}

void hf::AudioManager::init(int source_count){
    al_device = alcOpenDevice(NULL);
    if(!al_device){
        throw std::runtime_error("AudioManager: Device could not be opened!");
    }

    al_context = alcCreateContext(al_device, NULL);
    alcMakeContextCurrent(al_context);
    if(al_context == NULL) {
        throw std::runtime_error("AudioManager: Could not create openal context!");
    }

    ALfloat listenerOri[] = { 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f };
    alListener3f(AL_POSITION, 0, 0, 1.0f);
    alListener3f(AL_VELOCITY, 0, 0, 0);
    alListenerfv(AL_ORIENTATION, listenerOri);

    alGetError();
    al_sources.resize(source_count);
    alGenSources(source_count, &al_sources[0]);
    if(auto error = alGetError() != AL_NO_ERROR) {
        throw std::runtime_error("AudioManager: Could not generate openal source!");
    }

    auto source_startup = [] (int src) {
        alSourcef(src, AL_PITCH, 1);
        alSourcef(src, AL_GAIN, 1);
        alSource3f(src, AL_POSITION, 0, 0, 0);
        alSource3f(src, AL_VELOCITY, 0, 0, 0);
        alSourcei(src, AL_LOOPING, AL_FALSE);
    };
    std::for_each(al_sources.begin(), al_sources.end(), source_startup);
}

int hf::AudioManager::load_audio(std::string_view path) {
    alGetError();
    al_buffers.push_back(0);
    int index = al_buffers.size() - 1;
    alGenBuffers(1, &al_buffers[index]);
    if(auto error = alGetError() != AL_NO_ERROR) {
        throw std::runtime_error("AudioManager: Could not generate openal buffer!");
    }

    int len = 0;
    int channels;
    int sample_rate;
    short* output;
    // TODO: Support for file formats other than .ogg should be done here
    SDL_RWops* sdl_file = SDL_RWFromFile(path.data(), "rb");
    if(sdl_file == NULL) {
        throw std::runtime_error("AudioManager: Could not open .ogg file!");
    }

    unsigned char* mem = new unsigned char[SDL_RWsize(sdl_file)];
    SDL_RWread(sdl_file, mem, sizeof(unsigned char) * SDL_RWsize(sdl_file), 1);
    len = stb_vorbis_decode_memory(mem, sizeof(unsigned char) * SDL_RWsize(sdl_file), &channels, &sample_rate, &output);
    if(len == -1) {
        throw std::runtime_error("AudioManager: Could not decode .ogg file!");
    }

    alBufferData(al_buffers[index], AL_FORMAT_STEREO16, output, len * channels * sizeof(short), sample_rate);
    if(auto error = alGetError() != AL_NO_ERROR) {
        throw std::runtime_error("AudioManager: Could not attach openal buffer data!");
    }

    return index;
}

void hf::AudioManager::play_audio(int audio_id, float pitch, float panning) {
    auto src = al_sources[source_index++];
    if(source_index >= al_sources.size()){
        source_index = 0;
    }

    alSourceStop(src);

    alSourcei(src, AL_BUFFER, al_buffers[audio_id]);
    alSourcef(src, AL_PITCH, pitch);
    if(auto error = alGetError() != AL_NO_ERROR) {
        throw std::runtime_error("AudioManager: Could not attach openal source to buffer!");
    }

    alSourcePlay(src);
}
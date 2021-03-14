#include "include/audio.h"

internal bool
load_audio(audio_t* audio)
{
    /* Load the WAV */
    if (!SDL_LoadWAV("Assets/Dungeon.wav", &audio->wav_spec, &audio->wav_buffer, &audio->wav_length))
    {
        fprintf(stderr, "Could not open test.wav: %s\n", SDL_GetError());
        return false;
    }
    else
    {
        /* Do stuff with the WAV data, and then... */
        
        audio->device_id = SDL_OpenAudioDevice(0, 0, &audio->wav_spec, 0, 0);
        
        i32 success = SDL_QueueAudio(audio->device_id, audio->wav_buffer, audio->wav_length);
        
        if (success != 0)
        {
            printf("Failed to queue audio!\n");
            return false;
        }
        
        SDL_PauseAudioDevice(audio->device_id, 0);
    }
    
    return true;
}

internal void
free_audio(audio_t* audio)
{
    SDL_CloseAudioDevice(audio->device_id);
    SDL_FreeWAV(audio->wav_buffer);
    SDL_CloseAudio();
}
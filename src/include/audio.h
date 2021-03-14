/* date = March 13th 2021 6:41 pm */

#ifndef AUDIO_H
#define AUDIO_H

typedef struct AUDIO_STRUCT {
    SDL_AudioSpec wav_spec;
    u32 wav_length;
    u8 *wav_buffer;
    SDL_AudioDeviceID device_id;
} audio_t;

internal bool load_audio(audio_t* audio);
internal void free_audio(audio_t* audio);

#endif //AUDIO_H

#ifndef MP3_ENCODER_H
#define MP3_ENCODER_H

#include <string>

#include "Packet.h"
#include "FileWriter.h"
#include "PacketChainBase.h"

#include "lame.h"
#include "pthread.h"

struct wavHeader
{
    char chunk_id[4];
    int chunk_size;
    char format[4];
    char subchunk1_id[4];
    int subchunk1_size;
    short int audio_format;
    short int num_channels;
    int sample_rate;
    int byte_rate;
    short int block_align;
    short int bits_per_sample;
    char subchunk2_id[4];
    int subchunk2_size;
};

class MP3Encoder:
    public PacketChainBase
{
    public:
        MP3Encoder (std::string filePath, wavHeader header);
        ~MP3Encoder ();

        virtual void execute (std::shared_ptr<Packet> packet) override;
    private:
        lame_t _lame;
        wavHeader _header;
};


#endif

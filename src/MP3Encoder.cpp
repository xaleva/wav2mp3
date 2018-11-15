#include "../include/MP3Encoder.h"
#include "../include/ThreadPool.h"

#include <iostream>
#include <stdio.h>
#include <math.h>

MP3Encoder::MP3Encoder (std::string filePath, wavHeader header)
{
    _header = header;
    _lame = lame_init ();
    lame_set_in_samplerate (_lame, _header.sample_rate);
    lame_set_num_channels (_lame, _header.num_channels);
    lame_set_mode (_lame, _header.num_channels > 1 ? MPEG_mode::STEREO : MPEG_mode::MONO);
    lame_set_VBR (_lame, vbr_default);

    lame_set_quality (_lame, 3); // increase quality level
    lame_set_bWriteVbrTag (_lame, 0);

    lame_init_params (_lame);

    setNext (std::make_shared<FileWriter> (filePath));
}

MP3Encoder::~MP3Encoder ()
{
    lame_close (_lame);
}

void MP3Encoder::execute (std::shared_ptr<Packet> packet)
{
    auto& manager = MemoryManager<unsigned char>::getInstance ();
    packet->setOutputChunk (manager.allocateChunk (CHUNK_ELEMENT_COUNT));
    int write = 0;
    if (_header.num_channels > 1)
    {
        write = lame_encode_buffer_interleaved(_lame, packet->inputChunk ()->data (),
                    packet->inputChunk ()->getUsedSize (), packet->outputChunk ()->data (),
                    packet->outputChunk ()->getSize ());
    }
    else
    {
        write = lame_encode_buffer(_lame, packet->inputChunk ()->data (), NULL,
                packet->inputChunk ()->getUsedSize (), packet->outputChunk ()->data (),
                packet->outputChunk ()->getSize ());
    }
    packet->outputChunk ()->setUsedSize (write);
}

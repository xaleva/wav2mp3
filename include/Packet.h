#ifndef PACKET_H
#define PACKET_H

#include "MemoryManager.h"

#define CHUNK_ELEMENT_COUNT 8192

class Packet
{
    public:
        Packet (int packetId);
        Packet (const Packet&) = delete;
        ~Packet () = default;

        int getId () const;
        std::shared_ptr<MemoryChunk<short int>> inputChunk ();
        void setInputChunk (std::shared_ptr<MemoryChunk<short int>> chunk);
        std::shared_ptr<MemoryChunk<unsigned char>> outputChunk ();
        void setOutputChunk (std::shared_ptr<MemoryChunk<unsigned char>> chunk);
    private:
        int _packetId;
        std::shared_ptr<MemoryChunk<short int>> _inputChunk;
        std::shared_ptr<MemoryChunk<unsigned char>> _outputChunk;
};


#endif

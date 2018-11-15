#include "../include/Packet.h"

Packet::Packet (int packetId):
    _packetId (packetId)
{
}

int Packet::getId () const
{
    return _packetId;
}

std::shared_ptr<MemoryChunk<short int>> Packet::inputChunk ()
{
    return _inputChunk;
}

std::shared_ptr<MemoryChunk<unsigned char>> Packet::outputChunk ()
{
    return _outputChunk;
}

void Packet::setInputChunk (std::shared_ptr<MemoryChunk<short int>> chunk)
{
    _inputChunk = chunk;
}

void Packet::setOutputChunk (std::shared_ptr<MemoryChunk<unsigned char>> chunk)
{
    _outputChunk = chunk;
}

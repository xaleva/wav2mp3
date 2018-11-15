#ifndef FILE_CONVERTER_H
#define FILE_CONVERTER_H

#include <string>

#include "Packet.h"
#include "PacketChainBase.h"
#include "MP3Encoder.h"

class FileConverter:
    public PacketChainBase
{
    public:
        FileConverter (std::string& filePath);
        ~FileConverter ();

        virtual void operator() () override;
        virtual void execute (std::shared_ptr<Packet> packet) override {}
    private:
        void copy (unsigned char* from, short int* to, int size);
        void failureExit ();
    private:
        std::string _filePath;
        long _lastPacketId;
        wavHeader _header;
        FILE* _fl;
};


#endif

#ifndef FILE_WRITER_H
#define FILE_WRITER_H

#include <string>

#include "Packet.h"
#include "PacketChainBase.h"

class FileWriter:
    public PacketChainBase
{
    public:
        FileWriter (std::string& fileName);
        ~FileWriter ();

        virtual void execute (std::shared_ptr<Packet> packet) override;
    private:
        std::string _fileName;
        FILE *_mp3;
};


#endif

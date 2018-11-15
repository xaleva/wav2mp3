#include "../include/FileConverter.h"
#include "../include/ThreadPool.h"
#include "../include/SystemOutput.h"
#include "../include/MemoryManager.h"

#include <stdio.h>
#include <math.h>

FileConverter::FileConverter (std::string& filePath):
    _filePath (filePath),
    _lastPacketId (0)
{
    _fl = fopen (_filePath.data (), "r");
}

FileConverter::~FileConverter ()
{
    if (_fl != NULL)
    {
        fclose (_fl);
    }
}

void FileConverter::operator() ()
{
    if (_fl != NULL)
    {
        // Reading wav header information
        fread (&_header, 1, sizeof (_header), _fl);

        // calculating the size
        fseek (_fl, 0, SEEK_END);
        long size = ftell (_fl) - sizeof (_header);
        rewind (_fl);
        fseek (_fl, sizeof (_header), SEEK_CUR);

        if (_header.bits_per_sample == 0)
        {
            failureExit ();
            _fl = NULL;
            return;
        }

        setNext (std::make_shared<MP3Encoder> (_filePath, _header));

        short int sampleSize = _header.num_channels * _header.bits_per_sample / 8;
        int chunkSize = CHUNK_ELEMENT_COUNT * sampleSize;
        long chunkCount = ceil (size / chunkSize);

        auto& memoryManager = MemoryManager<short int>::getInstance ();
        auto tmpChunk = MemoryManager<unsigned char>::getInstance ().allocateChunk (chunkSize);

        for (int i = 0;i < chunkCount;++i)
        {
            auto packet = std::make_shared<Packet> (++_lastPacketId);
            packet->setInputChunk (memoryManager.allocateChunk (chunkSize));

            if (sampleSize < 2)
            {
                int readSize = fread (tmpChunk->data (), 1, CHUNK_ELEMENT_COUNT, _fl);
                copy (tmpChunk->data (), packet->inputChunk ()->data (), readSize);
                packet->inputChunk ()->setUsedSize (readSize);
            }
            else
            {
                int readSize = fread (packet->inputChunk ()->data (), sampleSize, CHUNK_ELEMENT_COUNT, _fl);
                packet->inputChunk ()->setUsedSize (readSize);
            }
            forward (packet);
        }
    }

}

void FileConverter::copy (unsigned char* from, short int* to, int size)
{
    for (int j = 0; j < size; j++)
    {
        to [j] = (short int)(from [j] - 0x80) << 8;
    }
}

void FileConverter::failureExit ()
{
    SystemOutput::getInstance ().output (_filePath + ": cannot encode! bits_per_sample is 0!");
}

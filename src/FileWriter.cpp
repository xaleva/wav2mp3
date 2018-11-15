#include "../include/FileWriter.h"
#include "../include/SystemOutput.h"

#include <iostream>
#include <stdio.h>
#include <math.h>

FileWriter::FileWriter (std::string& fileName):
    _fileName (fileName)
{
    auto pos = _fileName.rfind (".");
    std::string mp3FileName = _fileName.substr (0, pos + 1);
    mp3FileName.append ("mp3");
    _mp3 = fopen (mp3FileName.data (), "wb");
}

FileWriter::~FileWriter ()
{
    SystemOutput::getInstance ().output (_fileName + ": finished");
    if (_mp3 != NULL)
    {
        fclose (_mp3);
    }
}

void FileWriter::execute (std::shared_ptr<Packet> packet)
{
    if (_mp3 != NULL)
    {
        fwrite (packet->outputChunk ()->data (), packet->outputChunk ()->getUsedSize (), 1, _mp3);
    }
}

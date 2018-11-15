#include "include/FileConverter.h"
#include "include/ThreadPool.h"
#include "include/SystemOutput.h"

#include <dirent.h>

#ifdef WIN32
#define SEPARATOR "\\"
#else
#define SEPARATOR "/"
#endif

int main (int argc, char* argv [])
{
    ThreadPool::getInstance ().init ();

    if (argc < 2)
    {
        std::cerr << "Usage: " << argv [0] << " <path> " << std::endl;
        return 0;
    }
    auto path = argv [1];

    SystemOutput::getInstance ().output ("Start searching wav files...");
    DIR* dirFile = opendir (path);
    if ( dirFile )
    {
       struct dirent* hFile;
       while (( hFile = readdir( dirFile )) != NULL )
       {
          if (!strcmp (hFile->d_name, ".")) continue;
          if ( !strcmp( hFile->d_name, ".." )) continue;

          if (strstr (hFile->d_name, ".wav"))
          {
              std::string fileName = std::string (path) + std::string (SEPARATOR) + hFile->d_name;
              auto converter = std::make_shared<FileConverter> (fileName);
              ThreadPool::getInstance ().runTask (converter);
          }
       }
       closedir (dirFile);
    }

    return ThreadPool::getInstance ().exec ();
}

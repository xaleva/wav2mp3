# wav2mp3
Simple wav to mp3 encoder

## Dependencies:
*  c++14 
*  cmake (for building project)
*  pthread
*  lame

## Build (For Linux)
mkdir build
cd build
cmake ../
make

## Build (For Windows)
In order to cross compile it for Windows under the linux you will need mingw installed
mkdir build
cd build
cmake ../win
make

## Features:
* Supports mono and stereo types of wav files
* Asynchronous encoding using all cores on the machine. 

## Known issues:
* No managing of errors and exceptions
* Not all wav files are supported

## Description

wav2mp3 utility was build using lame library for encoding and pthreads for multithreading. 

It has ThreadPool singleton with some simple thread allocation algorithm : each time when there is a shortage of threads it allocates a new one or use one of the already allocated but inactive thread. The algorithm can be easily changed according to your needs. 
In order to tackle memory consumption the MemoryManager singleton has been created. It again acts as a memory pool, and at the same time gives you an opportunity to allocate chunks of different types. In the future by using both ThreadPool and MemoryManager you can create a better approach and different strategies for leveraging between memory consumption and cpu usage. 
The process of encoding is being split to 3 steps: reading file, encoding and writing. Each step is being run in distinct thread so i/o operations will not block each other and encoding process. All steps are connected by using Chain of Responsibility pattern , which was a little bit modified by using threads. The positive aspect of such an architecture is that in the future you can add another layer (for instance write packets to a different storage or convert in different formats also). It is worth to mention that although each layer is acting independently in different thread data synchronization is still managed. 

I have tested it on my linux machine. I managed to cross-compile it for Windows but have not tested it.

In order to build lame for Windows under the Linux you will need mingw package installed. Build it with:




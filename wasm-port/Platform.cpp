/*
    Copyright 2016-2021 Arisotura

    This file is part of melonDS.

    melonDS is free software: you can redistribute it and/or modify it under
    the terms of the GNU General Public License as published by the Free
    Software Foundation, either version 3 of the License, or (at your option)
    any later version.

    melonDS is distributed in the hope that it will be useful, but WITHOUT ANY
    WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
    FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with melonDS. If not, see http://www.gnu.org/licenses/.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Platform.h>


namespace Platform
{




void Init(int argc, char** argv)
{
}

void DeInit()
{
    
}


void StopEmu()
{

}


// Under Emscripten, fopen()/fread()/fwrite() transparently operate against
// whatever virtual filesystem is mounted (MEMFS by default, plus IDBFS at
// /data, mounted from JS - see index.html). So a real fopen() wrapper here
// is enough to make things like the DLDI-emulated SD card actually work,
// as long as JS has written/mounted the backing file before melonDS tries
// to open it (i.e. before Module._loadROM() is called).
FILE* OpenFile(const char* path, const char* mode, bool mustexist)
{
    FILE* f = fopen(path, mode);
    if (!f)
        printf("OpenFile: failed to open '%s' (mode '%s')\n", path, mode);
    return f;
}

FILE* OpenLocalFile(const char* path, const char* mode)
{
    FILE* f = fopen(path, mode);
    if (!f)
        printf("OpenLocalFile: failed to open '%s' (mode '%s')\n", path, mode);
    return f;
}


Thread* Thread_Create(std::function<void()> func)
{
    return NULL;
}

void Thread_Free(Thread* thread)
{

}

void Thread_Wait(Thread* thread)
{

}


Semaphore* Semaphore_Create()
{
    return nullptr;
}

void Semaphore_Free(Semaphore* sema)
{

}

void Semaphore_Reset(Semaphore* sema)
{

}

void Semaphore_Wait(Semaphore* sema)
{

}

void Semaphore_Post(Semaphore* sema, int count)
{
 
}

Mutex* Mutex_Create()
{
    return nullptr;
}

void Mutex_Free(Mutex* mutex)
{

}

void Mutex_Lock(Mutex* mutex)
{

}

void Mutex_Unlock(Mutex* mutex)
{

}

bool Mutex_TryLock(Mutex* mutex)
{
    return true;
}



bool MP_Init()
{
    return false;
}

void MP_DeInit()
{

}

int MP_SendPacket(u8* data, int len)
{
   return 0;
}

int MP_RecvPacket(u8* data, bool block)
{
    return 0;
}



bool LAN_Init()
{
    return false;
}

void LAN_DeInit()
{

}

int LAN_SendPacket(u8* data, int len)
{
     return 0;
}

int LAN_RecvPacket(u8* data)
{
    return 0;
}



}

#include "util.h"
#include "PacketBuffer.h"

PacketBuffer::PacketBuffer()
{
}

PacketBuffer::~PacketBuffer()
{
}

const char* PacketBuffer::GetBuffer()
{
    char* buffer = new char[MAX_BUF_SIZE + 1];
    ZeroMemory(buffer, sizeof(buffer));

    int bufferSize = 0;

    // header
    bufferSize += WriteBuffer(buffer + bufferSize, len, sizeof(short));
    bufferSize += WriteBuffer(buffer + bufferSize, no, sizeof(short));

    // body
    bufferSize += WriteBuffer(buffer + bufferSize, message.c_str(), message.length());

    return buffer;
}

template<typename T>
int PacketBuffer::WriteBuffer(char* buffer, T data, int size)
{
    CopyMemory(buffer, &data, size);

    return size;
}

int PacketBuffer::ReadBuffer()
{

    return 0;
}
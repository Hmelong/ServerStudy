#pragma once

#include <winsock2.h>
#include <stdio.h>
#include <stdlib.h>
#include <tchar.h>
#include <string>
#include <iostream>
#include <array>
#include "primitive_types.h"
#include "log.h"

#pragma comment(lib, "ws2_32.lib")

#define BUFSIZE 1024
#define MAX_BUF_SIZE 65535

struct msg
{
    msg()
        : no(0)
    {}

    int32 no;
    std::string message;

    bool ParseBuffer(const char* pBuffer)
    {
        if (!pBuffer)
            return false;

        uint16 packetLength = 0;
        CopyMemory(&packetLength, pBuffer, sizeof(uint16));

        if (packetLength == 0 || packetLength > MAX_BUF_SIZE)
            return false;

        CopyMemory(&no, pBuffer + sizeof(packetLength), sizeof(no));

        const char* pStartMessage = pBuffer + sizeof(packetLength) + sizeof(no);
        const char* pEndMessage = pBuffer + packetLength;
        message.assign(pStartMessage, pEndMessage);

        return true;
    }

    void WriteBuffer(char* pBuffer)
    {
        if (!pBuffer)
        {
            LOG_ERROR("msg pBuffer is nullptr.");
            return;
        }

        uint16 packetLength = GetPacketSize();
        if (packetLength > MAX_BUF_SIZE)
        {
            LOG_ERROR("packetLength({}) is bigger than MAX_BUF_SIZE({})",
                packetLength, MAX_BUF_SIZE);
            return;
        }

        CopyMemory(pBuffer, &packetLength, sizeof(packetLength));

        CopyMemory(pBuffer + sizeof(packetLength), &no, sizeof(no));

        CopyMemory(pBuffer + sizeof(packetLength) + sizeof(no), message.data(), message.size());
    }

    uint16 GetPacketSize() const { return GetHeaderSize() + GetBodySize(); }
    uint16 GetHeaderSize() const { return sizeof(uint16); }
    uint16 GetBodySize() const
    {
        return sizeof(no) + static_cast<uint32>(message.length());
    }
};

enum PACKET_NO
{
    NO_0 = 0,
    NO_1,

    MAX_NO
};

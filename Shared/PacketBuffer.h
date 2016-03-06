#pragma once

class PacketBuffer
{
public:
    PacketBuffer();
    ~PacketBuffer();

public:
    /// header
    uint16 len;

    /// body
    int32 no;
    std::string message;

    uint16 GetPacketSize() const { return GetHeaderSize() + GetBodySize(); }
    uint16 GetHeaderSize() const { return sizeof(uint16); }
    uint16 GetBodySize() const
    {
        return sizeof(no) + static_cast<uint32>(message.length());
    }

    void WriteBuffer(char* pBuffer);
    bool ParseBuffer(const char* pBuffer);
};

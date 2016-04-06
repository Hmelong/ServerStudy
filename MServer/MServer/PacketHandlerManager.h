#pragma once

#include <google/protobuf/message_lite.h>
#include "PacketBuffer.h"

template<typename TSession, typename TProtoBuf>
class PacketHandler;

// Packet 정의
#define DECLARE_HANDLER(TSession, TProtoBuf)\
template <>\
class PacketHandler<TSession, TProtoBuf>\
{\
public:\
    bool operator()(TSession& session, const TProtoBuf& packet);\
};

// Static 함수 초기화 선언
#define IMPLEMENT_INITIALIZE(TSession)\
TSession::TPacketHandlerManager SessionBase<TSession>::_s_packet_handler_manager;\
static void Initialize(TSession::TPacketHandlerManager& packetHandlerManager)

// 패킷 함수 핸들러에 등록
#define REGISTER_HANDLER(TProtoBuf)\
packetHandlerManager.Register<TProtoBuf>(TProtoBuf::PROTOCOL_NUMBER)

// 패킷 핸들러 구현
#define IMPLEMENT_HANDLER(TSession, TProtoBuf)\
bool PacketHandler<TSession, TProtoBuf>::operator()(TSession& session, const TProtoBuf& packet)

template<typename TSession>
class PacketFunctorBase
{
public:
    explicit PacketFunctorBase() {};
    virtual ~PacketFunctorBase() {};

    virtual bool Handle(TSession& session, PacketBuffer& packetBuffer) const = 0;
};

template<typename TSession, typename TProtoBuf>
class PacketFunctor : PacketFunctorBase<TSession>
{
public:
    explicit PacketFunctor() {};
    virtual ~PacketFunctor() {};

    virtual bool Handle(TSession& session, PacketBuffer& packetBuffer) const override
    {
        TProtoBuf protobuf;
        if (!protobuf.ParseFromArray(
            packetBuffer.GetPayloadBuffer(), packetBuffer.GetPayloadBufferSize()))
        {
            LOG_ERROR("PacketParse error.");
            return false;
        }

        packetBuffer.ConsumePacket();

        return PacketHandler<TSession, TProtoBuf>()(session, protobuf);
    }
};

template<typename TSession>
class PacketHandlerManager
{
public:
    explicit PacketHandlerManager() 
    {
        ClearPacketFunctors();
        Initialize(*this);
    };

    ~PacketHandlerManager() 
    {
        ClearPacketFunctors();
    };

    template<class TProtoBuf>
    bool Register(uint16 protocolNumber)
    {
        if (protocolNumber > packetFunctors.size())
        {
            LOG_ERROR("ProtocolNumber is out of range. protocolNumber: {}",
                protocolNumber);
            return false;
        }

        if (packetFunctors[protocolNumber])
        {
            LOG_ERROR("Already registered protocol. protocolNumber: {}",
                protocolNumber);
            return false;
        }

        PacketFunctorBase<TSession>* pPacketFunctor =
            reinterpret_cast<PacketFunctorBase<TSession>*>(
            new PacketFunctor<TSession, TProtoBuf>());

        packetFunctors[protocolNumber].reset(pPacketFunctor);
        return true;
    }

    bool Handle(TSession& session, PacketBuffer& packetBuffer)
    {
        const uint16 packetNo = packetBuffer.GetPacketNo();

        PacketFunctorBase<TSession>* pFnPacketHandler = packetFunctors[packetNo].get();
        if (!pFnPacketHandler)
        {
            LOG_ERROR("PacketHandler is not exist. packetNo: {}", packetNo);
            return false;
        }

        return pFnPacketHandler->Handle(session, packetBuffer);
    }

    void ClearPacketFunctors()
    {
        for (auto& functor : packetFunctors)
        {
            functor.reset();
        }
    }

private:
    std::array<std::unique_ptr<PacketFunctorBase<TSession>>, UINT16_MAX> packetFunctors;
};

#pragma once

class PacketBuffer;

template<typename TSession, typename TPacketBuffer>
class PacketHandler;

// Packet 정의
#define DECLARE_HANDLER(TSession, TPacketBuffer)\
class TPacketBuffer : public PacketBuffer {};\
template <>\
class PacketHandler<TSession, TPacketBuffer>\
{\
public:\
    bool operator()(TSession& session, const TPacketBuffer& packet);\
};

// Static 함수 초기화 선언
#define IMPLEMENT_INITIALIZE(TSession)\
TSession::TPacketHandlerManager SessionBase<TSession>::_s_packet_handler_manager;\
static void Initialize(TSession::TPacketHandlerManager& packetHandlerManager)

// 패킷 함수 핸들러에 등록
#define REGISTER_HANDLER(TPacketBuffer, packetNumber)\
packetHandlerManager.Register<TPacketBuffer>(packetNumber)

// 패킷 핸들러 구현
#define IMPLEMENT_HANDLER(TSession, TPacketBuffer)\
bool PacketHandler<TSession, TPacketBuffer>::operator()(TSession& session, const TPacketBuffer& packet)

template<typename TSession>
class PacketFunctorBase
{
public:
    explicit PacketFunctorBase() {};
    virtual ~PacketFunctorBase() {};

    virtual bool Handle(TSession& session, const PacketBuffer& packet) const = 0;
};

template<typename TSession, typename TPacketBuffer>
class PacketFunctor : PacketFunctorBase<TSession>
{
public:
    explicit PacketFunctor() {};
    virtual ~PacketFunctor() {};

    virtual bool Handle(TSession& session, const PacketBuffer& packet) const override
    {
        TPacketBuffer packetBuffer;
        packetBuffer.CopyFrom(packet);

        return PacketHandler<TSession, TPacketBuffer>()(session, packetBuffer);
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

    template<class TPacketBuffer>
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
            new PacketFunctor<TSession, TPacketBuffer>());

        packetFunctors[protocolNumber].reset(pPacketFunctor);
        return true;
    }

    bool Handle(TSession& session, const PacketBuffer& packet)
    {
        PacketFunctorBase<TSession>* pFnPacketHandler = packetFunctors[packet.no].get();
        if (!pFnPacketHandler)
        {
            LOG_ERROR("PacketHandler is not exist. no: {}", packet.no);
            return false;
        }

        return pFnPacketHandler->Handle(session, packet);
    }

    void ClearPacketFunctors()
    {
        for (auto& functor : packetFunctors)
        {
            functor.reset();
        }
    }

private:
    std::array<std::unique_ptr<PacketFunctorBase<TSession>>, MAX_NO> packetFunctors;
};

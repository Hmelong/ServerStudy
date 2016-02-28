#pragma once

class PacketBUffer;

class PacketHandler : public Singleton<PacketHandler>
{
public:
	PacketHandler();
	~PacketHandler();

	bool HandlePacket(Session* session, PacketBuffer packet);


	static bool PACKET_NO_0(Session* session, PacketBuffer packet);
	static bool PACKET_NO_1(Session* session, PacketBuffer packet);
};


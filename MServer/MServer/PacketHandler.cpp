#pragma once

#include "util.h"
#include "Singleton.h"

#include "Session.h"
#include "PacketBuffer.h"

#include "PacketHandler.h"

PacketHandler::PacketHandler()
{
}

PacketHandler::~PacketHandler()
{
}


bool PacketHandler::HandlePacket(Session* session, PacketBuffer packet)
{	
	if (session == nullptr)
		return false;

	switch (packet.no)
	{
	case NO_0:
		return PACKET_NO_0(session, packet);
		break;
	case NO_1:
		return PACKET_NO_0(session, packet);
		break;
	}

	return true;
}

bool PacketHandler::PACKET_NO_0(Session* session, PacketBuffer packet)
{

	return true;
}

bool PacketHandler::PACKET_NO_1(Session* session, PacketBuffer packet)
{

	return true;
}
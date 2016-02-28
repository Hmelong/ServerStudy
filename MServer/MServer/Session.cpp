#pragma once;

#include "util.h"

#include "PacketBuffer.h"
#include "Session.h"


Session::Session()
{
}

Session::~Session()
{
}

bool Session::SendPacket(PacketBuffer packet)
{
	const char* buffer = packet.GetBuffer();
	int bufferSize = packet.HeaderSIze() + packet.len;

	int retval = send(client_sock, buffer, bufferSize, 0);
	if (SOCKET_ERROR == retval)
		return false;

	return true;
}
#include "util.h"

#include "PacketBuffer.h"
#include "Session.h"

Session::Session()
{
}

Session::~Session()
{
}

bool Session::InitSession(SOCKET listen_sock)
{
	int32 addrlen = sizeof(clientAddr);

	client_sock = accept(listen_sock, (SOCKADDR*)&clientAddr, &addrlen);
	if (client_sock == INVALID_SOCKET)
	{
		LOG_ERROR("accept()");
		return false;
	}

	LOG_INFO("client_sock success.");

	return true;
}

void Session::CloseSession()
{
	closesocket(client_sock);
}

bool Session::SendPacket(PacketBuffer packet)
{
	std::array<char, MAX_BUF_SIZE + 1> buf = { 0 };

	// send
	packet.WriteBuffer(buf.data());

	int retval = send(client_sock, buf.data(), packet.GetPacketSize(), 0);
	if (SOCKET_ERROR == retval)
		return false;

    return true;
}
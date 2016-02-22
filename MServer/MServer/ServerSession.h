#pragma once

class ServerSession
{
public:
	ServerSession();
	~ServerSession();

	bool InitSession();

	void CloseSession();

protected:
	bool InitWSA();
	bool InitSocket();
	bool Bind();
	bool Listen();

public:
	SOCKET listen_sock;
};


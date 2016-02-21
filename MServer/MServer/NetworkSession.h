#pragma once

class NetworkSession
{
public:
	NetworkSession();
	~NetworkSession();

	bool InitSession();

	void CloseSession();

protected:
	bool InitWSA();
	bool InitSocket();
	bool Bind();
	bool Listen();

private:
	WSADATA WSA;

public:
	SOCKET listen_sock;
};


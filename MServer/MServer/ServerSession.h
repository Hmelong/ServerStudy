#pragma once

class ServerSession
{
public:
    ServerSession();
    ~ServerSession();

    bool InitSession();
    void CloseSession();

    SOCKET GetSocket() const;

protected:
    bool InitWSA();
    bool InitSocket();
    bool Bind();
    bool Listen();

private:
    SOCKET listen_sock;
};

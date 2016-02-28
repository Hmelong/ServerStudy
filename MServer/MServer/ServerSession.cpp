#include "util.h"
#include "ServerSession.h"


ServerSession::ServerSession()
{
}

ServerSession::~ServerSession()
{
}

bool ServerSession::InitSession()
{
    // socket
    if (InitSocket() == false)
        return false;

    // bind
    if (Bind() == false)
        return false;

    // listen
    if (Listen() == false)
        return false;

    return true;
}

void ServerSession::CloseSession()
{
    // cloasesocket();
    closesocket(listen_sock);
}

bool ServerSession::InitSocket()
{
    listen_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_sock == INVALID_SOCKET)
    {
        LOG_ERROR("Initalize listen_sock");
        return false;
    }

    LOG_INFO("socket success.");
    return true;
}

bool ServerSession::Bind()
{
    SOCKADDR_IN serverAddr;
    ZeroMemory(&serverAddr, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(9000);
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);

    int retval = bind(listen_sock, (SOCKADDR*)&serverAddr, sizeof(serverAddr));
    if (retval == SOCKET_ERROR)
    {
        LOG_ERROR("bind() error: {}", retval);
        return false;
    }

    LOG_INFO("bind success.");
    return true;
}

bool ServerSession::Listen()
{
    int retval = listen(listen_sock, SOMAXCONN);
    if (retval == SOCKET_ERROR)
    {
        LOG_ERROR("listen() error: {}", retval);
        return false;
    }

    LOG_INFO("listen success.");
    return true;
}
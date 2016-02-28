#include "util.h"
#include "NetworkEngine.h"


NetworkEngine::~NetworkEngine()
{
}

bool NetworkEngine::InitWSA()
{
    if (WSAStartup(MAKEWORD(2, 2), &WSA) != 0)
    {
        LOG_ERROR("Initialize WSASock");
        return false;
    }

    LOG_INFO("WSASock success.");
    return true;
}

void NetworkEngine::ReleaseWSA()
{
    // 윈속 종료
    WSACleanup();
}
#pragma once

#include "Singleton.h"

class NetworkEngine : public Singleton<NetworkEngine>
{
public:
    ~NetworkEngine();

    bool InitWSA();
    void ReleaseWSA();

private:
    WSADATA WSA;
};

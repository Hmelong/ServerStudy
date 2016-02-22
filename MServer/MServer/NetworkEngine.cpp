#pragma once

#include "util.h"
#include "NetworkEngine.h"


NetworkEngine::~NetworkEngine()
{
}

bool NetworkEngine::InitWSA()
{
	if (WSAStartup(MAKEWORD(2, 2), &WSA) != 0)
	{
		printf("[ERROR] Initalize WSASock");
		return false;
	}
	printf("WSASock success.\n");

	return true;
}

void NetworkEngine::ReleaseWSA()
{
	// 윈속 종료
	WSACleanup();
}
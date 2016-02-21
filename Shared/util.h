
#pragma once

#include <winsock2.h>
#include <stdio.h>
#include <stdlib.h>
#include <tchar.h>
#include <string>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

#define BUFSIZE 1024
#define MAX_BUF_SIZE 65535

struct msg
{
	int no;
	string message;
	
	short size()
	{
		short size = sizeof(int) + message.length();
		return size;
	}

	void addPacket()
	{
		no++;
		message += std::to_string(no);
	}
};
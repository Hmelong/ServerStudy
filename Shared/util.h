
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
};


enum PACKET_NO
{
	NO_0	= 0,
	NO_1,

	MAX_NO
};


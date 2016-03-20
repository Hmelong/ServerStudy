#pragma once

#include <winsock2.h>
#include <stdio.h>
#include <stdlib.h>
#include <tchar.h>
#include <string>
#include <iostream>
#include <array>
#include <memory>
#include "primitive_types.h"
#include "log.h"

#pragma comment(lib, "ws2_32.lib")


#define BUFSIZE 1024
#define MAX_BUF_SIZE 65535


enum PACKET_NO : uint16
{
    NO_0 = 0,
    NO_1,

    MAX_NO
};

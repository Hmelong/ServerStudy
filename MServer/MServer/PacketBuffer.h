#pragma once

class PacketBuffer
{
public:
	PacketBuffer();
	~PacketBuffer();

	const char* GetBuffer();

public:
	/// header
	unsigned short len;
	unsigned short no;

	int HeaderSIze() { return sizeof(short) + sizeof(short); }
	
	template<typename T>
	int WriteBuffer(char* buffer, T data, int size);
	int ReadBuffer();

	// body
	string message;
};


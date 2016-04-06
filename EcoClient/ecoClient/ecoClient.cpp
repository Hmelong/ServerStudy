// ecoClient.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "util.h"

#include "PacketBuffer.h"
#include "p_client_server.h"
#include "p_server_client.h"

int _tmain(int argc, _TCHAR* argv[])
{
    int32 retval = 0;

    // 윈속 초기화
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
    {
        return -1;
    }

    // socket()
    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
    if (INVALID_SOCKET == sock)
    {
        LOG_ERROR("socket()");
    }

    // connect()
    SOCKADDR_IN serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(9000);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    retval = connect(sock, (SOCKADDR*)&serverAddr, sizeof(serverAddr));
    if (SOCKET_ERROR == retval)
    {
        LOG_ERROR("connect error: {}", retval);
    }

    // 데이터 통신
    PacketBuffer sendBuffer;
    PacketBuffer recvBuffer;

    while (1)
    {
        // 데이터 입력
        LOG_INFO("[보낼 데이터]");
        
        std::array<char, MAX_BUF_SIZE> inputData = {0};
        if (NULL == fgets(inputData.data(), inputData.size(), stdin))
        {
            continue;
        }

        // enter 제거
        uint32 len = static_cast<uint32>(strlen(inputData.data()) - 1);
        inputData.at(len) = '\0';

        PC2S_Chat out;
        out.set_message(inputData.data());

        // 데이터 보내기
        if (!sendBuffer.SetPacket(out))
        {
            LOG_ERROR("Fail to set packet to packetBuffer.");
            continue;
        }

        retval = send(sock, sendBuffer.GetPacketBuffer(), sendBuffer.GetPacketSize(), 0);
        if (SOCKET_ERROR == retval)
        {
            LOG_ERROR("send() error: {}", retval);
            continue;
        }
        sendBuffer.ConsumePacket();

        LOG_INFO("[TCP Client] {} 바이트를 보냈습니다.", retval);

        while (1)
        {
            // 데이터 받기
            std::array<char, MAX_BUF_SIZE> recvData = {0};

            retval = recv(sock, recvData.data(), recvData.size(), 0);
            if (SOCKET_ERROR == retval || retval == 0)
            {
                LOG_ERROR("recv error: {}", retval);
                break;
            }

            if (!recvBuffer.AppendBuffer(recvData.data(), retval))
            {
                LOG_ERROR("Fail to append buffer.");
                break;
            }

            if (!recvBuffer.IsAbleToGetPacket())
                continue;

            uint16 packetNo = recvBuffer.GetPacketNo();

            PS2C_Chat packet;
            if (!packet.ParseFromArray(
                recvBuffer.GetPayloadBuffer(), recvBuffer.GetPayloadBufferSize()))
            {
                LOG_ERROR("Fail to packet parse.");
                break;
            }

            recvBuffer.ConsumePacket();

            LOG_INFO("[TCP Client] {} 바이트를 받았습니다.", retval);
            //LOG_INFO("[받은 데이터] No.{}-{}",  packetNo, packet.message());


            // 데이터 보내기
            PC2S_Chat out;
            out.set_message(packet.message());

            if (!sendBuffer.SetPacket(out))
            {
                LOG_ERROR("Fail to set packet.");
                break;
            }

            retval = send(sock, sendBuffer.GetPacketBuffer(), sendBuffer.GetPacketSize(), 0);
            if (SOCKET_ERROR == retval)
            {
                LOG_ERROR("send() error: {}", retval);
                break;
            }

            sendBuffer.ConsumePacket();

            LOG_INFO("[TCP Client] {} 바이트를 보냈습니다.", retval);
        }
    }

    // closesocket();
    closesocket(sock);

    // 윈속 종료
    WSACleanup();

    return 0;
}


#include <iostream>
#include <WS2tcpip.h>
#include <MSWSock.h>
#include "Protocol.h"
#include "Over.h"

#pragma comment(lib, "WS2_32.lib")
#pragma comment(lib, "MSWSock.lib")

int main()
{
	HANDLE h_iocp;

	WSADATA WSAData;
	WSAStartup(MAKEWORD(2, 2), &WSAData);
	SOCKET s_server = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	SOCKADDR_IN server_addr;
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(PORT_NUM);
	server_addr.sin_addr.S_un.S_addr = INADDR_ANY;
	bind(s_server, reinterpret_cast<sockaddr*>(&server_addr), sizeof(server_addr));
	listen(s_server, SOMAXCONN);
	SOCKADDR_IN cl_addr;
	int addr_size = sizeof(cl_addr);
	int client_id = 0;

	h_iocp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, 0, 0, 0);
	CreateIoCompletionPort(reinterpret_cast<HANDLE>(s_server), h_iocp, INT_MAX, 0);
	SOCKET c_socket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	OVER a_over(ACCEPT);
	AcceptEx(s_server, c_socket, a_over.m_sendbuf, 0, addr_size + 16, addr_size + 16, 0, &a_over.m_wsaover);

	while (true)
	{
		DWORD num_bytes;
		ULONG_PTR key;
		WSAOVERLAPPED* wsaover = nullptr;
		BOOL ret = GetQueuedCompletionStatus(h_iocp, &num_bytes, &key, &wsaover, INFINITE);

		if (ret == false)
		{
			std::cout << "ERROR" << std::endl;
			while (true);
		}

		OVER* over = reinterpret_cast<OVER*>(wsaover);
		switch (over->type)
		{
		case ACCEPT:
			std::cout << "ACCEPT 패킷입니다 : " << over->type << std::endl;
			break;
		case SEND:
			std::cout << "SEND 패킷입니다 : " << over->type << std::endl;
			break;
		case RECV:
			std::cout << "RECV 패킷입니다 : " << over->type << std::endl;
			break;
		default:
			std::cout << "정의되지 않은 타입의 패킷입니다 : " << over->type << std::endl;
		}
	}
}
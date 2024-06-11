#include <iostream>
#include <WS2tcpip.h>
#include <MSWSock.h>
#include "Protocol.h"
#include "Over.h"
#include <array>

#pragma comment(lib, "WS2_32.lib")
#pragma comment(lib, "MSWSock.lib")

std::atomic_int client_id = 0;

struct Client_Data {
	SOCKET sock;
	float x, y, z;
};

std::array<Client_Data, MAX_USER + MAX_NPC> clients;

void worker_thread(HANDLE h_iocp, SOCKET s_server)
{
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
		{
			//std::cout << "ACCEPT 패킷입니다 : " << over->type << std::endl;
			SOCKET c_socket = over->sock;
			//std::cout << "SOCKET : " << c_socket << ", id: " << client_id << std::endl;
			
			clients[client_id].sock = c_socket;
			
			// overlapped 구조체 재사용
			CreateIoCompletionPort(reinterpret_cast<HANDLE>(c_socket), h_iocp, client_id++, 0);
			c_socket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, nullptr, 0, WSA_FLAG_OVERLAPPED);
			over->sock = c_socket;
			ZeroMemory(&over->m_wsaover, sizeof(over->m_wsaover));
			int addr_size = sizeof(SOCKADDR_IN);
			AcceptEx(s_server, c_socket, over->m_sendbuf, 0, addr_size + 16, addr_size + 16, nullptr, &over->m_wsaover);
			break;
		}
			//WSASend(c_socket, &over->m_wsabuf, 1, 0, 0, &over->m_wsaover, 0);
			//std::cout << "전송 완료" << std::endl;
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


	h_iocp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, 0, 0, 0);
	CreateIoCompletionPort(reinterpret_cast<HANDLE>(s_server), h_iocp, INT_MAX, 0);
	SOCKET c_socket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	OVER a_over(ACCEPT, c_socket);
	AcceptEx(s_server, c_socket, a_over.m_sendbuf, 0, addr_size + 16, addr_size + 16, 0, &a_over.m_wsaover);

	worker_thread(h_iocp, s_server);
}
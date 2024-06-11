// Fill out your copyright notice in the Description page of Project Settings.


#include "IOCP.h"
#include <WS2tcpip.h>
#pragma comment (lib, "WS2_32.LIB")

void CALLBACK recv_callback(DWORD er, DWORD recv_size, LPWSAOVERLAPPED pwsaover, DWORD sendflag);

// Sets default values for this component's properties
UIOCP::UIOCP()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UIOCP::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UIOCP::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

bool UIOCP::ConnectIOCP()
{
	UE_LOG(LogTemp, Log, TEXT("ConnectIOCP"));

	WSADATA WSAData;
	WSAStartup(MAKEWORD(2, 2), &WSAData);

	server_s = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, nullptr, 0, WSA_FLAG_OVERLAPPED);
	SOCKADDR_IN server_a;
	server_a.sin_family = AF_INET;
	server_a.sin_port = htons(4000);

	char ip[] = "127.0.0.1";
	inet_pton(AF_INET, ip, &server_a.sin_addr);
	int res = WSAConnect(server_s, reinterpret_cast<sockaddr*>(&server_a), sizeof(server_a), nullptr, nullptr, NULL, NULL);
	if (0 != res)
	{
		UE_LOG(LogTemp, Log, TEXT("Connect Failed"));
		return false;
	}
	RecvIOCP();
	return true;
}

void UIOCP::DisconnectIOCP()
{
	UE_LOG(LogTemp, Log, TEXT("DisconnectIOCP"));
	closesocket(server_s);
}

void UIOCP::RecvIOCP()
{
	ZeroMemory(buf, BUFSIZE);
	wsabuf[0].buf = buf;
	wsabuf[0].len = BUFSIZE;
	DWORD recv_flag = 0;

	ZeroMemory(&wsaover, sizeof(wsaover));
	wsaover.inst = this;
	int res = WSARecv(server_s, wsabuf, 1, nullptr, &recv_flag,reinterpret_cast<WSAOVERLAPPED*>(&wsaover), recv_callback);
	if (0 != res)
	{
		int err_no = WSAGetLastError();
		if (WSA_IO_PENDING != err_no)
			UE_LOG(LogTemp, Log, TEXT("Error!!! RecvIOCP"));
	}
}

void CALLBACK recv_callback(DWORD er, DWORD recv_size, LPWSAOVERLAPPED pwsaover, DWORD sendflag)
{
	if (0 != er)
	{
		if (0 == recv_size) {
			return;
		}
		UE_LOG(LogTemp, Log, TEXT("Error!!! recv_callback"));
	}

	UE_LOG(LogTemp, Log, TEXT("recv_callback Come!!!!!!!"));
	Fclient_over* over = reinterpret_cast<Fclient_over*>(pwsaover);
	over->inst->RecvIOCP();
}

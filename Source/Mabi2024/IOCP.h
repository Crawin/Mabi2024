// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include <WS2tcpip.h>
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "IOCP.generated.h"

constexpr int BUFSIZE = 256;

USTRUCT()
struct Fclient_over {
	GENERATED_BODY()
public:
	WSAOVERLAPPED wsaover;
	UIOCP* inst;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MABI2024_API UIOCP : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UIOCP();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	SOCKET server_s;
	WSABUF wsabuf[1];
	char buf[BUFSIZE];
	Fclient_over wsaover;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "IOCP")
	bool ConnectIOCP();

	UFUNCTION(BlueprintCallable, Category = "IOCP")
	void DisconnectIOCP();

	void RecvIOCP();
};

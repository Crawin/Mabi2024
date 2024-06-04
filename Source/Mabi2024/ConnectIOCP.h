// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ConnectIOCP.generated.h"

/**
 * 
 */
UCLASS()
class MABI2024_API UConnectIOCP : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
	UFUNCTION(BlueprintCallable, Category = "IOCP")
	static void ConnectIOCP();
};

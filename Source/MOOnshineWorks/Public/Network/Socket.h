// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Networking.h"
#include "Messages.h"
#include "Socket.generated.h"

/**
 * 
 */
UCLASS()
class MOONSHINEWORKS_API ASocket : public AActor
{
	GENERATED_BODY()

public:
	ASocket(const class FObjectInitializer& PCIP);

	FSocket* ListenerSocket;
	FSocket* ConnectionSocket;
	FIPv4Endpoint RemoteAddressForConnection;

	bool StartUDPReceiver(
		const FString& YourChosenSocketName,
		const FString& TheIP,
		const int32 ThePort
		);

	FSocket* CreateUDPConnectionListener(
		const FString& YourChosenSocketName,
		const FString& TheIP,
		const int32 ThePort,
		const int32 ReceiveBufferSize = 2 * 1024 * 1024
		);

	FIPv4Endpoint Destination;

	void start(FString name, FString ip, int32 port);

	//Timer functions, could be threads
	void UDPConnectionMaker();
	void UDPConnectionListener(); 	//can thread this eventually
	void UDPSocketListener();		//can thread this eventually

	bool ParseMessage(TArray<uint8> msg);
	
	void SendString(FString msg);
	void SendMEvent(int id, int x, int y, int powerLevel);
	
	//Format String IP4 to number array
	bool FormatIP4ToNumber(const FString& TheIP, uint8(&Out)[4]);

	//Rama's StringFromBinaryArray
	FString StringFromBinaryArray(const TArray<uint8>& BinaryArray);
	
};

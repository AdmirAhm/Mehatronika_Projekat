#pragma once
#include "ws_base.h"
#include <iostream>
#include <fo/FileOperations.h>
#include <cnt/StringBuilder.h>
#include <mu/mu.h>
#include <future>
#include "..\global.h"
class server : public ws_base
{
protected:

	void handleClient()
	{
		sockaddr clientAdr;

		SOCKET clientSocket = acceptConnection(clientAdr);
		if (clientSocket == INVALID_SOCKET)
			return;


		try
		{
			while (true)
			{
				td::String message = receiveMsg(clientSocket);
				if (message.getAt(0) == 'a') setMessage1(message);
				else setMessage(message);
				if (close_server) {
					closesocket(clientSocket);
					return;
				}
			}
		}
		catch (...)
		{
			closesocket(clientSocket);
			return;

		}
		
	}

	void shutdownClient(SOCKET client, int how)
	{
		int iRes = shutdown(client, how);
		if (iRes != 0)
		{
			throw std::runtime_error("WinSock error");
		}
	}



	SOCKET acceptConnection(sockaddr& client)
	{
		std::memset(&client, 0, sizeof client);
		int clientAdrSize = sizeof client;
		return accept(_socket, &client, &clientAdrSize);
	}

public:

	void initAndListenForConnections(td::String port, protocol protocol)
	{
		try
		{

			WSADATA wsa = initWSA();

			addrinfo* result = nullptr, hints;
			std::memset(&hints, 0, sizeof(addrinfo));
			hints.ai_family = AF_INET;
			hints.ai_socktype = protocol == protocol::TCP ? SOCK_STREAM : SOCK_DGRAM;
			hints.ai_protocol = protocol == protocol::TCP ? IPPROTO_TCP : IPPROTO_UDP;
			hints.ai_flags = AI_PASSIVE;
			getAdrInfo(nullptr, port.c_str(), &hints);
		
			initSocket();

			bindSocketToAddress();

			listenSocket(SOMAXCONN);


			while (true)
			{

				handleClient();
				if (close_server) {
					return;
				}
			}
		}
		catch (...)
		{
			//std::cout << "WinSock error code: " << WSAGetLastError();
			freeaddrinfo(_adrInfo);
			closesocket(_socket);
			WSACleanup();
		}
	}

	server()
	{
	}

};
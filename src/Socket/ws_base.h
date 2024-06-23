#pragma once

#include <stdexcept>
#include <stdio.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <td/String.h>
#include <iostream>
class ws_base
{
public:
	enum class protocol { TCP, UDP };
protected:
	static constexpr size_t buffSize = 1024 * 64;
	static constexpr WORD version = MAKEWORD(2, 2);

	SOCKET _socket = INVALID_SOCKET;
	addrinfo* _adrInfo = nullptr;
protected:

	void putLengthToStringFront(td::String& string, td::UINT4 frontBuffSize)
	{
		auto len = string.length();
		auto buffIndex = frontBuffSize - 1;
		while (len > 0)
		{
			auto digit = len % 10;
			string.setAt(buffIndex--, '0' + digit);
			len /= 10;
		}
	}

	WSADATA initWSA()
	{
		WSADATA wsa;
		int iRes = WSAStartup(version, &wsa);
		if (iRes != 0)
		{
			throw std::runtime_error("WinSock error");
		}
		return wsa;
	}
	void getAdrInfo(const char* adr, const char* port, addrinfo* hints)
	{
		int iRes = getaddrinfo(adr, port, hints, &_adrInfo);
		if (iRes != 0)
		{
			throw std::runtime_error("WinSock error");
		}
	}

	void initSocket()
	{
		_socket = socket(_adrInfo->ai_family, _adrInfo->ai_socktype, _adrInfo->ai_protocol);
		if (_socket == INVALID_SOCKET)
		{
			throw std::runtime_error("WinSock error");
		}
	}

	void bindSocketToAddress()
	{
		int iRes = bind(_socket, _adrInfo->ai_addr, (int)_adrInfo->ai_addrlen);
		if (iRes != 0)
		{
			throw std::runtime_error("WinSock error");
		}
	}

	bool listenSocket(int maxBuff)
	{
		int iRes = listen(_socket, maxBuff);
		if (iRes != 0)
		{
			throw std::runtime_error("WinSock error");
		}
		return true;
	}



	void shutdownConnection(int how)
	{
		int iRes = shutdown(_socket, how);
		if (iRes != 0)
		{
			throw std::runtime_error("WinSock error");
		}
	}


	void sendMsg(SOCKET to, td::String msg, int flags = 0)
	{

		int iRes = send(to, msg.c_str(), msg.length(), flags);
		if (iRes == 0)
		{ 
			throw std::runtime_error("WinSock error");
		}
	}

	void sendMsg(SOCKET to, td::String msg, int& bytesSent, int flags = 0 )
	{

		int iRes = send(to, msg.c_str(), msg.length(), flags);
		if (iRes == 0)
		{
			bytesSent = 0;
			throw std::runtime_error("WinSock error");
		}
		bytesSent = iRes;
	}



	td::String receiveMsg(SOCKET& from, int flags = 0)
	{

		td::String msg;
		constexpr auto buffFreePlace = buffSize - 1;
		char* locForMsg = new char[buffSize];
		locForMsg[buffFreePlace] = '\0';
		while (true) 
		{
			int iRes = recv(from, locForMsg, buffFreePlace, flags);

			if (iRes > 0 && iRes == buffFreePlace)
			{
				td::String part(locForMsg, iRes);
				msg += part;
			}
			else if (iRes > 0 && iRes < buffFreePlace)
			{
				td::String part(locForMsg, iRes);
				msg += part;
				delete[] locForMsg;
				return msg;
			}
			else if (iRes == 0)
			{
				//connection is closed, recv done, we are done receiving data
				delete[] locForMsg;
				return msg;
			}
			else
			{
				delete[] locForMsg;

				throw std::runtime_error("WinSock error");
			}
		}
		return msg;
	}


public:


};












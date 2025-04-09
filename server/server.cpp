#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <WinSock2.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <fstream>
#include <string>
#include <ctime>
#include <sstream>
#include <cstdlib>
#include <vector>
#include <cassert>
#include <algorithm>
using namespace std;

#pragma comment(lib, "ws2_32.lib")

#include "rsa.h"

char buf[6] = { 'O', 'K' };
int ID = 0;

typedef struct TDATA
{
	SOCKET sockC;
	int id;
} TDATA;

void PrintHex(char* input, int len)
{
	for (int i = 0; i < len; i++)
	{
		printf("% 02x  ", input[i] & 0xff);
		if (i % 11 == 10)
			printf("\n");
	}
	printf("\n");
}

DWORD WINAPI TFun(LPVOID lpParameter)
{
	TDATA* p = (TDATA*)lpParameter;

	char recvBuf[256];
	char writeBuf[49];
	memset(recvBuf, 0, sizeof(recvBuf));
	memset(writeBuf, 0, sizeof(writeBuf));
	//接收数据
	recv(p->sockC, recvBuf, sizeof(recvBuf), 0);
	//发送数据
	for (int ij = 0; ij < 4; ij++)
	{
		buf[ij + 2] = char(p->id >> (ij * 8) & 0xff);
	}
	int iSend = send(p->sockC, buf, sizeof(buf), 0);
	if (iSend == SOCKET_ERROR)
	{
		printf("send failed");
		// break;
	}
	closesocket(p->sockC);

	RsaDe(recvBuf, writeBuf, 256);
	printf("密钥长度: %d\n密钥: \n", (int)(writeBuf[0] & 0xff));
	PrintHex(&writeBuf[1],32);
	printf("\n初始向量: \n");
	PrintHex(&writeBuf[33], 16);
	string filename = to_string(p->id) + ".key";
	ofstream output(filename.c_str(), ios::out | ios::binary);
	output.write(writeBuf, 49);
	output.close();
	printf("\nID:%d 解密完成\n", p->id);
	return 0L;
}

int main()
{
	WSADATA wsaData;
	int port = 5099;

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		printf("Failed to load Winsock");
		return 0;
	}

	//创建用于监听的套接字
	SOCKET sockSrv = socket(AF_INET, SOCK_STREAM, 0);

	SOCKADDR_IN addrSrv;
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(port); //1024以上的端口号
	addrSrv.sin_addr.S_un.S_addr = htonl(INADDR_ANY);

	int retVal = bind(sockSrv, (LPSOCKADDR)&addrSrv, sizeof(SOCKADDR_IN));
	if (retVal == SOCKET_ERROR)
	{
		printf("Failed bind:%d\n", WSAGetLastError());
		return 0;
	}

	if (listen(sockSrv, 10) == SOCKET_ERROR)
	{
		printf("Listen failed:%d", WSAGetLastError());
		return 0;
	}

	SOCKADDR_IN addrClient;
	int len = sizeof(SOCKADDR);
	TDATA data[1000];
	int tnum = 0;

	//等待客户请求到来
	while (1)
	{
		SOCKET sockConn = accept(sockSrv, (SOCKADDR*)&addrClient, &len);
		data[tnum].sockC = sockConn;
		data[tnum].id = ID;
		if (sockConn == SOCKET_ERROR)
		{
			printf("Accept failed:%d", WSAGetLastError());
			//break;
		}
		printf("来自IP:[%s]的连接\n", inet_ntoa(addrClient.sin_addr));
		ID += 1;
		//创建一个线程
		HANDLE thread = CreateThread(NULL, 0, TFun, &data[tnum], 0, NULL);
		//关闭线程句柄
		CloseHandle(thread);
		tnum = (tnum + 1) % 1000;
	}
	closesocket(sockSrv);
	WSACleanup();
	system("pause");
	return 0;
}

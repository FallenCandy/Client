// Client.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <WinSock2.h>
#include <ws2tcpip.h>

using namespace std;
#pragma comment(lib,"ws2_32.lib")

bool MySend(int fd)
{
	cout << "请输入要发送的内容" << endl;
	char  sendbuf[20];
	cin >> sendbuf;
	if (sendbuf == "\0")
	{
		return true;
	}
	send(fd, sendbuf, sizeof(sendbuf), 0);
	return false;
}
int main()
{
	int socketfd;
	WSAData wsdata;
	if (WSAStartup(MAKEWORD(2, 2), &wsdata) < 0)
	{
		cout << "加载socket库失败！" << endl;
		return 0;
	}
	socketfd = socket(AF_INET,SOCK_STREAM,0);
	if (socketfd == SOCKET_ERROR)
	{
		cout << "socket创建失败！" << endl;
		return 0;
	}
	
	sockaddr_in claddr;
	claddr.sin_family = AF_INET;
	claddr.sin_port = htons(8090);
	const char * ipAddr = "127.0.0.1";
	inet_pton(AF_INET,ipAddr, &claddr.sin_addr);
	if (connect(socketfd, (sockaddr*)&claddr, sizeof(claddr)) == SOCKET_ERROR)
	{
		cout << "客户端连接服务端失败！错误码："<<GetLastError() << endl;
		return 0;
	}
	if (MySend(socketfd))
	{
		return 0;
	}
	for (;;)
	{
		if (MySend(socketfd))
		{
			break;
		}
		while (1)
		{
			char  buffer[1500] = { 0 };
			int size = recv(socketfd, buffer, 1499, 0);
			if (size > 0)
			{
				cout << "收到的数据内容为：" << buffer << endl;
				break;
			}
			else if (size == 0)
			{
				cout << "数据读取完成或客户端下线" << endl;
				break;
			}
			else
			{
				cout << "数据读取错误" << endl;
				break;
			}
		}
	}
	

	closesocket(socketfd);
	WSACleanup();

	system("pause");
	return 0;
}


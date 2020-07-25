#pragma once

#include <vector>

#include<WinSock2.h>  
#include<iostream>  

using namespace std;

class UDPSender
{
public:
	UDPSender()
	{
		m_sock = -1;
	}
	~UDPSender() {}
	bool SetRecvAddr(const char* addr, int port)
	{
		int iResult;
		WSADATA wsaData;
		iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);

		if (iResult != 0) {
			return -1;
		}

		SOCKET sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
		if (sock == INVALID_SOCKET) {
			WSACleanup();
			return -1;
		}
		/*int sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
		if (sock < 0)
		{
			cout << "socket init error!" << endl;
			return false;
		}*/
		sockaddr_in addrSrv;
		addrSrv.sin_addr.s_addr = inet_addr(addr);
		addrSrv.sin_family = AF_INET;
		addrSrv.sin_port = htons(port);
		if ((addrSrv.sin_addr.s_addr >> 24) == 0xff)//if addr end with 255 then broadcast
		{
			GetLocalIP();
			isBroadCast = true;
			string local = findlocalIp(addr);
			if (local.compare("") == 0)
			{
				cout << "find local ip error!" << endl;
				return false;
			}
			sockaddr_in addrSrvLocal;
			addrSrvLocal.sin_addr.s_addr = inet_addr(local.c_str());
			addrSrvLocal.sin_family = AF_INET;
			addrSrvLocal.sin_port = 0;//unuse
			if (bind(sock, (sockaddr*)&addrSrvLocal, sizeof(addrSrvLocal)) != 0)
			{
				cout << "bind local ip error!" << endl;
				return false;
			}
			int opval = 1;
			addrSrv.sin_addr.s_addr = htonl(INADDR_BROADCAST);
			setsockopt(sock, SOL_SOCKET, SO_BROADCAST, (char*)&opval, sizeof(opval));
		}
		m_sock = sock;
		m_sockaddr = addrSrv;
		return true;
	}
	void Send(char* buf, int bufsize)
	{
		sendto(m_sock, buf, bufsize, 0, (struct sockaddr*)&m_sockaddr, sizeof(sockaddr));
	}
private:
	void GetLocalIP()
	{
		FILE* fstream = NULL;
		char buf[128];
		memset(buf, 0, sizeof(buf));
		//string cmd = "/sbin/ifconfig |grep \"inet addr\"|awk \'{print $2}\'|awk -F \':\' \'{print $2}\'";// cmd: /sbin/ifconfig |greo "inet addr"|awk '{print $2}'|awk -F ':' '{print $2}'
		//string cmd = "ipconfig |grep \"inet addr\"|awk \'{print $2}\'|awk -F \':\' \'{print $2}\'";// cmd: /sbin/ifconfig |greo "inet addr"|awk '{print $2}'|awk -F ':' '{print $2}'
//string cmd = "for /f \"usebackq tokens = 2 delims = :\" %i in (`ipconfig^|findstr IPv4`) do echo %i";
string cmd = "ipconfig|findstr IPv4";
/*


*/
		if (NULL == (fstream = _popen(cmd.c_str(), "r")))
		{
			cout << "ifconfig exec error" << endl;
			return;
		}
		Sleep(100);
		while (NULL != fgets(buf, sizeof(buf), fstream))
		{
			char* chr = NULL;
			chr = strchr(buf, ':');
			chr+=2;// 还有个空格
			int len = strlen(chr);
			memcpy(buf, chr, len);
			buf[len - 1] = '\0';//erase \n
			LocalIP.push_back(buf);
		}
		return;
	}
	string findlocalIp(string BCast)
	{
		if (LocalIP.size() == 0)
		{
			cout << "local ip empty" << endl;
			return "";
		}
		int pos = BCast.find_first_of(".");
		if (pos < 0)
		{
			cout << "distination IP address format error!" << endl;
			return "";
		}
		BCast[pos] = 0;//compare first byte
		vector<string>::iterator it = LocalIP.begin();
		for (; it != LocalIP.end(); it++)
		{
			string local = *it;
			pos = local.find_first_of(".");
			if (pos < 0)
			{
				cout << "local IP address format error" << endl;
				return "";
			}
			local[pos] = 0;
			if (strcmp(BCast.c_str(), local.c_str()) == 0)
			{
				return *it;
			}
		}
		return "";
	}
	public :
	vector<string> LocalIP;
	bool isBroadCast;
	int m_sock;
	sockaddr_in m_sockaddr;
};
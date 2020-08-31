#include "ClientSocketManager.h"
#include <Ws2tcpip.h>
#include <Sensapi.h>
#include "NetTastHandle.h"
#include "NetProtocol.h"
#include "..\Utils\ClientLog.h"
#include "..\Utils\ZlibUtil.h"
#include"protocolstream.h"
#pragma comment(lib, "Sensapi.lib")
#pragma comment(lib, "ws2_32.lib")
using namespace Net;

#define MAX_PACKAGE_SIZE 10*1024*1024

ClientSocketManager::ClientSocketManager()
{
	m_socket = INVALID_SOCKET;
	m_connected = false;
	m_port = 0;
	m_strServer = "";

	m_nHeartbeatInterval = 10;  //先默认设置为10秒发次心跳包
	m_nLastDataTime = (long)time(NULL);
	m_nHeartbeatSeq = 0;

	handle_obj = new NetTastHandle;
}

bool ClientSocketManager::Init()
{
	m_bStop = false;

	if (!m_spSendThread)
		m_spSendThread.reset(new std::thread(std::bind(&ClientSocketManager::SendThreadProc, this)));

	if (!m_spRecvThread)
		m_spRecvThread.reset(new std::thread(std::bind(&ClientSocketManager::RecvThreadProc, this)));

	return true;
}

void ClientSocketManager::Uninit()
{
	//先设置标志位，将线程分离出来,再释放条件变量让线程彻底执行完,再释放线程指针。
	m_bStop = true;

	if (m_spSendThread)
	{
		m_spSendThread->detach();
	}

	if (m_spRecvThread)
	{
		m_spRecvThread->detach();
	}

	m_cvSendBuf.notify_one();
	m_cvRecvBuf.notify_one();

	//如果线程退掉了，这里指针会自动为空，所以再次reset()时先判断一下
	if (m_spSendThread)
	{
		m_spSendThread.reset();
	}
	if (m_spRecvThread)
	{
		m_spRecvThread.reset();
	}
}

bool ClientSocketManager::Connect(int timeout /*= 3*/)
{
	this->Close();
	m_socket = ::socket(AF_INET, SOCK_STREAM, 0);
	if (m_socket == INVALID_SOCKET)
	{
		return false;
	}

	/*int setsockopt(int sock, int level, int optname, const void *optval, socklen_t optlen);
	    参数：
		//sock：将要被设置或者获取选项的套接字。
		level：选项所在的协议层。
		optname：需要访问的选项名。
		optval：对于getsockopt()，指向返回选项值的缓冲。对于setsockopt()，指向包含新选项值的缓冲。
		optlen：对于getsockopt()，作为入口参数时，选项值的最大长度。作为出口参数时，选项值的实际长度。对于setsockopt()，现选项的长度。
	*/
	long tmSend = 3 * 1000L;
	long tmRecv = 3 * 1000L;
	long noDelay = 1;
	setsockopt(m_socket, IPPROTO_TCP, TCP_NODELAY, (LPSTR)&noDelay, sizeof(long));//不使用Nagle算法(包拼到一起)
	setsockopt(m_socket, SOL_SOCKET, SO_SNDTIMEO, (LPSTR)&tmSend, sizeof(long));////发送时限
	setsockopt(m_socket, SOL_SOCKET, SO_RCVTIMEO, (LPSTR)&tmRecv, sizeof(long));////接收时限

	//ioctlsocket控制套接口的模式  FIONBIO:允许或禁止套接口s的非阻塞模式
	unsigned long on = 1;
	if (::ioctlsocket(m_socket, FIONBIO, &on) == SOCKET_ERROR)
	{
		return false;
	}


	//sockaddr_in解决了sockaddr的缺陷，把port和addr 分开储存在两个变量中
	struct sockaddr_in addrSrv = { 0 };
	struct hostent* pHostent = NULL;
	unsigned int addr = 0;


	int add_ret = inet_pton(AF_INET, m_strServer.c_str(), (void *)&addrSrv.sin_addr.s_addr);
	
	if(add_ret == INADDR_NONE)
	{
		//如果是无效的ip地址
//		pHostent = ::gethostbyname(m_strServer.c_str());
		if (!pHostent)
		{
			return false;
		}
		else
		{
			addrSrv.sin_addr.s_addr = *((unsigned long*)pHostent->h_addr);
		}
	}

	addrSrv.sin_family = AF_INET;//地址族
	addrSrv.sin_port = htons((u_short)m_port);//端口号

	//为了能够随意控制connect的超时时间，可以使用select。大致的过程就是先将socket设置成非阻塞，使用select去轮询套接口，再根据套接口去判断连接状态
	//程序先把socket设置成非阻塞，connect在非阻塞模式下会立刻返回，如果没有其他错误，返回值等于0。当connect不能立刻建立连接时，会返回一个EINPROGRESS，表示连接正在建立的过程中，这时我们可以使用select去轮询套接口，而select的轮询超时时间可以根据自己的需要去设置，最主要的是轮询的集合一定要是读和写的集合，即select的第二和第三个参数要赋值，待select返回就可以去判断返回值来确定connect的进程状态了。

	//这里设置成阻塞 一定会直接返回
	int ret = ::connect(m_socket, (struct sockaddr*)&addrSrv, sizeof(addrSrv));
	if (ret == 0)
	{
		m_connected = true;
		return true;
	}
	else if (ret == SOCKET_ERROR && WSAGetLastError() != WSAEWOULDBLOCK)
	{
		return false;
	}


	//用select检测状态
	fd_set writeset; //socket的集合
	FD_ZERO(&writeset);//集合清空
	FD_SET(m_socket, &writeset); //把socket添加到一个fd_set类型的集合中
	struct timeval tv = { timeout, 0 };
	//select函数是一个轮循函数，循环询问文件节点，可设置超时时间，超时时间到了就跳过代码继续往下执行。

		//超时返回0
		//失败返回 - 1
		//成功返回大于0的整数，这个整数表示就绪描述符的数目
	if (::select(m_socket + 1, NULL, &writeset, NULL, &tv) != 1)
	{
		//LOG_ERROR("Could not connect to server:%s, port:%d.", m_strServer.c_str(), m_nPort);
		return false;
	}
	m_connected = true;
	return true;
}

void ClientSocketManager::Close()
{
	if (m_socket == INVALID_SOCKET)
	{
		return;
	}

	::shutdown(m_socket, SD_BOTH);
	::closesocket(m_socket);
	m_socket = INVALID_SOCKET;
	m_connected = false;
}

bool ClientSocketManager::IsClosed()
{
	return !m_connected;
}

void ClientSocketManager::SetIpAndPort(std::string ip, short port)
{
	this->m_strServer = ip;
	this->m_port = port;
}

bool ClientSocketManager::Send()
{
	//如果未连接则重连，重连也失败则返回FALSE
	//TODO: 在发送数据的过程中重连没什么意义，因为与服务的Session已经无效了，换个地方重连
	if (IsClosed() && !Connect())
	{
	//LOG_ERROR("connect server:%s:%d error.", m_strServer.c_str(), m_nPort);
	   return false;
	}

	int nSentBytes = 0;
	int nRet = 0;
	while (true)
	{
		//若无错误发生，send()返回所发送数据的总数（请注意这个数字可能小于len中所规定的大小）。否则的话，返回SOCKET_ERROR错误，应用程序可通过WSAGetLastError()获取相应错误代码。

		//GYF:心跳包不断发送，这这里如果发送不过去，认为是客户端断开了连接
		//这个时候可以主线程用定时器每隔几秒访问这个是否断开变量，来切换客户端的
		//状态，而Qt中socket可以直接信号槽消息模式。
		nRet = ::send(m_socket, m_strSendBuf.c_str(), m_strSendBuf.length(), 0);
		if (nRet == SOCKET_ERROR)
		{
			if (::WSAGetLastError() == WSAEWOULDBLOCK)
			{
				break;
			}
			else
			{
				//LOG_ERROR("Send data error, disconnect server:%s, port:%d.", m_strServer.c_str(), m_nPort);
				Close();
				return false;
			}
		}
		else if(nRet < 1)
		{
			//LOG_ERROR("Send data error, disconnect server:%s, port:%d.", m_strServer.c_str(), m_nPort);
			Close();
			return false;
		}

		m_strSendBuf.erase(0, nRet);
		if (m_strSendBuf.empty())
		{
			break;
		}
		::Sleep(1);
	}

	{
		//记录最近的一次发包时间
		std::lock_guard<std::mutex> guard(m_mutexLastDataTime);
		m_nLastDataTime = (long)time(NULL);
	}
	return true;
}

void ClientSocketManager::Send(const std::string& strBuffer)
{
	//主线程执行
	//size_t nDestLength;
	std::string strDestBuf;
	if (!ZlibUtil::CompressBuf(strBuffer, strDestBuf))
	{
		LOG_ERROR("Compress error.");
		return;
	}

	int32_t length = (int32_t)strBuffer.length();
	msg header;
	header.compressflag = 1;
	header.originsize = length;
	header.compresssize = (int32_t)strDestBuf.length();

	std::lock_guard<std::mutex> guard(m_mtSendBuf);
	m_strSendBuf.append((const char*)&header, sizeof(header));
	m_strSendBuf.append(strDestBuf);
	m_cvSendBuf.notify_one();
}

bool ClientSocketManager::Recv()
{
	int nRet = 0;
	char buff[10 * 1024];
	while (true)
	{

		nRet = ::recv(m_socket, buff, 10 * 1024, 0);
		if (nRet == SOCKET_ERROR)				//һ�����ִ�������̹ر�Socket
		{
			if (::WSAGetLastError() == WSAEWOULDBLOCK)
				break;
			else
			{
				LOG_ERROR("Recv data error, errorNO=%d.", ::WSAGetLastError());
				//Close();
				return false;
			}
		}
		else if (nRet < 1)
		{
			LOG_ERROR("Recv data error, errorNO=%d.", ::WSAGetLastError());
			//Close();
			return false;
		}
		m_strRecvBuf.append(buff, nRet);
		::Sleep(1);
	}
	{
		std::lock_guard<std::mutex> guard(m_mutexLastDataTime);
		m_nLastDataTime = (long)time(NULL);
	}
	return true;
}

void ClientSocketManager::SendThreadProc()
{
	//LOG_INFO("Recv data thread start...");
	while (!m_bStop)
	{
		std::unique_lock<std::mutex> guard(m_mtSendBuf);
		while (m_strSendBuf.empty())
		{
			if (m_bStop)
			{
				return;
			}

			//字符为空就卡住线程 等待信息
			m_cvSendBuf.wait(guard);
		}

		if (!Send())
		{	
		}
	}
	//LOG_INFO("Recv data thread finish...");
}

void ClientSocketManager::RecvThreadProc()
{
	//LOG_INFO("Recv data thread start...");

	int nRet;
	//上网方式 
	DWORD   dwFlags;
	BOOL    bAlive;
	while (!m_bStop)
	{
		//检测到数据则收数据
		nRet = CheckReceivedData();
		//出错
		if (nRet == -1)
		{
			//m_pRecvMsgThread->NotifyNetError();
		}
		//无数据
		else if (nRet == 0)
		{
			bAlive = ::IsNetworkAlive(&dwFlags);		//是否在线      
			if (!bAlive && ::GetLastError() == 0)
			{
				//网络已经断开
				//m_pRecvMsgThread->NotifyNetError();
				//LOG_ERROR("net error, exit recv and send thread...");
				Uninit();
				break;
			}

			//在无数据的条件下判断心跳包的发送条件，直接放到while外面可能会造成性能损耗
			//判断心条包发送条件
			long nLastDataTime = 0;

			{
				std::lock_guard<std::mutex> guard(m_mutexLastDataTime);
				nLastDataTime = m_nLastDataTime;
			}

			if (m_nHeartbeatInterval > 0)
			{
				//time(NULL)获取系统时间 单位秒，年数从1900年开始
				if (time(NULL) - nLastDataTime >= m_nHeartbeatInterval)
				{
					//发送间隔大于设置的间隔时间10s 发送一次心跳包
					SendHeartbeatPackage();
				}
			}
		}
		else if (nRet == 1) //有数据
		{
			if (!Recv())
			{
				//m_pRecvMsgThread->NotifyNetError();
				continue;
			}
			DecodePackages();
		}// end if
	}// end while-loop
	//LOG_INFO("Recv data thread finish...");
}

int ClientSocketManager::CheckReceivedData()
{
	fd_set readset;
	FD_ZERO(&readset);
	FD_SET(m_socket, &readset);

	fd_set exceptionset;
	FD_ZERO(&exceptionset);
	FD_SET(m_socket, &exceptionset);

	timeval timeout;
	timeout.tv_sec = 0;
	timeout.tv_usec = 500;

	long nRet = ::select(m_socket + 1, &readset, NULL, &exceptionset, &timeout);
	if (nRet >= 1)
	{
		if (FD_ISSET(m_socket, &exceptionset))
			return -1;

		if (FD_ISSET(m_socket, &readset))
			return 1;
	}
	else if (nRet == SOCKET_ERROR)
	{
		return -1;
	}
	return 0;
}

bool ClientSocketManager::DecodePackages()
{
	while (true)
	{
		//包长度不够,继续缓冲
		if (m_strRecvBuf.length() <= sizeof(msg))
		{
			break;
		}

		msg header;
		memcpy_s(&header, sizeof(msg), m_strRecvBuf.data(), sizeof(msg));
		//数据压缩过
		if (header.compressflag == PACKAGE_COMPRESSED)
		{
			//防止包头定义的数据是一些错乱的数据，这里最大限制每个包大小为10M
			if (header.compresssize >= MAX_PACKAGE_SIZE || header.compresssize <= 0 ||
				header.originsize >= MAX_PACKAGE_SIZE || header.originsize <= 0)
			{
				LOG_ERROR("Recv a illegal package, compresssize: %d, originsize=%d.", header.compresssize, header.originsize);
				m_strRecvBuf.clear();
				return false;
			}

			//接收缓冲区不够一个整包大小（包头+包体），继续缓存
			if (m_strRecvBuf.length() < sizeof(msg) + header.compresssize)
			{
				break;
			}

			//去除包头信息
			m_strRecvBuf.erase(0, sizeof(msg));
			//拿到包体
			std::string strBody;
			strBody.append(m_strRecvBuf.c_str(), header.compresssize);
			//去除包体信息
			m_strRecvBuf.erase(0, header.compresssize);

			//解压
			std::string strUncompressBody;
			if (!ZlibUtil::UncompressBuf(strBody, strUncompressBody, header.originsize))
			{
				LOG_ERROR("uncompress buf error, compresssize: %d, originsize: %d", header.compresssize, header.originsize);
				m_strRecvBuf.clear();
				return false;
			}
			handle_obj->AddPacketVec(strBody);
		}
		//数据未压缩过
		else
		{
			//防止包头定义的数据是一些错乱的数据，这里最大限制每个包大小为10M
			if (header.originsize >= MAX_PACKAGE_SIZE || header.originsize <= 0)
			{
				LOG_ERROR("Recv a illegal package, originsize=%d.", header.originsize);
				m_strRecvBuf.clear();
				return false;
			}

			//接收缓冲区不够一个整包大小（包头+包体）
			if (m_strRecvBuf.length() < sizeof(msg) + header.originsize)
			{
				break;
			}
			//去除包头信息
			m_strRecvBuf.erase(0, sizeof(msg));
			//拿到包体
			std::string strBody;
			strBody.append(m_strRecvBuf.c_str(), header.originsize);
			//去除包体信息
			m_strRecvBuf.erase(0, header.originsize);
			handle_obj->AddPacketVec(strBody);
		}
	}
	return true;
}

void ClientSocketManager::Join()
{
	if (m_spSendThread && m_spSendThread->joinable())
	{
		m_spSendThread->join();
	}
	if (m_spRecvThread && m_spRecvThread->joinable())
	{
		m_spRecvThread->join();
	}
}

ClientSocketManager::~ClientSocketManager()
{

}

void Net::ClientSocketManager::SendHeartbeatPackage()
{
	std::string outbuf;
	net::BinaryStreamWriter writeStream(&outbuf);
	writeStream.WriteInt32(msg_type_heartbeat);
	writeStream.WriteInt32(m_nHeartbeatSeq);
	std::string dummy;
	writeStream.WriteString(dummy);
	writeStream.Flush();
	++m_nHeartbeatSeq;
	Send(outbuf);
}

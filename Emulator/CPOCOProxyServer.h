#pragma once
#include <thread>
#include <memory>

class CPOCOProxyAcceptor;

namespace Poco {
	namespace Net {
		class StreamSocket;
		class SocketReactor;
		class ServerSocket;
	}
}

////////////////////////////////////////////////////////////////////////////////////////
/// class CPOCOProxyHandler

class CPOCOProxyHandler
{
public:
	CPOCOProxyHandler(Poco::Net::StreamSocket& front, Poco::Net::StreamSocket& back);
	~CPOCOProxyHandler();

	Poco::Net::StreamSocket& front()
	{
		return m_front;
	}

	Poco::Net::StreamSocket& back()
	{
		return m_back;
	}

	virtual void OnFrontConnect() {}
	virtual void OnFrontDisconnect() {}
    virtual void OnFrontReadable(byte* buffer, int length) {}

	virtual void OnBackConnect() {}
	virtual void OnBackDisconnect() {}
	virtual void OnBackReadable(byte* buffer, int length) {}

private:
	Poco::Net::StreamSocket& m_front;
	Poco::Net::StreamSocket& m_back;
};

////////////////////////////////////////////////////////////////////////////////////////
/// class CPOCOProxyByPassHandler

class CPOCOProxyByPassHandler : public CPOCOProxyHandler
{
public:
	CPOCOProxyByPassHandler(Poco::Net::StreamSocket& front, Poco::Net::StreamSocket& back);
	~CPOCOProxyByPassHandler();
	
	virtual void OnFrontReadable(byte* buffer, int length);
	virtual void OnBackReadable(byte* buffer, int length);
};

////////////////////////////////////////////////////////////////////////////////////////
/// class CPOCOProxyServer

// Front Client <-> CPOCOProxyServer <-> Back Server

/*
    // example 1
	CPOCOProxyServer proxy;
	auto handler = [](Poco::Net::StreamSocket& front, Poco::Net::StreamSocket& back)
	{
		return new CPOCOProxyByPassHandler(front, back);
	};

	proxy.Activate(1470, "10.10.10.88", 4242, handler);
	// Something to do...
	proxy.Deactivate();

	// example 2
	CPOCOProxyServer proxy;	
	proxy.Activate(1470, "10.10.10.88", 4242);
	// Something to do...
	proxy.Deactivate();
*/

typedef std::function<CPOCOProxyHandler* (Poco::Net::StreamSocket&, Poco::Net::StreamSocket&)> HandlerCreator;
 
class CPOCOProxyServer
{
public:
	CPOCOProxyServer();
	~CPOCOProxyServer();
	
	bool Activate(int front_port, const std::string& back_ip, int back_port);
	bool Activate(int front_port, const std::string& back_ip, int back_port, HandlerCreator creator);
	bool Deactivate();

private:
	void ThreadLoop();

private:
	int			m_front_port;
	std::string m_back_ip;
	int			m_back_port;

	std::unique_ptr<Poco::Net::SocketReactor>    m_reactor;
	std::unique_ptr<Poco::Net::ServerSocket>     m_server_socket;
	std::unique_ptr<CPOCOProxyAcceptor>			 m_acceptor;
	HandlerCreator								 m_creator;

	std::thread			m_thread;
	std::atomic<bool>	m_thread_running;
};

